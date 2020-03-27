#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Date {
    int day, month, year;
}date;

typedef struct List_Node{
    char recordID[32];
    char patientFirstName[32];
    char patientLastName[32];
    char diseaseID[32] ;
    char country[32];
    date entryDate;
    date exitDate;

    struct List_Node* next;
}list_node;

void list_insert (list_node** head, char* id, char* cou, char* dis, char* first, char* last, date in, date out);
void delete_list(list_node** head);
int check_list(list_node* head, char* key);
int set_exitdate(list_node* head, char* key, date exit);

int string_to_date(char* , date*);
int date_older(date, date);