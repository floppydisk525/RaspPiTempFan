//Build and run this program with something like:
//gcc -Wall arraytest.c -o arraytest
//sudo ./arraytest

#include <stdio.h>
#include <string.h>

//void printarray( char **array, int SIZE )
void printarray( char array[][50], int SIZE ){
    int i;
    int j;

    for( j = 0; j < SIZE; j++ ){
        for( i = 0; i < SIZE; i ++){
            printf( "%c ", array[j][i] );
        }
        printf( "\n" );
    }
}

void fillarray ( char ** array, int SIZE){
	
	
	
}

int getTempSensCnt () {
	int cnt = 3;
	return cnt;
}

int main( void ){
//    char array[][50];
//    int SIZE;
	
	//int tempSensCnt;
	
	int tempSensCnt = getTempSensCnt();
	printf ("The count is: %d\n", tempSensCnt);

//    ...call function to fill array... this part works.
	//fillarray( array, SIZE);
    //printarray( array, SIZE );
	
}

