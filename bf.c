#include "bf.h"

void set_bitstringsize (int inserts, bloom *bf){		//ypologismos bit_string size analoga me ton arithmo eisagwgwn
	int flag, bit_string, i;

        bit_string= inserts* 5;
        do{				//find first prime afther bit_string
                bit_string++;
                flag=0;
                for (i=2; i<bit_string/2 && flag==0 ; i++) {
                        if (bit_string % i ==0) flag=1;
                }
        }while (flag==1);

	bf->bit_size=bit_string;

}

int get_bitstringsize (bloom bf){
	return bf.bit_size;
}

void init_bf(bloom *bf){			//dhmioyrgia bf; pinakas char megethous kata 1 megalytero tou bit_string size

	int i;
	bf->array_size= (get_bitstringsize(*bf) / 8) + 1;
	bf->array=malloc(bf->array_size * (sizeof(char)));
	if (bf->array==NULL)
		printf ("Error allocating memory\n");

	for (i=0;i<(bf->array_size);i++)
		bf->array[i]=0;
}

void clear_bf(bloom bf){		//eleftherwsh mnhmhs
	free(bf.array);
}

long long int hash1(char * key){		//1h hash function

	int i, size;
	long long int hash1=0;
	size=strlen(key);
	for (i=0;i<size;i++){
		hash1=(hash1<<8)+key[i];
	}
	return hash1;
}

long long int hash2(char * key){		//2h hash function

	int i, size;
	long long int hash2=0;
	size=strlen(key)-1;
	for (i=size;i>=0;i--){
		hash2=(hash2<<8)+key[i];
	}
	return hash2;
}

long long int hash3(long long int h1, long long int h2){	//3h hash function
	return (h1+h2);
}

void insert_key (char * s, bloom *bf){		//allagh (an den einai hdh allagmena) 3 bit tou bf

	long long int key1, key2, key3;
	int mask,pos;

	key1=hash1(s)%get_bitstringsize(*bf);
	key2=hash2(s)%get_bitstringsize(*bf);
	key3=hash3(key1, key2)%get_bitstringsize(*bf);

	mask=1;
	pos=key1/8;

	if (key1%8==0){
		pos--;
		mask=mask<<7;
	}else{
		key1=key1%8;
		mask=mask<<(key1-1);
	}

	bf->array[pos] = bf->array[pos] | mask;

	mask=1;
	pos=key2/8;

	if (key2%8==0){
		pos--;
		mask=mask<<7;
	}else{
		key2=key2%8;
		mask=mask<<(key2-1);
	}
	bf->array[pos] = bf->array[pos] | mask;

	mask=1;
	pos=key3/8;

	if (key3%8==0){
		pos--;
		mask=mask<<7;
	}else{
		key3=key3%8;
		mask=mask<<(key3-1);
	}
	bf->array[pos] = bf->array[pos] | mask;




}

int check_bf( char * x, bloom bf){	//elegxos gia pithanh yparxh toy key sto bf
	long long int key1, key2, key3;
	int mask,pos;

	key1=hash1(x)%get_bitstringsize(bf);
	key2=hash2(x)%get_bitstringsize(bf);
	key3=hash3(key1,key2)%get_bitstringsize(bf);


	mask=1;
	pos=key1/8;

	if (key1%8==0){
		pos--;
		mask=mask<<7;
	}else{
		key1=key1%8;
		mask=mask<<(key1-1);
	}

	if ((bf.array[pos] & mask) ==0)
		return 0;



	mask=1;
	pos=key2/8;

	if (key2%8==0){
		pos--;
		mask=mask<<7;
	}else{
		key2=key2%8;
		mask=mask<<(key2-1);
	}

	if ((bf.array[pos] & mask) ==0)
		return 0;


	mask=1;
	pos=key3/8;

	if (key3%8==0){
		pos--;
		mask=mask<<7;
	}else{
		key3=key3%8;
		mask=mask<<(key3-1);
	}

	if ((bf.array[pos] & mask) ==0)
		return 0;

	return 1;
}

// void set_bf( node tree, bloom * bf){	//gemisma tou bf me keys apo to dentro
//         if (tree==NULL)
//                 return;
//         set_bf(tree->left, bf);
//         set_bf(tree->right, bf);
//         insert_key(tree->value, bf);
// }



void zero_bf(bloom *bf){		//mhdenismos tou bf gia pithanh epanadhmioyrgia tou
	int i;
	for (i=0;i<bf->array_size;i++)
		bf->array[i]=0;
}
