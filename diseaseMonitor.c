#include "hashtable.h"
#include "bf.h"


int main(int argc, char* argv[]){

    int i,diseaseHashtableNumOfEntries,countryHashtableNumOfEntries,bucketSize, recCounter=0, sickCounter;
    char patientRecordFile[256];
    char recordID[32];
    char patientFirstName[32];
    char patientLastName[32];
    char diseaseID[32] ;
    char country[32];
    char entryDate[16]; 
    char exitDate[16];
    date inDate, outDate;
    char command[64];
    char* arg[8];
    char* token;
    char string[512];

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;


    bloom bf;

    list_node* head=NULL;
//    list_node* cur;         
    bucket** diseaseHashTable;
    bucket** countryHashTable;

    bucket_record* temp;


    FILE* fp;

    diseaseHashtableNumOfEntries=10;       //default times an den dothoun orismata
    countryHashtableNumOfEntries=10;
    bucketSize=124;


    for (i=1; i<argc ; i+=2)
    {
        if (strcmp(argv[i],"-p")==0){
            strcpy(patientRecordFile, argv[i+1]);
        }else if (strcmp(argv[i],"-h1")==0){
            diseaseHashtableNumOfEntries=atoi(argv[i+1]);
        }else if (strcmp(argv[i],"-h2")==0){
            countryHashtableNumOfEntries=atoi(argv[i+1]);
        }else if (strcmp(argv[i],"-b")==0){
            bucketSize=atoi(argv[i+1]);
        }else{
            printf("%s-->%s\n", argv[i], argv[i+1]);
            printf("Lathos orismata\n");
            return 1;
        }
    }
    if (strlen(patientRecordFile)==0){
        printf("Den yparxei arxeio asthenwn gia diavasma\n");
        return 1;
    }

    diseaseHashTable=malloc(diseaseHashtableNumOfEntries*sizeof(bucket*));
    for (i=0; i<diseaseHashtableNumOfEntries; i++)
        diseaseHashTable[i]=NULL;

    countryHashTable=malloc(countryHashtableNumOfEntries*sizeof(bucket*));
    for (i=0; i<countryHashtableNumOfEntries; i++)
        countryHashTable[i]=NULL;



    fp = fopen(patientRecordFile, "r");
    while(nread=getline(&line, &len, fp)!=-1)   //read whole file to get #of records
        recCounter++;

    set_bitstringsize(recCounter, &bf);	//dhmioyrgia bloom filter
	init_bf(&bf);
 
    fseek(fp, 0, SEEK_SET);     //reset pointer to start of the file

    for (i=0; i<recCounter; i++){
        fscanf(fp,"%s %s %s %s %s %s %s",recordID , patientFirstName, patientLastName, diseaseID, country, entryDate, exitDate);
        string_to_date(entryDate, &inDate);     //metatroph string date se struct date
        string_to_date(exitDate, &outDate);

        if (date_older(inDate, outDate)==1){
            if (i==0){
                list_insert(&head, recordID, country, diseaseID, patientFirstName, patientLastName, inDate, outDate);
                insert_key(recordID, &bf);
                insert_in_table(diseaseHashTable, diseaseID, diseaseHashtableNumOfEntries, bucketSize, head);
                insert_in_table(countryHashTable, country,  countryHashtableNumOfEntries, bucketSize, head);
            }else{
                if (check_bf(recordID, bf)==0){      //den yparxei sth lista
                    list_insert(&head, recordID, country, diseaseID, patientFirstName, patientLastName, inDate, outDate);
                    insert_key(recordID, &bf);
                    insert_in_table(diseaseHashTable, diseaseID, diseaseHashtableNumOfEntries, bucketSize, head);
                    insert_in_table(countryHashTable, country,  countryHashtableNumOfEntries, bucketSize, head);
                }else{          //mallon yparxei sth lista
                    if (check_list(head, recordID)==0){  //den yparxei telika sth lista
                        list_insert(&head, recordID, country, diseaseID, patientFirstName, patientLastName, inDate, outDate);
                        insert_key(recordID, &bf);
                        insert_in_table(diseaseHashTable, diseaseID, diseaseHashtableNumOfEntries, bucketSize, head);
                        insert_in_table(countryHashTable, country,  countryHashtableNumOfEntries, bucketSize, head);
                    }else{          //yparxei telika sth lista, svhsta ola
                        printf("###Record with ID:%s already exists.\n", recordID);
                        fclose(fp);
                        clear_hashtable(countryHashTable, countryHashtableNumOfEntries);
                        clear_hashtable(diseaseHashTable, diseaseHashtableNumOfEntries);
                        clear_bf(bf);
                        delete_list(&head);                    
                        return 0;
                    }
                }
            }
        }else{
            printf("###Record with ID:%s is rejected cause of date error.\n", recordID);
        }
    }
    fclose(fp);
    for (i=0; i<8; i++)
        arg[i]=malloc(64*sizeof(char));
    do{
        for (i=0; i<8; i++)
            strcpy(arg[i], "*");
        fflush(stdin);
        scanf(" %[^\n]", string);
        token=strtok(string," ");
        strcpy(command, token);
        for(i=0; i<8 && token!=NULL; i++){
             strcpy(arg[i], token);
             token=strtok(NULL, " ");
        }

        if (strcmp(command, "/globalDiseaseStats")==0){
            if(strcmp(arg[1],"*")==0){          //xwris date1 date2
                table_patients(diseaseHashTable, diseaseHashtableNumOfEntries);
            }else if(strcmp(arg[2],"*")==0){
                printf("$$You must give an exit date.\n");
            }else{          //me date1 date2
                string_to_date(arg[1], &inDate);
                string_to_date(arg[2], &outDate);
                table_patients_ranged(diseaseHashTable, diseaseHashtableNumOfEntries, inDate, outDate);
            }

        }else if (strcmp(command, "/diseaseFrequency")==0){
            string_to_date(arg[2], &inDate);
            string_to_date(arg[3], &outDate);
            if(strcmp(arg[4], "*")==0)
                table_patients_key_ranged(diseaseHashTable, diseaseHashtableNumOfEntries, inDate, outDate, arg[1]);
            else
                table_patients_doublekey_ranged(diseaseHashTable, diseaseHashtableNumOfEntries, inDate, outDate, arg[1], arg[4]);
        }else if (strcmp(command, "/topk-Diseases")==0){

        }else if (strcmp(command, "/topk-Countries")==0){

        }else if (strcmp(command, "/insertPatientRecord")==0){
            string_to_date(arg[6], &inDate);
            if(strcmp(arg[7],"*")==0)
                string_to_date("-", &outDate);
            else
                string_to_date(arg[7], &outDate);
            if (date_older(inDate, outDate)==2)
                printf("$$Exit date must be after entry date.\n");
            else{
                if (check_bf(arg[1], bf)==0){
                    list_insert(&head, arg[1], arg[5], arg[4], arg[2], arg[3], inDate, outDate);
                    insert_key(arg[1], &bf);
                    insert_in_table(diseaseHashTable, arg[4], diseaseHashtableNumOfEntries, bucketSize, head);
                    insert_in_table(countryHashTable, arg[5],  countryHashtableNumOfEntries, bucketSize, head);
                    printf("Record added\n");
                }else if (check_list(head, arg[1])==0){
                    list_insert(&head, arg[1], arg[5], arg[4], arg[2], arg[3], inDate, outDate);
                    insert_key(arg[1], &bf);
                    insert_in_table(diseaseHashTable, arg[4], diseaseHashtableNumOfEntries, bucketSize, head);
                    insert_in_table(countryHashTable, arg[5],  countryHashtableNumOfEntries, bucketSize, head);    
                    printf("Record added\n");
                }else{
                    printf("error\n");
                }
            }
        }else if (strcmp(command, "/recordPatientExit")==0){
            string_to_date(arg[3], &outDate);
            if (set_exitdate(head, arg[2], outDate)==0)
                printf("Not found\n");
        }else if (strcmp(command, "/numCurrentPatients")==0){
                if (strcmp(arg[1], "*")!=0){
                    temp=search_table(diseaseHashTable, arg[1], diseaseHashtableNumOfEntries);
                    if(temp!=NULL){
                        sickCounter=0;
                        countSick(temp->t, &sickCounter);
                        printf("%s %d\n", arg[1], sickCounter);
                    }else{
                        printf("%s 0\n", arg[1]);
                     //   printf("error\n");
                    }
                }else{
                    table_sick(diseaseHashTable, diseaseHashtableNumOfEntries);
                }
        }else if (strcmp(command, "/exit")==0){
            printf("exiting\n");
        }else{
            printf("\nWrong command, please try again.\n");
        }

    }while (strcmp(command, "/exit")!=0);

    clear_hashtable(countryHashTable, countryHashtableNumOfEntries);
    clear_hashtable(diseaseHashTable, diseaseHashtableNumOfEntries);
    clear_bf(bf);
    delete_list(&head);

    return 0;
}