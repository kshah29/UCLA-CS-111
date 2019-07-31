// NAME: Kanisha Shah
// EMAIL: kanishapshah@gmail.com
// ID: 504958165

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>



long num_threads = 1; // store total threads
long iterations = 1; // store total iterations
char sync_opt = '\0'; // store sync option passsed
int yield = 0; // checks if option yield is passed
long long counter = 0;
pthread_mutex_t mutex_lock; // mutex lock variable
int test_and_set_lock = 0;
int compare_and_swap_lock = 0;
struct timespec start_time, end_time; //store start and end times
char * yield_name ;


void add(long long *pointer, long long value) {
    long long sum = *pointer + value;
    if (yield)
        sched_yield();
    *pointer = sum;
}

void add_mutex(long long *pointer, long long value) {
    pthread_mutex_lock(&mutex_lock);
    long long sum = *pointer + value;
    if (yield)
        sched_yield();
    *pointer = sum;
    pthread_mutex_unlock(&mutex_lock);
}

void add_test_and_set(long long *pointer, long long value) {
    while( __sync_lock_test_and_set (&test_and_set_lock, 1) == 1) ; // spin
    long long sum = *pointer + value;
    if (yield)
        sched_yield();
    *pointer = sum;
    __sync_lock_release (&test_and_set_lock);
}

void add_compare_and_sawp(long long *pointer, long long value) {
    while (__sync_val_compare_and_swap (&compare_and_swap_lock, 0, 1) == 1) ;//spin
    long long sum = *pointer + value;
    if (yield)
        sched_yield();
    *pointer = sum;
    __sync_val_compare_and_swap (&compare_and_swap_lock, 1, 0) ;
}

void * thread_add_function (){
    int iter_index;
    for (iter_index=0; iter_index < iterations; iter_index++)
    {
        switch (sync_opt) {
            case '\0':  //NO lock
                add(&counter, 1);
                add(&counter, -1);
                break;
                
            case 'm':    // Mutex lock
                add_mutex(&counter, 1);
                add_mutex(&counter, -1);
                break;
                
            case 'c':    // test and set lock
                add_compare_and_sawp(&counter, 1);
                add_compare_and_sawp(&counter, -1);
                break;
                
            case 's':    // compare and swap lock
                add_test_and_set(&counter, 1);
                add_test_and_set(&counter, -1);
                break;
                
            default:
                break;
        } // end switch-case
    } // end for
    return NULL;
} // end fn


int main(int argc, char *argv[ ])
{
    // *** Parsing Options ***
    static struct option long_options[] =
    {
        {"threads", 1, 0, 't' },
        {"iterations", 1, 0, 'i' },
        {"sync", 1, 0, 's' },
        {"yield", 0, 0, 'y' },
        {0, 0, 0, 0}
    };
    char c = 0 ;  // option parse character
    int long_index = 0 ; //index of getopt()
    while ((c = getopt_long(argc, argv, "s",long_options, &long_index )) != -1) {
        switch(c) {
            case 't' :
                num_threads = atoi(optarg) ;
//                printf("%ld\n", num_threads) ;
                break ;
                
            case 'i' :
                iterations = atoi(optarg) ;
//                 printf("%ld\n", iterations) ;
                break ;
                
            case 's' :
                sync_opt = (char )optarg[0] ;
//                 printf("%c %d\n", sync_opt, (int) sizeof(sync_opt)) ;
                if ( sync_opt == 'm' || sync_opt == 's' || sync_opt == 'c') ;
                else {
                    fprintf (stderr, "Error: Not valid argument for sync \n") ;
                    exit(1) ;
                }
                break ;
                
            case 'y' :
                yield = 1 ;
                yield_name =  realloc (yield_name, 7) ;
                yield_name = "-yield" ;
//                printf("%s\n", yield_name) ;
                break ;

            default:
                fprintf (stderr, "Error: Unrecognized option \n") ;
                exit (1) ;
        } // end of switch
    } // end of while
    

    if (yield == 0)
        yield_name = "" ;

    clock_gettime(CLOCK_MONOTONIC, &start_time) ;
    
    if (sync_opt == 'm')
        mutex_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    
    pthread_t tid [num_threads] ;
    //create threads
    int i = 0, rc;
    for (i=0; i<num_threads; i++) {
        rc = pthread_create(&tid[i], NULL, (void *) &thread_add_function, NULL) ;
        if (rc) {
            fprintf(stderr, "Error while creating pthread_create \n") ;
            exit (1);
        }
    }
    
    //join threads
    for (i=0; i<num_threads; i++) {
        rc = pthread_join(tid[i], NULL) ;
        if (rc != 0) {
            fprintf(stderr, "Error while creating pthread_join \n") ;
            exit (1);
        }
    }
    
    if (sync_opt == 'm')
        pthread_mutex_destroy(&mutex_lock);

    clock_gettime(CLOCK_MONOTONIC, &end_time) ;
    

    long long runtime = ((end_time.tv_sec - start_time.tv_sec) * 1000000000) + end_time.tv_nsec - start_time.tv_nsec ;
    long operations = num_threads * iterations * 2;
    long avg_runtime = runtime/operations;
    
    if (sync_opt == '\0')
        printf("add%s-none,%ld,%ld,%ld,%lld,%ld,%lld\n", yield_name, num_threads, iterations, operations, runtime, avg_runtime, counter);
    else
        printf("add%s-%c,%ld,%ld,%ld,%lld,%ld,%lld\n", yield_name, sync_opt, num_threads, iterations, operations, runtime, avg_runtime, counter);
    
    exit(0) ; // normal execution
}
