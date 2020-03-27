#include "list.h"

void list_insert (list_node** head, char* id, char* cou, char* dis, char* first, char* last, date in, date out){
    list_node* new_node=malloc(sizeof(list_node));


    strcpy(new_node->country,cou);
    strcpy(new_node->diseaseID,dis);
    strcpy(new_node->patientFirstName,first);
    strcpy(new_node->patientLastName,last);
    strcpy(new_node->recordID,id);
    new_node->entryDate.day=in.day;
    new_node->entryDate.month=in.month;
    new_node->entryDate.year=in.year;
    new_node->exitDate.day=out.day;
    new_node->exitDate.month=out.month;
    new_node->exitDate.year=out.year;
    new_node->entryDate.day=in.day;
    new_node->next=(*head);
    
    (*head)=new_node;
}

void delete_list(list_node** head){
    list_node* cur=*head;
    list_node* next;

    while(cur!=NULL){
        next=cur->next;
        free(cur);
        cur=next;
    }

    *head=NULL;
}

int check_list(list_node* head, char* key){
    while(head!=NULL){
        if (strcmp(head->recordID, key)==0)
            return 1;
        head=head->next;
    }
    return 0;
}

int set_exitdate(list_node* head, char* key, date exit){
    while(head!=NULL){
        if (strcmp(head->recordID, key)==0){
            if (date_older(head->entryDate, exit)==2){
                printf("$$Exit date must be after entry date.\n");
                return 1;
            }
            head->exitDate.day=exit.day;
            head->exitDate.month=exit.month;
            head->exitDate.year=exit.year;
            printf("Exit date has just been changed\n");
            return 1;
        }
        head=head->next;
    }
    return 0;
}

int string_to_date(char* string, date* returnDate){
    char* token = strtok(string, "-");
    
    if (token==NULL){
        returnDate->day=0;
        returnDate->month=0;
        returnDate->year=0;
    }else{
        returnDate->day=atoi(token);
        token = strtok(NULL, "-");
        returnDate->month=atoi(token);
        token = strtok(NULL, "-");
        returnDate->year=atoi(token);
    }
//	printf("###%2d/%2d/%4d\n", returnDate->day, returnDate->month, returnDate->year);
    return 0;
}

int date_older(date date1, date date2){       //epistrefei to pio palio date
    if (date2.year==0)
        return 1;
    if (date1.year<date2.year)
        return 1;
    else if (date1.year>date2.year)
        return 2;
    else{
        if (date1.month<date2.month)
            return 1;
        else if (date1.month>date2.month)
            return 2;
        else{
            if (date1.day<date2.day)
                return 1;
            else if (date1.day>date2.day)
                return 2;
            else
                    return 0;
            
        }
    }
}
