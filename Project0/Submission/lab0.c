// NAME: Kanisha Shah
// EMAIL: kanishapshah@gmail.com
// ID: 504958165


#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>


void sighandler ()
{
    fprintf (stderr, "Error: Segmentation Fault \n") ;
    exit (4) ;
}


int main(int argc, char *argv[ ])
{
    char *input_file;
    char *output_file;
    char c = 0 ;  // option parse character
    char buffer ; // read() and write () buffer
    int long_index = 0 ; //index of getopt()
    int segfault = 0;  // flags parsed from options
    
    
    // *** Parsing Options ***
    
    static struct option long_options[] =
    {
        {"input", 1, 0,  'i' },
        {"output", 1, 0,  'o' },
        {"segfault", 0, 0, 's'},
        {"catch", 0, 0, 'c'},
        {0, 0, 0, 0}
    };
    
    while ((c = getopt_long(argc, argv, "sci:o",long_options, &long_index )) != -1)
    {
        switch(c)
        {
            case 'i' :
                input_file = optarg ;
                // *** 1. File Redirection ***
                int fd_input = open (input_file, O_RDONLY) ;
                if (fd_input >= 0)  // change file descriptor for input file
                {
                    close (0) ;
                    dup(fd_input) ;
                    close (fd_input) ;
                }
                else
                {
                    fprintf (stderr,"Error with Input file \n") ;
                    fprintf(stderr, "Error: %s\n", strerror(errno));
                    perror ("Error") ;
                    exit (2) ;
                }
                break ;
                
            case 'o' :
                output_file = optarg ;
                // *** 1. File Redirection ***
                int fd_output = creat (output_file, 0666) ;
                if (fd_output >= 0)   // change file descriptor for output file
                {
                    close (1) ;
                    dup(fd_output) ;
                    close (fd_output) ;
                }
                else
                {
                    fprintf (stderr,"Error with Output file \n") ;
                    fprintf(stderr, "Error: %s\n", strerror(errno));
                    perror ("Error") ;
                    exit (3) ;
                }
                break ;
                
            case 'c' :
                // *** 2. Register the Signal sighandler ***
                signal(SIGSEGV, sighandler) ;
                // printf("Catch\n") ;
                break ;
                
            case 's' :
                segfault = 1;
                // printf("Segfault\n") ;
                break ;
                
            default:
                fprintf (stderr, "Error: Unrecognized option \n") ;
                if (input_file == NULL) fprintf (stderr, "Need argument for --input \n");
                if (output_file == NULL) fprintf (stderr, "Need argument for --output \n");
                exit (1) ;
        } // end of switch
    } // end of while
    

    // *** 3. Cause the Segfault ***
    if (segfault)
    {
        char * c = NULL ;
        *c = 'a' ;
    }
    
    
    // *** 4. Copy stdin (fd = 0) to stdout (fd = 1) ***
    
    ssize_t canwrite ;
    ssize_t canread = read(0, &buffer, sizeof(char)) ;
    if (canread == -1)
    {
        fprintf(stderr,"Error reading \n") ;
        fprintf(stderr, "Error: %s\n", strerror(errno));
        perror ("Error") ;
        exit(-1) ;
    }
    
    while ( canread > 0)
    {
        canwrite = write(1, &buffer, sizeof(char)) ;
        if (canwrite == -1)
        {
            fprintf(stderr,"Error writing \n") ;
            fprintf(stderr, "Error: %s\n", strerror(errno));
            perror ("Error") ;
            exit(-1);
        }
        canread = read(0, &buffer, sizeof(char)) ;
    }
    
    close(0);
    close(1);
    exit (0) ;
}
