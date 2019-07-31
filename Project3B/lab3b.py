import csv # read csv file
import os # check if file path exists
import sys # print stuff / exit with errros

# Define lists
superblock = None
group = None
free_inodes = []
free_blocks = []
inode_list = []
group_list = []
dirent_list = []
indirect_list = []
errors = 0
revised_free_inodes = []
revised_inodes_list = []

# Define all classes:
class Superblock:
	def __init__(self, row):
		self.total_block_num = int(row[1])
		self.total_inodes_num = int(row[2])
		self.block_size = int(row[3])
		self.inode_size = int(row[4])
		self.blocks_per_group = int(row[5])
		self.inodes_per_group = int(row[6])
		self.first_nonreserved_inode = int(row[7])

class Group:
    def __init__(self, row):
        self.group_num = int(row[1])
        self.total_blocks_in_group = int(row[2])
        self.total_inodes_in_group = int(row[3])
        self.total_free_blocks = int(row[4])
        self.total_free_inodes = int(row[5])
        # 3 more here? we dont need them.
        #block number of free block bitmap for this group (decimal)
        #block number of free i-node bitmap for this group (decimal)
        #block number of first block of i-nodes in this group (decimal)

class Inode:
	def __init__(self, row):
		self.inode_num = int(row[1])
		self.file_type = row[2]
		self.mode = int(row[3])
		self.owner = int(row[4])
		self.group = int(row[5])
		self.link_count = int(row[6])
		self.ctime = row[7]
		self.mtime = row[8]
		self.atime = row[9]
		self.file_size = int(row[10])
		self.num_blocks_taken = int(row[11])
		self.blocks = map(int, row[12:24])
		self.single_ind = int(row[24])
		self.double_ind = int(row[25])
		self.triple_ind = int(row[26])

class Dirent:
    def __init__(self, row):
        self.parent_inode = int(row[1])
        self.logical_offset = int(row[2])
        self.inode_num = int(row[3])
        self.entry_length = int(row[4])
        self.name_length = int(row[5])
        self.name = row[6].rstrip()

class Indirect:
    def __init__(self, row):
        self.inode_num = int(row[1])
        self.indirection_level = int(row[2])
        self.logical_offset = int(row[3])
        self.block_num = int(row[4])
        self.reference_num = int(row[5])



def parse_file (filename) :
    # first check if file exists!
    if not os.path.isfile(filename):
    	sys.stderr.write("ERROR: file does not exist!")
    	sys.exit(1) # error, bad parameter
    # define all globals
    global superblock, group, free_blocks, free_inodes, inode_list, dirent_list, indirect_list

    with open(filename) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        for row in csv_reader:
            if row[0] == "SUPERBLOCK":
                superblock = Superblock(row)
            elif row[0] == "GROUP":
                group = Group(row)
            elif row[0] == "BFREE":
                free_blocks.append(int(row[1]))
            elif row[0] == "IFREE":
                free_inodes.append(int(row[1]))
            elif row[0] == "INODE":
                inode_list.append(Inode(row))
            elif row[0] == "DIRENT":
                dirent_list.append(Dirent(row))
            elif row[0] == "INDIRECT":
                indirect_list.append(Indirect(row))
            else:
                sys.stderr.write("ERROR: Invalid Line in CSV file")
                sys.exit(1)



# Fucntion that looks at all block pointers in the I-node and checks for invalid and
# reserved blocks
def check_block_consistency():
    global errors
    # we need a list of block that are referenced by inodes!
    referenced_blocks_num = [] # a list of block numbers that were referenced
    #since I messed up, I need another list that keeps track of the level, block, inode, and offset
    referenced_blocks = {}
    # we need to initialize this list... Otherwise we cant append!
    for num in range(0, superblock.total_block_num):
        referenced_blocks[num] = [(" ",0,0)]

    # we need to examine the blocks based on the level of indirection
    for inode in inode_list: # go over all inodes
        for offset, block in enumerate(inode.blocks):
            
            if block == 0:
                break
            # these will all be indirect blocks
            # Check if valid:
            if block > superblock.total_block_num:
                errors += 1
                print("INVALID BLOCK " + str(block) +  " IN INODE " + str(inode.inode_num) + " AT OFFSET " + str(offset) )
            # check if the block is reserved:
            elif block < 8:
                errors += 1
                print("RESERVED BLOCK " + str(block) + " IN INODE " + str(inode.inode_num) + " AT OFFSET " + str(offset) )
            # some stuff here
            else:
                # this is a valid block
                # add it to a list of all referenced blocks! 
                # Then check later if a block is referenced or not
            #    print("appending block num ", block)
                referenced_blocks_num.append(block)
                referenced_blocks[block].append((" ", offset, inode.inode_num))
          

        # Check 1st indirection:
            # Check if valid:
        if inode.single_ind != 0 :
            if inode.single_ind > superblock.total_block_num:
                errors += 1
                print("INVALID INDIRECT BLOCK " + str(inode.single_ind) + " IN INODE " + str(inode.inode_num) + " AT OFFSET 12" )
        # check if the block is reserved:
            elif inode.single_ind < 8:
                errors += 1
                print("RESERVED INDIRECT BLOCK " + str(inode.single_ind) + " IN INODE " + str(inode.inode_num) + " AT OFFSET 12")
            # some stuff here
            else:
             #   print("appending block num ", inode.single_ind)
                referenced_blocks_num.append(inode.single_ind)
                referenced_blocks[inode.single_ind].append((" INDIRECT ", 12, inode.inode_num))
                # print what already here!
            #    print("This is what is here now: ")
            #    for level, offset, inode_num in referenced_blocks[inode.single_ind]:
            #        print("BLOCK" , block)
            #    print("~~~~")
  
        
            # Check 2nd indirection:
        if inode.double_ind != 0 :
            # Check if valid:
            if inode.double_ind > superblock.total_block_num:
                errors += 1
                print("INVALID DOUBLE INDIRECT BLOCK " + str(inode.double_ind) + " IN INODE " + str(inode.inode_num) + " AT OFFSET 268" )
            # check if the block is reserved:
            elif inode.double_ind < 8:
                errors += 1
                print("RESERVED DOUBLE INDIRECT BLOCK " + str(inode.double_ind) + " IN INODE " + str(inode.inode_num) + " AT OFFSET 268")
            # some stuff here
            else:
              #  print("appending block num ", inode.double_ind)
                referenced_blocks_num.append(inode.double_ind)
                referenced_blocks[inode.double_ind].append((" DOUBLE INDIRECT ", 268, inode.inode_num))
           

        # Check 3rd indirection:
        if inode.triple_ind != 0 :
            # Check if valid:
            if inode.triple_ind > superblock.total_block_num:
                errors += 1
                print("INVALID TRIPLE INDIRECT BLOCK " + str(inode.triple_ind) + " IN INODE " + str(inode.inode_num) + " AT OFFSET 65804" )
            # check if the block is reserved:
            elif inode.triple_ind < 8:
                errors += 1
                print("RESERVED TRIPLE INDIRECT BLOCK " + str(inode.triple_ind) + " IN INODE " + str(inode.inode_num) + " AT OFFSET 65804")
            # some stuff here
            else:
               # print("appending block num ", inode.triple_ind)
                referenced_blocks_num.append(inode.triple_ind)
                referenced_blocks[inode.triple_ind].append((" TRIPLE INDIRECT ", 65804, inode.inode_num))
      
  

    for block in indirect_list:
        # check level of indirection in this block
        level = ""
        if block.indirection_level == 1:
            level = " INDIRECT "
        elif block.indirection_level == 2:
            level = " DOUBLE INDIRECT "
        elif block.indirection_level == 3:
            level = " TRIPLE INDIRECT "

        if block.reference_num != 0 :
        # Check if valid:
            if block.reference_num > superblock.total_block_num:
                errors += 1
                print(" INVALID " + level + " BLOCK " + str(block.reference_num) + " IN INODE " + str(block.inode_num) + " AT OFFSET " + str(block.logical_offset) )
            # check if the block is reserved:
            elif block.reference_num < 8:
                errors += 1
                print("RESERVED" + level + "BLOCK " + str(block.reference_num) + " IN INODE " + str(block.inode_num) + " AT OFFSET " + str(block.logical_offset))
            # some stuff here
            else:
              #  print("appending block num ", block.reference_num)
                referenced_blocks_num.append(block.reference_num)
                referenced_blocks[block.reference_num].append((" SOME INDIRECT ", block.logical_offset, block.inode_num))
                
  
        

    # handle unreferenced and allocted 
    # unrefereced = not referenced by file and not in the free list
    for block in range (8, superblock.total_block_num):
        if block not in free_blocks: 
            if block not in referenced_blocks_num:
                errors += 1
                print("UNREFERENCED BLOCK "+ str(block) )
        if block in free_blocks:
            if block in referenced_blocks_num:
                errors += 1
                print("ALLOCATED BLOCK "+ str(block) + " ON FREELIST")
        
    # handle duplicates!
    # I have a seperate list for this one becasue I didnt realize
    # that I need to keep track of other things as well
    for block in range(8, superblock.total_block_num):
        if block in referenced_blocks_num: # compare numbers
            if len(referenced_blocks[block]) > 2: # there is more than 1!
                for level, offset, inode_num in referenced_blocks[block]:
                    if inode_num!= 0:
                        errors += 1
                        print("DUPLICATE" + level + "BLOCK " + str(block) + " IN INODE " + str(inode_num) + " AT OFFSET "+ str(offset))





            

# Fucntion to scan through all of the I-nodes to determine which are valid/allocated
def check_inode_allocation():

	# define all globals
	global free_inodes, inode_list, errors, revised_free_inodes, revised_inodes_list, superblock
	revised_free_inodes = free_inodes

	for i in range (0, superblock.total_inodes_num):
		is_inode = False
		is_free_inode = False
		if i in free_inodes:
			is_free_inode = True
		for inode in inode_list:
			if i == inode.inode_num:
				is_inode = True
				break
		if is_inode == is_free_inode:
			if is_free_inode:
				print("ALLOCATED INODE " + str(i) + " ON FREELIST")
				errors += 1
			else:
				if i < 11 and i != 2:
					continue
				print("UNALLOCATED INODE " + str(i) + " NOT ON FREELIST")
				errors += 1


	for inode in inode_list:
		if inode.file_type == '0':
			if inode.inode_num not in free_inodes:
				revised_free_inodes.append(inode.inode_num)
		else:
			revised_inodes_list.append(inode)
			if inode.inode_num in free_inodes:
				revised_free_inodes.remove(inode.inode_num)



# Fucntion that scans all directories and reports inconsistencies
def check_directory_consistency():

	# define all globals
    global errors,  dirent_list, inode_list, superblock, revised_free_inodes, revised_inodes_list
    direct_linkcount = {}
    #inode_num_visited = [] # we are not using this
    # make a map that indexes inode number to parent inode number 
    inode_map_C_P = {2:2} 
    
    # We need to put values in inode_map_C_P
    for dirent in dirent_list:
        if dirent.inode_num not in revised_free_inodes:
            if dirent.name != "'.'" and dirent.name != "'..'":
                inode_map_C_P[dirent.inode_num] = dirent.parent_inode
            #    print("added inode ", dirent.inode_num, " to the list!!!")

	# free_inode: need to make a new list for actual unallocated lists
    for dirent in dirent_list:		
        inode_num = dirent.inode_num #dirent.parent_inode
        if dirent.inode_num <= superblock.total_inodes_num and dirent.inode_num not in revised_free_inodes :
            if inode_num in direct_linkcount :
                value = direct_linkcount[inode_num]
                direct_linkcount[inode_num] = value + 1
            else:
                direct_linkcount[inode_num] = int(1)

        if dirent.name != "'.'" and dirent.name != "'..'" :
            if dirent.inode_num <= superblock.total_inodes_num and dirent.inode_num not in revised_free_inodes :
                inode_map_C_P[dirent.inode_num] = dirent.parent_inode
    for dirent in dirent_list:
        if dirent.inode_num > superblock.total_inodes_num:
            print("DIRECTORY INODE " + str(dirent.parent_inode) + " NAME " + dirent.name + " INVALID INODE " + str(dirent.inode_num) )
            errors += 1
        if dirent.inode_num in revised_free_inodes:
            print("DIRECTORY INODE " + str(dirent.parent_inode) + " NAME " + dirent.name + " UNALLOCATED INODE " + str(dirent.inode_num) )
            errors += 1

        if dirent.name == "'.'" and dirent.parent_inode != dirent.inode_num :
            print("DIRECTORY INODE " + str(dirent.parent_inode) + " NAME '.' LINK TO INODE " + str(dirent.inode_num) + " SHOULD BE " + str(dirent.parent_inode))
            errors += 1
        if dirent.name == "'..'":
         #   print("got here with node " + str(dirent.parent_inode) ) 
           # if dirent.parent_inode in inode_map_C_P:
          #      print("got to second step with node " + str(dirent.parent_inode) )
            if dirent.inode_num != inode_map_C_P[dirent.parent_inode]:
                print("DIRECTORY INODE " + str(dirent.parent_inode) + " NAME '..' LINK TO INODE " + str(dirent.inode_num) + " SHOULD BE " + str(inode_map_C_P[dirent.parent_inode]) )
                errors += 1

    for inode in revised_inodes_list:
        if inode.inode_num in direct_linkcount :
            if direct_linkcount[inode.inode_num] != inode.link_count:
                # print("INODE " + str(inode.inode_num) + " HAS " + str(inode.link_count) + " LINKS BUT LINKCOUNT IS " + str(direct_linkcount[inode.inode_num]) )
                print("INODE " + str(inode.inode_num) + " HAS " + str(direct_linkcount[inode.inode_num]) + " LINKS BUT LINKCOUNT IS " + str(inode.link_count) )
                errors += 1
        else:
            if inode.link_count != 0:
                # print("INODE " + str(inode.inode_num) + " HAS " + str(inode.link_count) + " LINKS BUT LINKCOUNT IS 0" )
                print("INODE " + str(inode.inode_num) + " HAS 0 LINKS BUT LINKCOUNT IS " + str(inode.link_count) )
                errors += 1







def main():
    # check there is a correct number of arguments
    if (len(sys.argv) != 2):
        sys.stderr.write ("Incorrect number of arguments provided")
        exit(1) # exit with 1 becasue of bad parameters 

    # get csv here
    parse_file (sys.argv[1])
    check_block_consistency()
    check_inode_allocation()
    check_directory_consistency()

    # check how many inconsistencies and exit accordingly
    if errors == 0:
        exit(0) # success!
    else:
        exit(2) # successful execution, inconsistencies found

if __name__ == '__main__':
	main()
