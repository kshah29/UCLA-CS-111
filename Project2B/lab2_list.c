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
pthread_mutex_t * mutex_lock; // mutex lock variable
int * test_and_set_lock;
struct timespec start_time, end_time; //store start and end times
char * yield_name ;
int opt_yield = 0;
int num_lists = 1;
SortedList_t * list;
SortedListElement_t * elements;
int * element_list_num; // the list element should go to
long * lock_time; // lock time for each thread recorded here


void segfault_handler()
{
    fprintf(stderr, "Error: Segmentation Fault \n") ;
    exit(2) ;
}

//initializes elements, list, element_list_num, and thread lock
void initialize (){
    //Lists
    list = malloc (num_lists *sizeof (SortedList_t));
    int i=0;
    for (; i < num_lists; i++){
        list[i].prev = NULL;
        list[i].next = NULL;
        list[i].key = NULL ;
    }
    // Elements
    int num_elements = num_threads * iterations;
    elements = malloc(num_elements * sizeof(SortedListElement_t)) ;
    time_t t;
    srand((unsigned) time(&t));
    for (i=0; i < num_elements; i++) {
        char* temp_key = malloc (3*sizeof(char)) ;
        temp_key[0] = (rand() % 26) + 65;
        temp_key[1] = (rand() % 26) + 65;
        temp_key[2] = '\0';
        elements[i].key = temp_key;
    }
    // elements_list_num
    element_list_num = malloc (num_elements * sizeof(int));
    for (i=0; i < num_elements; i++){
        int hash = ((int) elements[i].key[0]) % num_lists ;
        element_list_num[i] = hash;
        if (hash < 0) element_list_num[i] = hash * (-1) ;
//        printf("%d  %d    %d    ", (int) elements[i].key, hash, element_list_num[i]) ;
    }
//    printf ("\n");
    // lock time for threads
    lock_time = malloc (num_threads * sizeof(long)) ;
    for (i=0; i < num_threads; i++){
        lock_time[i] = 0;
//        printf("i: %d lock: %ld \n", i, lock_time[i]);
    }
}


void * thread_function (void* argument){
    
    int thread_num = *(int *) argument;
    long start = thread_num * iterations;
    struct timespec lock_start_time, lock_end_time; //store start and end times for lock
    long element_index ;
    int list_num;
    long time = 0;
    
//    printf("Thread: %d Start: %ld End: %ld \n", thread_num, start, start + iterations);
    
    // insert all the elemnts
    for (element_index = start; element_index < start + iterations ; element_index++ ){
        // start timer
        list_num = element_list_num[element_index] ;
        clock_gettime(CLOCK_MONOTONIC, &lock_start_time) ;
        
        //  Lock based on sync option
        if (sync_opt == 'm')
            pthread_mutex_lock (&mutex_lock[list_num]) ;
        else if (sync_opt == 's')
            while( __sync_lock_test_and_set (&test_and_set_lock[list_num], 1)) ; // spin
        
        // end timer
        clock_gettime(CLOCK_MONOTONIC, &lock_end_time) ;
        time += ((lock_end_time.tv_sec - lock_start_time.tv_sec) * 1000000000) + lock_end_time.tv_nsec - lock_start_time.tv_nsec ;
        
        //insert
        SortedList_insert (&list[list_num], &elements[element_index]) ;
        
        // unlock
        if (sync_opt == 'm')
            pthread_mutex_unlock(&mutex_lock[list_num]);
        else if (sync_opt == 's')
            __sync_lock_release (&test_and_set_lock[list_num]);
    }
    
    //double check the length
    long total_length = 0;
    for (list_num = 0; list_num < num_lists; list_num++){
        // start timer
        clock_gettime(CLOCK_MONOTONIC, &lock_start_time) ;
        
        //  Lock based on sync option
        if (sync_opt == 'm')
            pthread_mutex_lock (&mutex_lock[list_num]) ;
        else if (sync_opt == 's')
            while( __sync_lock_test_and_set (&test_and_set_lock[list_num], 1)) ; // spin
        
        // end timer
        clock_gettime(CLOCK_MONOTONIC, &lock_end_time) ;
        time += ((lock_end_time.tv_sec - lock_start_time.tv_sec) * 1000000000) + lock_end_time.tv_nsec - lock_start_time.tv_nsec ;
        
        //store length
        total_length += SortedList_length(&list[list_num]) ;
//        printf("List: %d Length: %ld\n", list_num, total_length);
        
        // unlock
        if (sync_opt == 'm')
            pthread_mutex_unlock (&mutex_lock[list_num]);
        else if (sync_opt == 's')
            __sync_lock_release (&test_and_set_lock[list_num]);
    }
    if ( total_length < 0) {
        fprintf(stderr, "Sorted List length is not same as inserted \n");
        exit(2);
    }
    
    // Look for the element based on keys and then delete it
    for (element_index = start; element_index < start + iterations ; element_index++) {
        // start timer
        list_num = element_list_num[element_index] ;
        clock_gettime(CLOCK_MONOTONIC, &lock_start_time) ;
        
        //  Lock based on sync option
        if (sync_opt == 'm')
            pthread_mutex_lock (&mutex_lock[list_num]) ;
        else if (sync_opt == 's')
            while( __sync_lock_test_and_set (&test_and_set_lock[list_num], 1)) ; // spin
        
        // end timer
        clock_gettime(CLOCK_MONOTONIC, &lock_end_time) ;
        time += ((lock_end_time.tv_sec - lock_start_time.tv_sec) * 1000000000) + lock_end_time.tv_nsec - lock_start_time.tv_nsec ;
        
        
        // lookup
        SortedListElement_t * temptr = SortedList_lookup(&list[list_num], elements[element_index].key) ;
        if (temptr == NULL) {
            fprintf(stderr, "Key in the Sorted List not found \n");
            exit(2);
        }
        // delete
        if (SortedList_delete(temptr) == 1){
            fprintf(stderr, "List corrupted - could not delete \n");
            exit(2);
        }
        
        // unlock
        if (sync_opt == 'm')
            pthread_mutex_unlock (&mutex_lock[list_num]);
        else if (sync_opt == 's')
            __sync_lock_release (&test_and_set_lock[list_num]);
    }

//    printf ("Thread: %d Lock time: %ld \n", thread_num, time);
    lock_time[thread_num] = time;
    
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
        {"lists", 1, 0, 'l' },
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
                
            case 'l' :
                num_lists = atoi(optarg) ;
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
    
    // Locking initialization
    if (sync_opt == 'm') {
        mutex_lock = malloc (num_lists * sizeof(pthread_mutex_t)) ;
        int i=0;
        for(; i < num_lists; i++)
            mutex_lock[i] = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    }
    else if (sync_opt == 's'){
        test_and_set_lock = malloc (num_lists * sizeof(int)) ;
        int i=0;
        for(; i < num_lists; i++)
            test_and_set_lock[i] = 0;
    }
    
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
//        printf("joined %d \n", i);
        if (rc != 0) {
            fprintf(stderr, "Error while creating pthread_join \n") ;
            exit (1);
        }
    }
    
    // locking free
    if (sync_opt == 'm') {
        int i=0;
        for(; i < num_lists; i++)
            pthread_mutex_destroy(&mutex_lock[i]);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end_time) ;
    
    
    long long runtime = ((end_time.tv_sec - start_time.tv_sec) * 1000000000) + end_time.tv_nsec - start_time.tv_nsec ;
    long operations = num_threads * iterations * 3;
    long avg_runtime = runtime/operations;
    long total_lock_wait = 0;
    for (i=0; i < num_threads; i++)
        total_lock_wait += lock_time[i] ;
    long avg_lock_wait = total_lock_wait/operations;

    if (sync_opt == '\0')
        printf("list%s-none,%ld,%ld,%d,%ld,%lld,%ld,%ld\n", yield_name, num_threads, iterations, num_lists, operations, runtime, avg_runtime, avg_lock_wait);
    else
        printf("list%s-%c,%ld,%ld,%d,%ld,%lld,%ld,%ld\n", yield_name, sync_opt, num_threads, iterations, num_lists, operations, runtime, avg_runtime, avg_lock_wait);
    
    if (SortedList_length(list) != 0) //list not empty
        exit(2);
    
    exit(0) ; // normal execution
    
}

