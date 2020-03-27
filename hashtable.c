#include "hashtable.h"


int hash_function(char* key, int table_size){
    int i=0, hash=0;

    for (i=0; i<strlen(key); i++)
        hash+=key[i]*i;
    return hash%table_size;
}


void insert_in_table(bucket** table, char* invalue, int table_size, int bucket_size, list_node* rec){
    int hash;
    bucket_record* b;

    hash=hash_function(invalue, table_size);
    if(table[hash]==NULL){   //an den exoume bucket se afto to "hash pointer"
        new_bucket(&(table[hash]), bucket_size);
        newrec_in_bucket(table[hash], invalue, rec);
        return;
    }
    b=search_bucket(table[hash], invalue);
    if(b==NULL){        //find where to insert new bucket record
        while(table[hash]->next!=NULL)  //phgaine sto teleftaio bucket
            table[hash]=table[hash]->next;
        if(table[hash]->occupied_records==table[hash]->fittable_records){   //exei gemisei to teleftaio bucket
            new_bucket(&(table[hash]), bucket_size);
            newrec_in_bucket(table[hash]->next, invalue, rec);
        }else{      //xwraei sto teleftaio bucket
            newrec_in_bucket(table[hash], invalue, rec);
        }
    }else{          //just insert in the tree
        insert_in_recordbuck(b, rec);
    }
//    printf("%d occupied and %d fittable\n", table[hash]->occupied_records, table[hash]->fittable_records);
    return;
}


void new_bucket(bucket** b, int bucket_size){
    bucket* newb;
    newb=(bucket*)malloc(sizeof(bucket));
    newb->fittable_records=(bucket_size-sizeof(bucket*)-2*sizeof(int))/sizeof(bucket_record);
    newb->records=malloc((newb->fittable_records)*sizeof(bucket_record));
    newb->occupied_records=0;
    newb->next=NULL;
    if (*b==NULL){
        *b=newb;
    }else{
        (*b)->next=newb;
    }
    return;
}

void newrec_in_bucket(bucket *b, char* invalue, list_node* rec){

    strcpy(b->records[b->occupied_records].value, invalue);
    b->records[b->occupied_records].t=NULL;
    insertNode(&(b->records[b->occupied_records].t), rec->entryDate, rec);
    (b->occupied_records)++;
    return;
}

void insert_in_recordbuck(bucket_record *brec, list_node* rec){
    insertNode(&(brec->t), rec->entryDate, rec);
    return;
}

bucket_record* search_bucket(bucket* b, char* key){
    int i;
    if (b==NULL)
        return NULL;
    do {
        for (i=0; i<b->occupied_records; i++){
            if (strcmp(b->records[i].value, key)==0)
                return &b->records[i];
        }
        b=b->next;
    }while (b!=NULL);

    return NULL;
}

bucket_record* search_table(bucket** t, char* key, int table_size){
    int i,j;
    for (i=0; i<table_size; i++){
        while(t[i]!=NULL){
            for (j=0; j<t[i]->occupied_records; j++){
                if (strcmp(t[i]->records[j].value, key)==0)
                    return &t[i]->records[j];
            }
            t[i]=t[i]->next;
        }
    }
    return NULL;
}

void table_sick(bucket** t, int table_size){
    int i,j,count;
    bucket* temp;
    for (i=0; i<table_size; i++){
        temp=t[i];
        while(temp!=NULL){
            for (j=0; j<temp->occupied_records; j++){
                count=0;
                countSick(temp->records[j].t, &count);
                printf("%s %d\n", temp->records[j].value, count);
            }
            temp=temp->next;
        }
    }
}

void table_patients(bucket** t, int table_size){
    int i,j,count;
    bucket* temp;
    for (i=0; i<table_size; i++){
        temp=t[i];
        while(temp!=NULL){
            for (j=0; j<temp->occupied_records; j++){
                count=0;
                countPatients(temp->records[j].t, &count);
                printf("%s %d\n",temp->records[j].value, count);
            }
            temp=temp->next;
        }
    }
}

void table_patients_ranged(bucket** t, int table_size, date in, date out){
    int i,j,count;
    bucket* temp;
    for (i=0; i<table_size; i++){
        temp=t[i];
        while(temp!=NULL){
            for (j=0; j<temp->occupied_records; j++){
                count=0;
                countPatientsRanged(temp->records[j].t, &count, in, out);
                printf("%s %d\n", temp->records[j].value, count);
            }
            temp=temp->next;
        }
    }    
}

void table_patients_key_ranged(bucket** t, int table_size, date in, date out, char* key){
    int count=0;
    bucket_record* temp;
    temp=search_table(t, key, table_size);
    countPatientsRanged(temp->t, &count, in, out);
    printf("%s %d\n", temp->value, count);
}

void table_patients_doublekey_ranged(bucket** t, int table_size, date in, date out, char* disease, char* country){
    int count=0;
    bucket_record* temp;
    temp=search_table(t, disease, table_size);
    countPatientsRangedCountry(temp->t, &count, in, out, country);
    printf("%s %d\n", temp->value, count);

}

void clear_hashtable(bucket** table, int table_size){
    int i,j;
    for (i=0; i<table_size; i++){
        while (table[i]!=NULL){
            for (j=0; j<(table[i]->occupied_records); j++)
                destroy(table[i]->records[j].t);
            table[i]=table[i]->next;
        }    
    }
    free(table);
}


