//Purpose of this program is to try malloc for 2d char array AND
//  have clang from microsoft compile it successfully.

//Build and run this program with something like:
//gcc -Wall arraymalloc.c -o arraymalloc
//sudo ./arraymalloc

//good book:  https://www.dipmat.univpm.it/~demeio/public/the_c_programming_language_2.pdf

// good example here:  https://www.quora.com/How-can-I-declare-a-2-dimensional-array-of-char-in-C

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *path = "/sys/bus/w1/devices";    //char pointer to path (that doesn't change)

#define devWIDTH 16

char **make_2d_array(size_t width, size_t height) {
	// First we allocate memory for the outer
	// 'row holder' array 
	char **outer_arr = malloc(height * sizeof(char*));
 
	// Then, in each row, we allocate the row
	// at the desired width
	for (int i = 0; i < height; i++) {
		outer_arr[i] = malloc(width * sizeof(char));
	} 
 
	// MORE STUFF CAN HAPPEN HERE
 	return outer_arr;
} 


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

void fillarray (char **prtarray, int height){
	int i;

	for( i = 0; i < height; i ++){
            sprintf(prtarray[i], "POS%d",i);
        }

	printf("Printed from fillarry\n");
	for (i = 0; i<height; i++){
		printf("dev variable is: %s\n", prtarray[i]);
	}
	printf("\n");
}


int getTempSensCnt () {
	int cnt = 5;
	
	printf ("The string is: %s\n", path);
	return cnt;	
}

int main( void ){
	int i;
	// how many temp sensors are there?  
	int tempSensCnt = getTempSensCnt();
	printf ("The number of sensors is: %d\n", tempSensCnt);
	printf("\n");
	
	//Make 2d array for sensor numbers
	char **dev = make_2d_array(devWIDTH, tempSensCnt);
	
//    ...call function to fill array... this part works.
	fillarray(dev, tempSensCnt);
    //printarray( array, SIZE );
	printf("Printed from main\n");
	for (i = 0; i<tempSensCnt; i++){
		printf("Dev variable is: %s\n", dev[i]);
	}
}

