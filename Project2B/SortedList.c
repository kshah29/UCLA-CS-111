// NAME: Kanisha Shah
// EMAIL: kanishapshah@gmail.com
// ID: 504958165

#include "SortedList.h"
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void SortedList_insert(SortedList_t *list, SortedListElement_t *element){
    
    if (list == NULL || element == NULL) // NO element or list
        return ;
    if (list->next == NULL){ //empty list
        if (opt_yield & INSERT_YIELD)
            sched_yield();
        list->next = element;
        element->next = NULL;
        element->prev= list;
    }
    else {
//        printf("insert in b/w\n") ;
        SortedListElement_t * current = list->next ;
        SortedListElement_t * previous = list ;
        
        while (current != NULL && strcmp(current->key, element->key) >= 0 ) {
//            printf("%s  %s  \n",current->key, element->key);
            previous = current;
            current = current->next;
        } //printf ("found \n");
        
        if (opt_yield & INSERT_YIELD)
            sched_yield();
        
        if (current == NULL) {//reached the end
            element->next = NULL ;
            previous->next = element;
            element->prev = previous;
        }
        else{
            previous->next = element;
            element->prev = previous;
            element->next = current;
            current->prev = element;
        }
    }// else end
//    printf("insert done \n") ;
} // end fn


int SortedList_delete( SortedListElement_t *element){
//    printf("delete \n") ;
    if (element == NULL) // NO element
        return 1;
    else{
        if (opt_yield & DELETE_YIELD)
            sched_yield();
        
        //check the prev ptr and link it to the next one
        if (element->prev != NULL) {
            if (element->prev->next != element)
                return 1;
            else
                element->prev->next = element->next ;
        }
        
        //check the next ptr and link it to the prev one
        if (element->next != NULL) {
            if (element->next->prev != element)
                return 1;
            else
                element->next->prev = element->prev ;
        }
    
    }// end else
    return 0 ;
}


SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key){
//    printf("lookup \n") ;
    if (list == NULL || key == NULL)
        return NULL;
    
    SortedListElement_t * current = list->next;
    
    while ((current != NULL) & (strcmp(current->key, key) > 0) ) {
        if (opt_yield & LOOKUP_YIELD)
            sched_yield();
        current = current->next ;
    }
    
    if (strcmp(current->key, key) == 0)
        return current;
    else
        return NULL;
}


int SortedList_length(SortedList_t *list){
//    printf("length \n") ;
    if (list == NULL)
        return -1;
    
    SortedListElement_t * current = list->next;
    int counter = 0;
    
    while (current != NULL){
        if (opt_yield & LOOKUP_YIELD)
            sched_yield();
        counter ++;
        current = current->next;
    }
    return counter;
}
