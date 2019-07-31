// NAME: Kanisha Shah, Nina Maller
// EMAIL: kanishapshah@gmail.com, nina.maller@gmail.com
// ID: 504958165, 604955977


#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <poll.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>
#include <ctype.h>
#include "ext2_fs.h"


int fd;
struct ext2_super_block superblock; 
unsigned int block_size;
unsigned int group_count;
struct ext2_group_desc * group_desc;


//helper function for summarize_inodes
void get_time(u_int32_t timestamp, char* buf) {
	time_t epoch = timestamp;
	struct tm ts = *gmtime(&epoch);
	strftime(buf, 80, "%m/%d/%y %H:%M:%S", &ts);
}


// function to produce the one line in csv summarizing the key file system parameters
void summarize_superblock() {
	if(pread(fd, &superblock, sizeof(superblock), 1024) < 0){
        fprintf(stderr, "Failed to read superblock");
        exit(1);
    }

    //get the block size and save in the global variable
    block_size = 1024 << superblock.s_log_block_size; 

    // print:
    // 1. SUPERBLOCK
    // 2. total number of blocks 
    // 3. total number of i-nodes
    // 4. block size
    // 5. i-node size
    // 6. blocks per group
    // 7. i-nodes per group
    // 8. first non-reserved i-node
	fprintf(stdout, "SUPERBLOCK,%d,%d,%d,%d,%d,%d,%d\n", superblock.s_blocks_count, superblock.s_inodes_count, block_size, 
             superblock.s_inode_size, superblock.s_blocks_per_group, superblock.s_inodes_per_group, superblock.s_first_ino);
}


void directory_info(struct ext2_inode * inode, int parent_inode) {
    struct ext2_dir_entry * entry;
    unsigned char block[block_size];
    unsigned int size = 0; // keep track of the bytes read 

    int block_num=0;
    for (; block_num < EXT2_NDIR_BLOCKS; block_num++) {

        long offset = 1024 + (inode->i_block[block_num] - 1) * block_size ;
        pread(fd, block, block_size, offset);
        entry = (struct ext2_dir_entry *) block; // entry in the directory 

        while((size < inode->i_size)  && entry->file_type) {
            char file_name[EXT2_NAME_LEN+1];
            memcpy(file_name, entry->name, entry->name_len);
            file_name[entry->name_len] = 0; // append null char to the file name 

            fprintf(stdout, "DIRENT,%d,%d,%d,%d,%d,'%s'\n", parent_inode, size, entry->inode, entry->rec_len, entry->name_len, file_name) ;

            size += entry->rec_len;
            entry = (void*) entry + entry->rec_len;  // move to the next entry 
        }
    } // end for
}


void indirect_info(int parent_inode, u_int32_t block_num, int indirection_level, u_int32_t logical_offset){
    
    // check the validity of the indirectrion level
    if ( ! (indirection_level == 1 || indirection_level == 2 || indirection_level == 3)){
        fprintf (stderr, "Indirection Level correupted \n") ;
        exit(2) ;
    }
    
    u_int32_t num_ptrs = block_size / sizeof(u_int32_t);
    u_int32_t ptrs [num_ptrs] ;
    long offset = 1024 + (block_num - 1) * block_size ;
    memset (ptrs, 0, sizeof(u_int32_t) * num_ptrs);
    
    pread(fd, ptrs, block_size, offset);
    
    int index = 0;
    for (; index < (int) num_ptrs; index++)
    {
        if (ptrs[index] == 0)
            continue;
        if (indirection_level == 1)
            fprintf(stdout, "INDIRECT,%d,%d,%u,%u,%u\n", parent_inode, indirection_level, logical_offset + index, block_num, ptrs[index]) ;
        else {
            fprintf(stdout, "INDIRECT,%d,%d,%u,%u,%u\n", parent_inode, indirection_level, logical_offset + index, block_num, ptrs[index]) ;
            u_int32_t logical_offset_passed;
            if (indirection_level == 2) {
                logical_offset_passed = logical_offset ;
            }
            else
                logical_offset_passed = logical_offset;
            indirect_info(parent_inode, ptrs[index], indirection_level - 1, logical_offset_passed);
        }
    }
}



// for each group, read the inode and output a summary in csv
void summarize_inodes( int inode_table_id, unsigned int index, unsigned int inode_num){
    
    struct ext2_inode inode; 
    int unsigned i;
    char file_type; 
    u_int16_t mode;
 
	unsigned long offset = (1024 + (inode_table_id - 1) * block_size) + index * sizeof(struct ext2_inode);
	pread(fd, &inode, sizeof(inode), offset);

	if (inode.i_mode == 0 || inode.i_links_count == 0) {
		return;
	}
            // check for the type of the file and update file file_type accordingly
            // get the value of the file first
            u_int16_t file_value = (inode.i_mode >> 12) << 12;
            if (file_value == 0x8000) 
				file_type = 'f';
            else if (file_value == 0x4000) 
				file_type = 'd';
            else if (file_value == 0xa000) 
				file_type = 's';
            else // we dont know so its "?"" 
                file_type = '?';

            // next, update the mode
            mode = inode.i_mode & (0x01C0 | 0x0038 | 0x0007);

            // time of last I-node change (mm/dd/yy hh:mm:ss, GMT)
            char create_time[80];
            get_time(inode.i_ctime, create_time);

            // modification time (mm/dd/yy hh:mm:ss, GMT)
            char mod_time[80];
            get_time(inode.i_mtime, mod_time);

            // time of last access (mm/dd/yy hh:mm:ss, GMT)
            char access_time[80];
            get_time(inode.i_atime, access_time);


            // now print anything:
            printf("INODE,%d,%c,%o,%u,%u,%u,%s,%s,%s,%u,%u", inode_num, file_type, mode, inode.i_uid, inode.i_gid,
					inode.i_links_count, create_time, mod_time, access_time, inode.i_size, inode.i_blocks);

            // also print all addresses
			for (i= 0; i < EXT2_N_BLOCKS; i++) {
				printf(",%u", inode.i_block[i]);
			}
			printf("\n");

    if (file_type == 'd') {
        if (((inode.i_mode >> 12) << 12) != 0x4000)
            exit(1) ;
        directory_info(&inode, inode_num);
    }
    
    // Check indirect blocks
    if (inode.i_block[EXT2_IND_BLOCK] != 0) {
        indirect_info(inode_num, inode.i_block[EXT2_IND_BLOCK], 1, 12);
    }
    if (inode.i_block[EXT2_DIND_BLOCK] != 0) {
        indirect_info(inode_num, inode.i_block[EXT2_DIND_BLOCK], 2, 268);
    }
    if (inode.i_block[EXT2_TIND_BLOCK] != 0) {
        indirect_info(inode_num, inode.i_block[EXT2_TIND_BLOCK], 3, 65804);
    }

}


void inode_info(int num_block, int group_num, int inode_table){
    
    int byteptr, bitptr;
    long offset = 1024 + block_size * (num_block - 1);
    int num_inode_bytes = superblock.s_inodes_per_group / 8;
    char * bitmap_bytes = malloc (num_inode_bytes * sizeof(char));
    pread(fd, bitmap_bytes, num_inode_bytes, offset);
    unsigned int num_inode = (group_num * superblock.s_inodes_per_group) + 1;
    unsigned int first = num_inode;

    for (byteptr=0; byteptr<num_inode_bytes; byteptr++){
        for (bitptr=0; bitptr<8; bitptr++){
            int bitStatus = ( bitmap_bytes[byteptr] >> bitptr) & 1 ;
            if (bitStatus){ // 1 used
                // call function to summarize_inodes
                summarize_inodes(inode_table,num_inode-first,num_inode);
            }
            else { // 0 free
                fprintf (stdout, "IFREE,%d\n", num_inode);
            }
            num_inode ++;
        } //inner for
    }// outer for
}


void group_info(){
    /* calculate number of block groups on the disk */
    group_count = 1 + (superblock.s_blocks_count-1) / superblock.s_blocks_per_group;
   
    /* calculate size of the group descriptor list in bytes */
    unsigned int descr_list_size = group_count * sizeof(struct ext2_group_desc);
    
    group_desc = malloc (descr_list_size ) ;
    if (group_desc == NULL){
        fprintf(stderr, "Error allocating memory to group_desc\n");
        exit (1);
    }
    
    long offset = block_size + 1024 ; //1024 = superblock offset
    pread (fd, group_desc, descr_list_size, offset);
    
    unsigned int i;
    for (i=0; i < group_count;  i++){
        unsigned int num_blocks_group;
        unsigned int num_inodes_group;
        
        if (i == (group_count - 1)) {
            num_blocks_group = superblock.s_blocks_count - (superblock.s_blocks_per_group * (group_count - 1));
            num_inodes_group = superblock.s_inodes_count - (superblock.s_inodes_per_group * (group_count - 1));
        }
        else {
            num_blocks_group = superblock.s_blocks_per_group;
            num_inodes_group = superblock.s_inodes_per_group;
        }
        
        //print group info in csv
        fprintf(stdout, "GROUP,%d,%d,%d,%d,%d,%d,%d,%d\n", i, num_blocks_group, num_inodes_group, group_desc[i].bg_free_blocks_count, 
        group_desc[i].bg_free_inodes_count, group_desc[i].bg_block_bitmap, group_desc[i].bg_inode_bitmap, group_desc[i].bg_inode_table) ;
     
        inode_info(group_desc[i].bg_inode_bitmap, i, group_desc[i].bg_inode_table) ;
    }
}


// fucntion that goes over the bit map and looks from free blocks
// then prints free block
void summarize_free_block() {
unsigned int i, j, k; 
int use;
u_int8_t byte;
int location;

for(i=0; i<group_count; i++){ // go over group
    for(j=0; j<block_size; j++){ // go over bit map
        if (pread(fd, &byte, 1, (block_size * group_desc[i].bg_block_bitmap) + j) < 0) {
            fprintf(stderr, "Failed to read to byte buffer");
            exit(1);
        }
        use = 1;
        for(k=0; k<8; k++){
            // Each bit represent the current state of a block within that block group, where 1 means "used" and 0 "free/available"
            if((byte & use) == 0){ 
                    location = superblock.s_blocks_per_group * i + j * 8 + k + 1;
                    fprintf(stdout, "BFREE,%u\n", location);
				}
                use <<= 1;
        }
    }

}
}



int main(int argc, char **argv) {
    if (argc != 2){
        fprintf (stderr, "Only one argument disk image shoud be passed \n");
        exit (1);
    }
    // *** Parsing Options ***
    static struct option long_options[] =
    {
        {0, 0, 0, 0}
    };
    int c = 0 ;  // option parse character
    while ((c = getopt_long(argc, argv, "",long_options, NULL )) != -1) {
        fprintf(stderr, "No options accepted \n");
        exit (1);
    }
    
    fd = open(argv[1], O_RDONLY);
    if (fd == -1){
        fprintf(stderr, "Cannot open file \n");
        exit (1);
    }

    
    summarize_superblock();
    group_info(); // this one will also call inode for each group
    summarize_free_block(); // summarize free blocks and call the inode summary
                            // function to print inode sumamry. Calls functions to print directory entries and indirect block references


}
