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
#include <poll.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>
#include <ctype.h>
#include <math.h>
#include <mraa.h>
#include <mraa/aio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

int log_option;
int report_log = 1;
char scale = 'F';
FILE* logfd = 0;
int period = 1;
int next = 0;
struct timeval clock_now;
struct tm * time_now ;
mraa_aio_context tSens;
//mraa_gpio_context button;

int id = 0;
char * host = "";
int port = 0;

struct sockaddr_in server_sockaddr ; //struct used by connect
struct hostent* server_hostent ; // struct used by gethostname
int sockfd ; // stores fd  for end point of socket

SSL *ssl = NULL;
SSL_CTX *ctx = NULL;

void printString(const char* string){
    if (log_option)
        fprintf(logfd, "%s\n", string);
    fflush(logfd) ;
}

void shutdown_connection(){
    time_now = localtime(&clock_now.tv_sec);
    char buf_out[256];
    sprintf(buf_out, "%02d:%02d:%02d SHUTDOWN\n", time_now->tm_hour, time_now->tm_min, time_now->tm_sec);
    SSL_write(ssl, buf_out, strlen(buf_out));
    if (log_option)
        fputs(buf_out, logfd) ;
//    mraa_gpio_close(button);
    mraa_aio_close(tSens);
    exit(0);
}

int same (const char* buffer, const char* string){
    if (strlen(buffer) != strlen(string) + 1)
        return 0 ;
    int index=0;
    int length = strlen(string);
    for (; index< length; index++) {
        if (buffer[index] == '\n') break;
        if (buffer[index] != string[index])
            return 0;
    }
    return 1;
        
}

void initialize (){
    tSens = mraa_aio_init(1);
//    button = mraa_gpio_init(60);
//    mraa_gpio_dir(button, MRAA_GPIO_IN) ;
    
    if (tSens == NULL){ //} || button == NULL){
        fprintf(stderr, "Error initializing AIO or GPIO \n");
        exit(1);
    }
}


void handler (const char* buffer){
    if (strlen(buffer)>4 && buffer[0] == 'L' && buffer[1] == 'O' && buffer[2] == 'G' && buffer[3] == ' '){
        if (log_option)
            fputs(buffer, logfd) ;
    }
    else {
        char string [] = "PERIOD=" ;
        int index=0;
        while (buffer[index] != '\0' && string[index] != '\0'){
            if (buffer[index] != string[index])
                return;
            index ++;
        }
        int length = strlen(buffer) - 1;
        if (index != 7) return;
        while(index < length){
            if ( !isdigit(buffer[index]))
                return;
            index++;
        }
        
        period = atoi(&buffer[7]);
        printString(buffer) ; // added
    }
}


void process_input (const char* buffer){
    if (same(buffer, "STOP")){
        report_log = 0;
        printString("STOP") ;
    }
    else if (same(buffer, "START")){
        report_log = 1;
        printString("START") ;
    }
    else if (same(buffer, "SCALE=F")){
        scale = 'F';
        printString("SCALE=F") ;
    }
    else if (same(buffer, "SCALE=C")){
        scale = 'C';
        printString("SCALE=C") ;
    }
    else if (same(buffer, "OFF")){
        printString("OFF") ;
        shutdown_connection();
    }
    else
        handler(buffer);
}


double get_temp(){
    const int B = 4275;               // B value of the thermistor
    const int R0 = 100000;            // R0 = 100k
    int a = mraa_aio_read (tSens);
    double R = 1023.0/((double)a)-1.0;
    R = R0*R;
    double temperature = 1.0 / ( log(R/R0)/B + 1/298.15) - 273.15; // convert to temperature via datasheet
    if (scale == 'F')
        temperature = temperature * 9/5 + 32 ;
    return temperature;
}

void post_time(){
    gettimeofday(&clock_now, 0);
    if (report_log && clock_now.tv_sec >= next){
        time_now = localtime(&clock_now.tv_sec);
        next = clock_now.tv_sec + period;
        double temp = get_temp();
        char buf_out[256];
        sprintf(buf_out, "%02d:%02d:%02d %.1f\n", time_now->tm_hour, time_now->tm_min, time_now->tm_sec, temp);
//        fputs (buf_out, stdout);  //remove later
        SSL_write(ssl, buf_out, strlen(buf_out));
        if (log_option)
            fputs(buf_out, logfd) ;
    }
}

int main(int argc, char *argv[ ])
{
    // *** Parsing Options ***
    static struct option long_options[] =
    {
        {"period", 1, 0, 'p' },
        {"scale", 1, 0, 's' },
        {"log", 1, 0, 'l' },
        {"host", 1, 0, 'h' },
        {"id", 1, 0, 'i' },
        {0, 0, 0, 0}
    };
    int c = 0 ;  // option parse character
//    int long_index = 0 ; //index of getopt()
    while ((c = getopt_long(argc, argv, "",long_options, NULL )) != -1) {
        switch(c) {
            case 's' :
                if ((optarg[0] != 'C' && optarg[0] != 'F') || strlen(optarg) != 1){
                    fprintf (stderr, "Error: Invalid argument for Scale \n") ;
                    exit (1) ;
                }
                scale = optarg[0] ;
//                printf ("%c \n", scale);
                break ;
                
            case 'l' :
                log_option = 1;
                logfd = fopen(optarg, "w") ;
                if ( logfd == NULL) { //FIX
                    fprintf(stderr,  "Error: could not open file \n") ;
                    exit (1) ;
                }
                break ;
                
            case 'p' :
                period = atoi(optarg) ;
//                printf ("%d \n", period);
                break ;
                
            case 'h' :
                host = optarg;
                break ;
                
            case 'i' :
                id = atoi(optarg);
//                printf("id received : %s\n", optarg);
                break ;
                
            default:
                fprintf (stderr, "Error: Unrecognized option \n") ;
                exit (1) ;
        } // end of switch
    } // end of while

    port = atoi (argv[argc - 1]) ; //last argument
//    printf("Host: %s!\n", host);
//    printf("id: %i!\n", id);
//    printf("port: %i!\n", port);
    
    
    if (logfd == 0){
        fprintf(stderr, "Must provide the name for the log file\n") ;
        exit(1) ;
    }
    if (strlen(host) == 0){
        fprintf(stderr, "Must provide the name for the host\n") ;
        exit(1) ;
    }
    char id_buf [50] ;
    sprintf(id_buf, "%d", id);
    if (strlen(id_buf) != 9){
        fprintf(stderr, "Must provide 9-digit ID number\n") ;
        exit(1) ;
    }
    if (port <= 0){
        fprintf(stderr, "Must provide a valid port number\n") ;
        exit(1) ;
    }
    
    
    // create connection
    sockfd = socket(AF_INET, SOCK_STREAM, 0) ;
    if (sockfd < 0){
        fprintf(stderr, "Error while creating socket\n") ;
        exit(1) ;
    }
    
    memset( (void * ) &server_sockaddr, 0, sizeof(server_sockaddr)) ;
    
    server_hostent = gethostbyname(host) ;
    if (server_hostent == NULL){
        fprintf(stderr, "Error while finding host\n") ;
        exit(1) ;
    }
    
    server_sockaddr.sin_family = AF_INET ;
    server_sockaddr.sin_port = htons(port) ;
    memcpy( (char *) &server_sockaddr.sin_addr.s_addr,(char *) server_hostent->h_addr, server_hostent->h_length);
    
    if ( connect (sockfd, (struct sockaddr *) &server_sockaddr, sizeof( server_sockaddr))  < 0){
        fprintf(stderr, "Error in initiating a connect on a socket \n") ;
        perror("Error ") ;
        exit (1) ;
    }
//    printf("socket connection done \n");
    
    SSL_library_init() ;
    OpenSSL_add_all_algorithms() ;
    SSL_load_error_strings() ;
    printf("loading for SSL \n");
    
    ctx = SSL_CTX_new( TLSv1_client_method());
    if (ctx == NULL){
        fprintf(stderr, "Error: cannot get context\n") ;
        exit(2) ;
    }
    
    ssl = SSL_new (ctx) ;
    if (ssl == NULL){
        fprintf(stderr, "Error: cannot setup SSL\n") ;
        exit(2) ;
    }
    
    if (SSL_set_fd(ssl, sockfd) != 1){
        fprintf(stderr, "Error: cannot set sock fd with SSL\n") ;
        exit(2) ;
    }
    
    if (SSL_connect(ssl) != 1){
        fprintf(stderr, "Error: SSL connection failed\n") ;
        exit(2) ;
    }
    
//    printf("Connected\n");
    //print ID
    char buf_out[20];
    sprintf(buf_out, "ID=%d\n", id);
    fputs(buf_out, logfd) ;
    SSL_write(ssl, buf_out, strlen(buf_out));
    
    struct pollfd poll_array [1] =
    {
        { sockfd, POLLIN | POLLHUP | POLLERR , 0},
    };

    initialize ();
  
    while(1){
        post_time();
        if (poll(poll_array, 1, 0) < 0){
            fprintf(stderr, "error: while polling");
            exit(1);
        }
        if (poll_array[0].revents & POLLIN){
            char buffer [256];
            bzero(buffer, 256) ;
            SSL_read(ssl, buffer, 256);
            process_input(buffer);
        }
        
//        if (mraa_gpio_read(button))
//            shutdown_connection();
    }
}
    
