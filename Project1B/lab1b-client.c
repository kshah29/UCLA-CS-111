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


struct termios old_terminal ; // original terminal attributes
int port_number = 0 ; // store --port argument
int log_option = 0 ; // if --log used
int logfd ; // stores fd of log file
int encrypt_option = 0 ; // if --encrypt used
char * key ; // store --encrypt file info
int sockfd ; // stores fd  for end point of socket
char CR = '\r', NL = '\n' ;  // char values used later
struct sockaddr_in server_sockaddr ; //struct used by connect
struct hostent* server_hostent ; // struct used by gethostname
MCRYPT encrypt_td ; //fd for encryption
MCRYPT decrypt_td ; //fd for decryption

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
    struct pollfd poll_array [2] =
    {
        { 0, POLLIN, 0}, // keyboard (stdin)
        { sockfd, POLLIN, 0} // shell output
    };
    
    char buffer [256] ;
    int end_while = 0 ;
    
    while (end_while != 1) {
        
        if (poll(poll_array, 2, 0) > 0) {
            
            // READ from Keyboard (stdin) if input pending
            if (poll_array[0].revents == POLLIN) {
                int canread = read (STDIN_FILENO, &buffer, sizeof(char) * 256) ;
                if (canread > 0) {
                    int i ;
                    for (i=0; i<canread; i++) {
                        if ( buffer[i] == '\r' || buffer[i] == '\n') {
                            write (STDOUT_FILENO, &CR, sizeof(char)) ; // carriage return
                            write (STDOUT_FILENO, &NL, sizeof(char)) ; // newline
                        }
                        else {
                            char c = buffer[i] ;
                            write (STDOUT_FILENO, &c, sizeof(char)) ;
                        }
                    } // end for
                } // end if
                else {
                    fprintf(stderr, "Error: Reading from Keyboard \n");
                    exit (1) ;
                }
                if (encrypt_option) { // encryption
                    if (mcrypt_generic(encrypt_td, &buffer,canread) < 0){
                        fprintf(stderr, "Error encrypting\n");
                        perror("Error") ;
                    }
                    int encrypt_size = strlen(buffer) ;
                    write (sockfd, &buffer, encrypt_size) ;
                    
                    if (log_option){  // outgoing data post-encryption
                        write (logfd, "SENT ", 5*sizeof(char)) ;
                        char bytes [20] ;
                        sprintf(bytes, "%d", encrypt_size) ;
                        write (logfd, &bytes, strlen(bytes)) ;
                        write (logfd, " bytes: ", 8*sizeof(char)) ;
                        write (logfd, &buffer, encrypt_size) ;
                        write (logfd, &NL, sizeof(char)) ;
                    }
                }
                else { // no encryption
                    write (sockfd, &buffer, canread) ;
                    
                    if (log_option){  // outgoing data post-encryption
                        write (logfd, "SENT ", 5*sizeof(char)) ;
                        char bytes [20] ;
                        sprintf(bytes, "%d", canread) ;
                        write (logfd, &bytes, strlen(bytes)) ;
                        write (logfd, " bytes: ", 8*sizeof(char)) ;
                        write (logfd, &buffer, canread) ;
                        write (logfd, &NL, sizeof(char)) ;
                    }
                }
                memset(buffer, 0, sizeof(char) * 256) ;
            } // end if (POLLIN)
            
            // Error in polling
            else if (poll_array[0].revents == POLLERR) {
                fprintf(stderr, "Error with poll with STDIN.\n");
                exit(1);
            }
            
            
            // READ from Socket if input pending
            if (poll_array[1].revents == POLLIN) {
                
                int canread = read (sockfd, &buffer, sizeof(char) * 256) ;
                if (canread < 0) {
                    fprintf(stderr, "Error: Reading from Shell");
                    exit (1) ;
                }
                else if (canread == 0)
                    break ;
                
                if (log_option){ //  incoming data pre-decryption
                    write (logfd, "RECEIVED ", 9*sizeof(char)) ;
                    char bytes [20] ;
                    sprintf(bytes, "%d", canread) ;
                    write (logfd, &bytes, strlen(bytes)) ;
                    write (logfd, " bytes: ", 8*sizeof(char)) ;
                    write (logfd, &buffer, canread) ;
                    write (logfd, &NL, sizeof(char)) ;
                }
                
                if (encrypt_option) { // encryption
                    if (mdecrypt_generic(decrypt_td, &buffer,canread) < 0){
                        fprintf(stderr, "Error encrypting\n");
                        perror("Error") ;
                    }
                    int encrypt_size = strlen(buffer) ;
                    write (STDOUT_FILENO, &buffer, encrypt_size) ;
                }
                else { // no encryption
                    write (STDOUT_FILENO, &buffer, canread) ;
                }
                memset(buffer, 0, sizeof(char) * 256) ;
            } // end if (POLLIN)
            
            // Error in polling
            else if (poll_array[1].revents == POLLERR || poll_array[1].revents == POLLHUP)
                end_while = 1;
            
        } // end if (poll)
    } // end while ()
} // fn end



int main(int argc, char *argv[ ])
{
    // *** Parsing Options ***
    static struct option long_options[] =
    {
        {"port", 1, 0, 'p' },
        {"log", 1, 0, 'l' },
        {"encrypt", 1, 0, 'e' },
        {0, 0, 0, 0}
    };
    char c = 0 ;  // option parse character
    int long_index = 0 ; //index of getopt()
    while ((c = getopt_long(argc, argv, "p:l:e",long_options, &long_index )) != -1) {
        switch(c) {
            case 'p' :
                port_number = atoi(optarg) ;
                break ;
            
            case 'l' :
                log_option = 1 ;
                logfd = creat(optarg, 0666) ;
                if ( logfd < 0)
                    fprintf(stderr,  "Error: could not create file \n") ;
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
    
    set_input_mode() ; // setup non-canonical input mode
    
    // create connection
    sockfd = socket(AF_INET, SOCK_STREAM, 0) ;
    if (sockfd < 0){
        fprintf(stderr, "Error while creating socket") ;
        exit(1) ;
    }
    
    memset( (void * ) &server_sockaddr, 0, sizeof(server_sockaddr)) ;
    
    server_hostent = gethostbyname("localhost") ;
    server_sockaddr.sin_family = AF_INET ;
    server_sockaddr.sin_port = htons(port_number) ;
    memcpy( (char *) &server_sockaddr.sin_addr.s_addr,(char *) server_hostent->h_addr, server_hostent->h_length);
    
    if ( connect (sockfd, (struct sockaddr *) &server_sockaddr, sizeof( server_sockaddr))  < 0){
        fprintf(stderr, "Error in initiating a connect on a socket \n") ;
        perror("Error") ;
        exit (1) ;
    }

    read_write () ;
    
    close (sockfd) ;
    close (logfd)  ;
    mcrypt_generic_deinit(encrypt_td) ;
    mcrypt_module_close(encrypt_td);
    mcrypt_generic_deinit(decrypt_td) ;
    mcrypt_module_close(decrypt_td);
    reset_input_mode () ;
    exit (0) ; // normal execution
}
