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
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <mcrypt.h>



int port_number = 0 ; // store --shell argument
int encrypt_option = 0; //stores if --encrypt option is passed
int pipePtoC [2];   // pipe from Child to Parent
int pipeCtoP [2];   // pipe from Parent to Child
pid_t cpid;   // ID for new process
char CR = '\r', NL = '\n' ;  // char values used later
struct sockaddr_in server_sockaddr, client_sockaddr ;
int sockfd0 ; // old sockfd
int sockfd1 ; // new fd returned by accept
MCRYPT encrypt_td ; //fd for encryption
MCRYPT decrypt_td ; //fd for decryption
char * key ; // store --encrypt file info

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
    close(sockfd0);
    close(sockfd1);
    mcrypt_generic_deinit(decrypt_td) ;
    mcrypt_module_close(decrypt_td);
    mcrypt_generic_deinit(encrypt_td) ;
    mcrypt_module_close(encrypt_td);
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
    exit(0);
}

void sigint_handler()
{
    kill(cpid, SIGINT);
    exit_handler();
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
    
    // execute shell
    if (execlp("/bin/bash", "/bin/bash", NULL, (char*) 0) < 0) {
        fprintf(stderr, "Failed to exec a shell.\n");
        exit(1);
    }


}


void parent_process ()
{
    close(pipePtoC[0]); // Close unused read end from parent
    close(pipeCtoP[1]); // Close unused write end from child
    
    struct pollfd poll_array [2] =
    {
        { sockfd1, POLLIN, 0}, // socket
        { pipeCtoP[0], POLLIN, 0} // shell output
    };
    
    char buffer [256] ;
    int end_while = 0 ;
    
    while (end_while != 1) {
        
        if (poll(poll_array, 2, 0) > 0) {
            
            // READ from socket if input pending
            if (poll_array[0].revents == POLLIN) {
                int canread = read (sockfd1, &buffer, sizeof(char) * 256) ;
                if (encrypt_option){
                    if (mdecrypt_generic(decrypt_td, &buffer,canread) < 0){
                        fprintf(stderr, "Error encrypting\n");
                        perror("Error") ;
                    }
                    canread = strlen(buffer) ;
                }
                
                if (canread > 0) {
                    int i;
                    for (i=0; i<canread; i++) {
                        if ( buffer[i] == '\r' || buffer[i] == '\n') {
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
                            write (pipePtoC[1], &c, sizeof(char)) ;
                        }
                    } // end for
                    memset(buffer, 0, sizeof(char) * 256) ;
                } // end if
                else {
                    fprintf(stderr, "Error: Reading from Keyboard\n");
                    exit (1) ;
                }
            } // end if (POLLIN)
            
            // Error in polling
            else if (poll_array[0].revents == POLLERR) {
                fprintf(stderr, "Error with poll with STDIN.\n");
                exit(1);
            }
            
        
            // READ from Shell output if input pending
            if (poll_array[1].revents == POLLIN) {
                int canread = read (pipeCtoP[0], &buffer, sizeof(char) * 256) ;
                if (canread > 0) {
                    if (!encrypt_option)
                        write (sockfd1, &buffer, canread) ;
                    else {
                        if (mcrypt_generic(encrypt_td, &buffer,canread) < 0){
                            fprintf(stderr, "Error encrypting\n");
                            perror("Error") ;
                        }
                        int encrypt_size = strlen(buffer) ;
                        write (sockfd1, &buffer, encrypt_size) ;
                    }
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
    signal(SIGPIPE, sigpipe_handler);
    signal(SIGINT, sigint_handler);
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
        {"port", 1, 0, 'p' },
        {"encrypt", 1, 0, 'e' },
        {0, 0, 0, 0}
    };
    char c = 0 ;  // option parse character
    int long_index = 0 ; //index of getopt()
    while ((c = getopt_long(argc, argv, "p:e:s",long_options, &long_index )) != -1) {
        switch(c) {
            case 'p' :
                port_number = atoi(optarg) ;
                break ;
                
            case 'e' :
                encrypt_option = 1;
                int fd = open(optarg, O_RDONLY) ;
                char buffer;
                int length=0 ;
                while (read(fd, &buffer, sizeof(char)) > 0){
                    length = length+1;
                    key = (char *) realloc (key,length) ;
                    key[length-1] = buffer;
                }
                key[length] = '\0' ;
                encrypt_td = mcrypt_module_open("twofish", NULL, "cfb", NULL);
                int size = mcrypt_enc_get_iv_size(encrypt_td);
                char *IV ;
                IV = malloc(size) ;
                int i ;
                for(i=0;i<size;i++)
                    IV[i] = rand();
                if (mcrypt_generic_init(encrypt_td, key, length, IV) < 0) {
                    fprintf(stderr, "Error in mcrypt_generic_init \n") ;
                    perror("Error") ;
                }
                decrypt_td = mcrypt_module_open("twofish", NULL, "cfb", NULL);
                if (mcrypt_generic_init(decrypt_td, key, length, IV) < 0) {
                    fprintf(stderr, "Error in mcrypt_generic_init \n") ;
                    perror("Error") ;
                }
                break ;
                
            default:
                fprintf (stderr, "Error: Unrecognized option \n") ;
                exit (1) ;
        } // end of switch
    } // end of while
    
    if (port_number == 0){
        fprintf(stderr, "Error: Port Number not provided \n") ;
        exit (1) ;
    }
    
    // create connection
    sockfd0 = socket(AF_INET, SOCK_STREAM, 0) ;
    if (sockfd0 < 0){
        fprintf(stderr, "Error while creating socket \n") ;
        exit(1) ;
    }
    
    
    memset( (void *) &server_sockaddr, 0, sizeof(server_sockaddr));
    server_sockaddr.sin_family = AF_INET ;
    server_sockaddr.sin_port = htons(port_number) ;
    server_sockaddr.sin_addr.s_addr = INADDR_ANY ;
    
    if ( bind( sockfd0, (struct sockaddr *) &server_sockaddr, sizeof(server_sockaddr))  < 0){
        fprintf(stderr, "Error while binding \n") ;
        perror("Error") ;
        exit (1) ;
    }

    listen(sockfd0, 5);
    unsigned int client_size = sizeof(client_sockaddr) ;
    sockfd1 =  accept(sockfd0, (struct sockaddr *) &client_sockaddr, (socklen_t *) &client_size);
    if (sockfd1 < 0){
        fprintf(stderr, "Error while accepting \n") ;
        perror("Error") ;
        exit(1) ;
    }

    setup_pipe () ;
    setup_process () ;
   
    exit (0) ; // normal execution
}


