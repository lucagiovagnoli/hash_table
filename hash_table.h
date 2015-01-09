/*
 *  hash_table.h
 *
 *  Hash Table - Separate Chaining
 *
 *  Chains are unordered linked lists so to have very quick insert procedures.
 *  Double keys allowed. 
 *  Checking of double entries is responsibility of the programmer,
 *  function "hash_table_contains" is used for this purpose. 
 *  Keys must be const char*.
 *  Resizing when load factor alpha > 0.75. 
 *  The hash table size is doubled each resizing.
 *
 *  Created by Luca Giovagnoli on 25/04/14.
 */

#define TRUE 1
#define FALSE 0

typedef struct table HASH_TABLE;

HASH_TABLE* hash_table_create(int M, void(*free_data)(void*));
void hash_table_insert(HASH_TABLE* table, const char* key, void* dati);

/*return NULL if element is not found*/
void* hash_table_get(HASH_TABLE* table, const char* key);

/*returns FALSE if element is not found, TRUE otherwise*/
int hash_table_contains(HASH_TABLE* table, const char* key);

/*return FALSE if element is not found, TRUE if found and correctly removed*/
int hash_table_remove(HASH_TABLE* table, const char* key);


/** FREE **/
void hash_table_free(HASH_TABLE* table);

/** PRINT **/
void hash_table_debug(HASH_TABLE* table, void(*print_data)(void*));
