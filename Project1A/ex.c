#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <termios.h>
#include <poll.h>
#include <sys/wait.h>


char *shell_program = NULL ; // store --shell argument
pid_t cpid ;
    int main(int argc, char *argv[ ])
    {
        // *** Parsing Options ***
        static struct option long_options[] =
        {
            {"shell", 1, 0, 's' },
            {0, 0, 0, 0}
        };
        char c = 0 ;  // option parse character
        int long_index = 0 ; //index of getopt()
        while ((c = getopt_long(argc, argv, "s",long_options, &long_index )) != -1) {
            switch(c) {
                case 's' :
                    shell_program = optarg ;
                    printf ("%s \n", shell_program);
                    break ;
                    
                default:
                    fprintf (stderr, "Error: Unrecognized option \n") ;
                    exit (1) ;
            } // end of switch
        } // end of while
        
        if ( shell_program != NULL) {
            //        printf ("Inside \n");
            cpid = fork();
            if (cpid == 0)
            {
//                ret = execlp ("ls", "ls", "-l", (char *)0);
//                char *cmd[] = {shell_program, "", (char *)0 } ; //execlp can be used too
                if ( execlp (shell_program, shell_program, NULL, (char *)0) < 0 ) {
                    perror("exec Error");
                    exit(1);
                }
                
                
            }
            int status ;
            if (waitpid(cpid, &status, 0) == -1) {  // error calling waitpid
                perror("waitpid error");
                exit(1);
            }
            fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", WIFSIGNALED(status), WEXITSTATUS(status)) ;

        }
        
    return 0;
} 
