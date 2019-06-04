//Purpose of this program is to try malloc for 2d char array AND
//  have clang from microsoft compile it successfully.

//Build and run this program with something like:
//gcc -Wall arraymalloc.c -o arraymalloc
//sudo ./arraymalloc

//good book:  https://www.dipmat.univpm.it/~demeio/public/the_c_programming_language_2.pdf

// good example here:  https://www.quora.com/How-can-I-declare-a-2-dimensional-array-of-char-in-C

#include <stdio.h>
#include <string.h>

char *path = "/sys/bus/w1/devices";    //char pointer to path (that doesn't change)
//char **dev;
//char *devPath[][128];

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

/*void fillarray (char *devarray){
	*devarray[0]="dev 0, 16";
	*devarray[1]="dev 1, 16";
	*devarray[2]="dev 2, 16";
	int i;
	printf("\n");
	printf("Printed from fillarry\n");
	for (i = 0; i<3; i++){
		printf("dev variable is: %s\n", *devarray[i]);
	}
	printf("\n");
}
*/

int getTempSensCnt () {
	int cnt = 3;
	
	printf ("The string is: %s\n", path);
	return cnt;	
}

int main( void ){
//    char array[][50];
//    int SIZE;
	int WIDTH = 16;
	int HEIGHT = 3;
	char **dev = malloc(HEIGHT * sizeof(char*));
	int i;
	for (int i = 0; i < HEIGHT; i++){
		*dev[i] = malloc(WIDTH * sizeof(char));
	}
	
	dev[0][WIDTH]="dev 0, 16";
	*dev[1]="dev 1, 16";
	*dev[2]="dev 2, 16";

	//dev[0][16] = "0";
//	devPath[0][128] = "0";
//	int i;
	
	//printf("Dev variable before tempSensCnt is: %s\n", dev[0][16]);
	
	int tempSensCnt = getTempSensCnt();
	printf ("The count is: %d\n", tempSensCnt);

//    ...call function to fill array... this part works.
	//fillarray(dev);
    //printarray( array, SIZE );
	printf("Printed from main\n");
	for (i = 0; i<3; i++){
		printf("Dev variable is: %s\n", *dev[i]);
	}
}
//char *dev[0][16];
//char *devPath[0][128];


