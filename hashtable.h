#include <stdio.h>
#include <string.h>

#include "rbt.h"


typedef struct Bucket_Record{
    char value[32];
    node t;
}bucket_record;

typedef struct Bucket{
    bucket_record* records;
    int occupied_records, fittable_records;
    struct Bucket* next;
}bucket;


int hash_function(char* key, int table_size);
void insert_in_table(bucket** table, char* invalue, int table_size, int bucket_size, list_node* rec);
void new_bucket(bucket** b, int bucket_size);
void newrec_in_bucket(bucket *b, char* invalue, list_node* rec);
void insert_in_recordbuck(bucket_record *brec, list_node* rec);
bucket_record* search_bucket(bucket* b, char* key);
void clear_hashtable(bucket** table, int table_size);
bucket_record* search_table(bucket** t, char* key, int table_size);
void table_sick(bucket** t, int table_size);
void table_patients(bucket** t, int table_size);
void table_patients_ranged(bucket** t, int table_size, date in, date out);
void table_patients_key_ranged(bucket** t, int table_size, date in, date out, char* key);
void table_patients_doublekey_ranged(bucket** t, int table_size, date in, date out, char* disease, char* country);