/*
 *  hash_table.c
 *  Created by Luca Giovagnoli on 25/04/14.
 */

#include "hash_table.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define range_s 0
#define range_t 1
#define A (sqrt(5)-1)/2

// for keys that are floats
#define hash_float(k,M) (((k-range_s)/(range_t-range_s))*M)

// for keys that are integers
#define hash_modular(k,M) (k%M)
#define hash_moltiplicativo_modulare(k,M) (k*A)%M

#define MAX_LOAD_FACTOR 0.75


typedef struct node{
    const char* key;
	void* data;
	struct node* next;
}NODE;

typedef struct table {
	NODE** array;
	int N;
	int M;
	void (*free_data) (void*);
} HASH_TABLE;

void free_node(NODE* elem, void(*free_data)(void*));
int _hash_string_efficient(const char* str, int M);

NODE* new_node(const char* key,void*data){

    NODE* node = (NODE*)malloc(sizeof(NODE));
    node->key = key;
    node->data = data;
    return node;
}

HASH_TABLE* hash_table_create(int M, void(*free_data)(void*)) {

	HASH_TABLE* table = (HASH_TABLE*)malloc(sizeof(HASH_TABLE));

	table->array = (NODE**)calloc(M,sizeof(NODE*));
	table->M = M;
	table->N = 0;
	table->free_data = free_data;
	return table;
}

void hash_table_resize(HASH_TABLE* table){

    NODE** new_array = (NODE**)calloc(2*table->M,sizeof(NODE*));
    int i=0;

    for(;i<table->M;i++){
        NODE* elem = table->array[i],*next;
        for(;elem!=NULL;elem=next){
            next = elem->next;
            int index = _hash_string_efficient(elem->key,2*table->M);
            elem->next = new_array[index];
            new_array[index] = elem;
        }
    }

    free(table->array);
    table->array = new_array;
    table->M *= 2;
}

float get_load_factor_alpha(HASH_TABLE* table){
	return ((float)table->N)/ ((float)table->M); //load factor alpha
}

void hash_table_insert(HASH_TABLE* table, const char* key, void* data){

    /* resize if load factor alpha > MAX_LOAD_FACTOR */
    if(get_load_factor_alpha(table)>MAX_LOAD_FACTOR) hash_table_resize(table);

    int index = _hash_string_efficient(key,table->M);

    NODE* elem = new_node(key,data);
    elem->next = (table->array)[index];
    (table->array)[index] = elem;
	(table->N)++;
}

void* hash_table_get(HASH_TABLE* table, const char* key){
    NODE* elem =  table->array[_hash_string_efficient(key,table->M)];

    for(;elem!=NULL;elem=elem->next){
        if(strcmp(elem->key,key)==0) return elem->data;
    }
    return NULL;
}

/*
 * Returns 0 if TRUE, -1 if FALSE 
 * */
int hash_table_contains(HASH_TABLE* table,const char* key){
    if(hash_table_get(table, key) != NULL) return TRUE;
    return FALSE;
}

int hash_table_remove(HASH_TABLE* table, const char* key){
    int index = _hash_string_efficient(key,table->M);
    NODE* elem =  table->array[index], *previous;

    //treat case first element of list is NULL
    if(elem==NULL) return FALSE;
    else if(strcmp(elem->key,key)==0) {
        table->array[index] = elem->next;
        free_node(elem, table->free_data);
        return TRUE;
    }

    for(previous=elem,elem=elem->next;elem!=NULL;elem=elem->next){
        if(strcmp(elem->key,key)==0) {
            previous->next = elem->next;
            free_node(elem, table->free_data);
            return TRUE;
        }
        previous=elem;
    }
    return FALSE;
}

/******************/
/* HASH FUNCTIONS */
/******************/

/**
 * This is a simple and efficient hash function for strings 
 * (only two arithmetic operations for each character)
 * According to Cormen: 
 *
 *          "this function has served programmers well for years".
 *          
 * This algorithm could be bad if for example the table size was a multiple 
 * of 127, but for practical applications the effects are negligible.
 * */
int _hash_string_efficient(const char* str, int M){
    int hash=0, a=127;

    for(; *str != '\0'; str++){
        hash = (a*hash + *str)%M;
    }
    return hash;
}

/**
 * The ideal solution would be to use different random values as 
 * coefficients 'a' in the computation. This randomized algorithm is 
 * called Universal Hashing. The following is a pseudo-random version
 * of the Universal Hashing (it is more efficient).
 * */
int _hash_string_pseudo_universal(const char* str, int M){
    int hash, a=31415, b=27183;

    for(hash=0; *str != '\0'; str++){
        hash = (a*hash + *str)%M;
        a=a*b%(M-1); // a and b are used to generate a pseudo-random sequence
    }
    return hash;
}

/** FREE **/
void free_node(NODE* elem, void(*free_data)(void*)){
    if(free_data==NULL) return;
    free_data(elem->data);
    free(elem);
}

void hash_table_free(HASH_TABLE* table){
    int i=0;
    for(;i<table->M;i++){
        NODE* elem = table->array[i], *next;

        while(elem!=NULL){
            next=elem->next;
            free_node(elem,table->free_data);
            elem=next;
        }
    }
    free(table->array);
    free(table);
}


/** PRINT **/
void hash_table_debug(HASH_TABLE* table, void (*print_data)(void*)){
    int i;

    for(i=0;i<table->M;i++){
        printf("%d : ",i);
        NODE* elem = table->array[i];
        for(;elem!=NULL;elem=elem->next){
            print_data(elem->data);
            printf("->");
        }
        printf("NULL\n");
    }
    printf("\n");
}





