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

int main( void ){
    char array[][50];
    int SIZE;

//    ...call function to fill array... this part works.
	fillarray( array, SIZE);
    printarray( array, SIZE );
}

