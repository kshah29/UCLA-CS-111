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
#include <signal.h>
#include "SortedList.h"


long num_threads = 1; // store total threads
long iterations = 1; // store total iterations
char sync_opt = '\0'; // store sync option passsed
int yield = 0; // checks if option yield is passed
pthread_mutex_t mutex_lock; // mutex lock variable
int test_and_set_lock = 0;
struct timespec start_time, end_time; //store start and end times
char * yield_name ;
int opt_yield = 0;
SortedList_t * list;
SortedListElement_t * elements;


void segfault_handler()
{
    fprintf(stderr, "Error: Segmentation Fault \n") ;
    exit(2) ;
}

//initializes elements and list
void initialize (){
    list = malloc (sizeof (SortedList_t));
    list->prev = NULL;
    list->next = NULL;
    list->key = NULL ;
    int num_elements = num_threads * iterations;
    elements = malloc(num_elements * sizeof(SortedListElement_t)) ;
    char* temp_key = malloc (3*sizeof(char)) ;
    int i;
    for (i=0; i < num_elements; i++) {
        temp_key[0] = (rand() % 26) + 65;
        temp_key[1] = (rand() % 26) + 65;
        temp_key[2] = '\0';
        elements[i].key = temp_key;
    }
}


void * thread_function (void* argument){
    
    int thread_num = *(int *) argument;
    long start = thread_num * iterations;

    //  Lock based on sync option
    if (sync_opt == 'm')
        pthread_mutex_lock (&mutex_lock) ;
    else if (sync_opt == 's')
        while( __sync_lock_test_and_set (&test_and_set_lock, 1)) ; // spin
//    printf("Thread: %d Lock: %d Start: %ld\n", thread_num, (test_and_set_lock), start);
    long element_index ;

    // insert all the elemnts
    for (element_index = start; element_index < start + iterations ; element_index++ ){
//        printf ("insert %ld \n", element_index) ;
        SortedList_insert(list, &elements[element_index]) ;
//        printf("L: %d \n", SortedList_length(list));
    }

    //double check the length
    long list_length = SortedList_length(list) ;
    if ( list_length != iterations) {
//        printf("list_length %ld \n",list_length );
        fprintf(stderr, "Sorted List length is less than inserted \n");
        exit(2);
    }

    // Look for the element based on keys and then delete it
    for (element_index = start; element_index < start + iterations ; element_index++){
        SortedListElement_t * temptr = SortedList_lookup(list, elements[element_index].key) ;
        if (temptr == NULL) {
            fprintf(stderr, "Key in the Sorted List not found \n");
            exit(2);
        }
        if (SortedList_delete(temptr) == 1){
            fprintf(stderr, "List corrupted - could not delete \n");
            exit(2);
        }
    }
    
    // destroy lock
    if (sync_opt == 'm')
        pthread_mutex_unlock(&mutex_lock);
    else if (sync_opt == 's')
        __sync_lock_release (&test_and_set_lock);
    
    return NULL;
} // end fn


int main(int argc, char *argv[ ])
{
    signal(SIGSEGV, segfault_handler);
    
    // *** Parsing Options ***
    static struct option long_options[] =
    {
        {"threads", 1, 0, 't' },
        {"iterations", 1, 0, 'i' },
        {"sync", 1, 0, 's' },
        {"yield", 1, 0, 'y' },
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
//                printf("%ld\n", iterations) ;
                break ;
                
            case 's' :
                sync_opt = (char )optarg[0] ;
//                printf("%c %d\n", sync_opt, (int) sizeof(sync_opt)) ;
                if ( sync_opt == 'm' || sync_opt == 's') ;
                else {
                    fprintf (stderr, "Error: Not valid argument for sync \n") ;
                    exit(1) ;
                }
                break ;
                
            case 'y' :
                yield = 1 ;
                yield_name =  realloc (yield_name, strlen(optarg) +2) ;
                yield_name[0] = '-';
                int i=0;
                for (; i < (int) strlen(optarg); i++){
                    switch (optarg[i]) {
                        case 'i':
                            opt_yield = opt_yield | INSERT_YIELD ;
                            yield_name[i+1] = 'i' ;
                            break;
                        case 'l':
                            opt_yield = opt_yield | LOOKUP_YIELD ;
                            yield_name[i+1] = 'l';
                            break;
                        case 'd':
                            opt_yield = opt_yield | DELETE_YIELD ;
                            yield_name[i+1] = 'd' ;
                            break;
                    } // end swicth-case
                } //end for
//                printf("%s\n", yield_name) ;
                break ;
                
            default:
                fprintf (stderr, "Error: Unrecognized option \n") ;
                exit (1) ;
        } // end of switch
    } // end of while
    
    
    if (yield == 0){
        yield_name =  realloc (yield_name, 6) ;
        yield_name = "-none" ;
    }
    initialize() ;
    clock_gettime(CLOCK_MONOTONIC, &start_time) ;
    
    if (sync_opt == 'm')
        mutex_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    
    pthread_t tid [num_threads] ;
    int thread_num [num_threads] ;
    //create threads
    int i = 0, rc;
    for (i=0; i<num_threads; i++) {
        thread_num[i] = i;
//        printf("num=%d i=%d\n", thread_num[i], i);
        rc = pthread_create(&tid[i], NULL, (void *) &thread_function, (void *) &thread_num[i]) ;
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
    long operations = num_threads * iterations * 3;
    long avg_runtime = runtime/operations;

    if (sync_opt == '\0')
        printf("list%s-none,%ld,%ld,1,%ld,%lld,%ld\n", yield_name, num_threads, iterations, operations, runtime, avg_runtime);
    else
        printf("list%s-%c,%ld,%ld,1,%ld,%lld,%ld\n", yield_name, sync_opt, num_threads, iterations, operations, runtime, avg_runtime);
    
    if (SortedList_length(list) != 0) //list not empty
        exit(2);
    
    exit(0) ; // normal execution
    
}

