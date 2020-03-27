#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct blooms{
	char *array;
    int bit_size;
	int array_size;
}bloom;


int get_bitstringsize(bloom );
void set_bitstringsize(int, bloom *);
void init_bf (bloom *);
void clear_bf(bloom );
long long int hash1(char *);
long long int hash2(char *);
long long int hash3(long long int,long long int);
void insert_key (char *, bloom *);
int check_bf(char *, bloom);

//void set_bf(node, bloom *);

void zero_bf(bloom *);

