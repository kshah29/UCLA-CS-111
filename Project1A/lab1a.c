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
#include <termios.h>
#include <poll.h>
#include <sys/wait.h>


struct termios old_terminal ; // original terminal attributes
char *shell_program = NULL ; // store --shell argument
int pipePtoC [2];   // pipe from Child to Parent
int pipeCtoP [2];   // pipe from Parent to Child
pid_t cpid;   // ID for new process
char CR = '\r', NL = '\n' ;  // char values used later

void reset_input_mode (void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal);
}

void set_input_mode (void)
{
    struct termios new_terminal;
    
    /* Make sure stdin is a terminal. */
    if (!isatty(STDIN_FILENO))
    {
        fprintf (stderr, "Not a terminal.\n");
        exit (1);
    }
    
    /* Save the terminal attributes so we can restore them later. */
    tcgetattr(STDIN_FILENO, &old_terminal);
    atexit(reset_input_mode);
    
    /* Set the terminal modes*/
    tcgetattr (STDIN_FILENO, &new_terminal);
    new_terminal.c_iflag = ISTRIP;
    new_terminal.c_oflag = 0;
    new_terminal.c_lflag = 0;
    
    tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal);
}


void read_write ()
{
    char buffer [256] ;
    ssize_t canread = read (STDIN_FILENO, buffer, sizeof(char) * 256) ;
    while (canread > 0) {
        for (int i=0; i<canread; i++) {
            if ( buffer[i] == CR || buffer[i] == NL) {
                write (STDOUT_FILENO, &CR, sizeof(char)) ; // carriage return
                write (STDOUT_FILENO, &NL, sizeof(char)) ; // newline
            }
            else if ( buffer[i] == '\004') {  // detects ^D
                reset_input_mode () ;
                exit(0) ;
            }
            else {
                char c = buffer[i] ;
                write (STDOUT_FILENO, &c, sizeof(char)) ;
            }
        } // end for
        
        memset(buffer, 0, sizeof(char) * 256) ;
        canread = read (STDIN_FILENO, buffer, sizeof(char) * 256) ;
    } // end while
} // end function


void setup_pipe ()  // setup pipes for Parent to child and other way around
{
    if (pipe(pipePtoC) == -1) {
        perror("pipe Error");
        exit(1);
    }
    if (pipe(pipeCtoP) == -1) {
        perror("pipe Error");
        exit(1);
    }
}


void exit_handler ()
{
    close(pipePtoC[1]);
    close(pipeCtoP[0]);
    int status ;
    if (waitpid(cpid, &status, 0) == -1) {  // error calling waitpid
        perror("waitpid error");
        exit(1);
    }
    fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", WIFSIGNALED(status), WEXITSTATUS(status)) ;
}


void sigpipe_handler()
{
    kill(cpid, SIGPIPE);
    exit_handler();
    reset_input_mode () ;
    exit(0);
}


void child_process ()
{
    //    printf ("Child \n") ;
    close(pipePtoC[1]);  // Close unused write end from parent to child
    close(pipeCtoP[0]);  // Close unused read end from child to parent

    dup2(pipePtoC[0], 0) ; // read terminal -> shell (P -> C)
    dup2(pipeCtoP[1], 1) ; // output shell -> terminal (C -> P)
    dup2(pipeCtoP[1], 2) ; // error shell -> terminal (C -> P)

    close (pipePtoC[0]) ;
    close (pipeCtoP[1]) ;

    if ( execlp (shell_program, shell_program, NULL, (char *)0) < 0 ) {
        perror("exec Error");
        exit(1);
    }
}


void parent_process ()
{
    close(pipePtoC[0]); // Close unused read end from parent
    close(pipeCtoP[1]); // Close unused write end from child
    
    struct pollfd poll_array [2] =
    {
        { 0, POLLIN, 0}, // keyboard (stdin)
        { pipeCtoP[0], POLLIN, 0} // shell output
    };
    
    char buffer [256] ;
    int end_while = 0 ;
    
    while (end_while != 1) {
        
        if (poll(poll_array, 2, 0) > 0) {
            
            // READ from Keyboard (stdin) if input pending
            if (poll_array[0].revents == POLLIN) {
                ssize_t canread = read (STDIN_FILENO, buffer, sizeof(char) * 256) ;
                if (canread > 0) {
                    for (int i=0; i<canread; i++) {
                        if ( buffer[i] == '\r' || buffer[i] == '\n') {
                            write (STDOUT_FILENO, &CR, sizeof(char)) ; // carriage return
                            write (STDOUT_FILENO, &NL, sizeof(char)) ; // newline
                            write (pipePtoC[1], &NL, sizeof(char)) ; // newline
                        }
                        else if ( buffer[i] == '\004') {  // detects ^D
                            end_while = 1 ;
                        }
                        else if ( buffer[i] == '\003') {  // detects ^C
                            kill(cpid, SIGINT) ;
                        }
                        else {
                            char c = buffer[i] ;
                            write (STDOUT_FILENO, &c, sizeof(char)) ;
                            write (pipePtoC[1], &c, sizeof(char)) ;
                        }
                    } // end for
                    memset(buffer, 0, sizeof(char) * 256) ;
                } // end if
                else {
                    fprintf(stderr, "Error: Reading from Keyboard");
                    exit (1) ;
                }
            } // end if (POLLIN)
            else if (poll_array[0].revents == POLLERR) {
                fprintf(stderr, "Error with poll with STDIN.\n");
                exit(1);
            }
            
            // READ from Shell output if input pending
            if (poll_array[1].revents == POLLIN) {
                
                ssize_t canread = read (pipeCtoP[0], buffer, sizeof(char) * 256) ;
                if (canread > 0) {
                    for (int i=0; i<canread; i++) {
                        if ( buffer[i] == '\n') {
                            write (STDOUT_FILENO, &CR, sizeof(char)) ; // carriage return
                            write (STDOUT_FILENO, &NL, sizeof(char)) ; // newline
                        }
                        else if ( buffer[i] == '\004') {  // detects ^D
                            end_while = 1 ;
                        }
                        else {
                            char c = buffer [i] ;
                            write (STDOUT_FILENO, &c, sizeof(char)) ;
                        }
                    } // end for
                    memset(buffer, 0, sizeof(char) * 256) ;
                } // end if
                else {
                    fprintf(stderr, "Error: Reading from Shell");
                    exit (1) ;
                }
            } // end if (POLLIN)
            // Error in polling
            else if (poll_array[1].revents == POLLERR || poll_array[1].revents == POLLHUP)
                end_while = 1;
        }// end if (poll > 0)
    } // end while
    exit_handler() ;
    exit (0) ;
} // end function


void setup_process ()
{
    set_input_mode () ; // store original terminal settings and then modify them
    signal(SIGPIPE, sigpipe_handler);
    cpid = fork();
    if (cpid == -1) {
        perror("fork Error");
        exit(1);
    }
 
    if (cpid == 0)   // Child Process (shell)
        child_process ();
        
    else if (cpid > 0)  // Parent Process
        parent_process ();
        
    
} // function end


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
//                printf ("%s \n", shell_program);
                break ;
            
            default:
                fprintf (stderr, "Error: Unrecognized option \n") ;
                exit (1) ;
        } // end of switch
    } // end of while
    
    if ( shell_program != NULL) {
        setup_pipe () ;
        setup_process () ;
    }
    else
        set_input_mode () ; // store original terminal settings and then modify them
    read_write () ;

    
    exit (0) ; // normal execution
} 


