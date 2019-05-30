//Build and run this program with something like:
//gcc -Wall arraytest.c -o arraytest
//sudo ./arraytest

//good book:  https://www.dipmat.univpm.it/~demeio/public/the_c_programming_language_2.pdf

#include <stdio.h>
#include <string.h>

char *path = "/sys/bus/w1/devices";    //char pointer to path (that doesn't change)
char *dev[][16];
char *devPath[][128];

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

void fillarray (){
	dev[0][16]="dev 0, 16";
	dev[1][16]="dev 1, 16";
	dev[2][16]="dev 2, 16";
	int i;
	printf("\n");
	printf("Printed from fillarry");
	for (i = 0; i<3; i++){
		printf("dev variable is: %s\n", dev[i][16]);
	}
	printf("\n");
}

int getTempSensCnt () {
	int cnt = 3;
	
	printf ("The string is: %s\n", path);
	return cnt;	
}

int main( void ){
//    char array[][50];
//    int SIZE;
	char dev[0][16] = "0";
	char devPath[0][128] = "0";
	int i;
	
	printf("Dev variable before tempSensCnt is: %s\n", dev[0][16]);
	
	int tempSensCnt = getTempSensCnt();
	printf ("The count is: %d\n", tempSensCnt);

//    ...call function to fill array... this part works.
	fillarray();
    //printarray( array, SIZE );
	printf("Printed from main\n");
	for (i = 0; i<3; i++){
		printf("Dev variable is: %s\n", dev[i][16]);
	}
}



/*  NOTES to Mark!!  I found that I could pass the pointer or not pass it 
through the function call.  The following exmaples are the SAME!
#include <stdio.h>
#include <string.h>

char *path = "/sys/bus/w1/devices";    //char pointer to path (that doesn't change)

int getTempSensCnt () {
	int cnt = 3;
	
	printf ("The string is: %s\n", path);
	return cnt;	
}

int main( void ){
	int tempSensCnt = getTempSensCnt();
	printf ("The count is: %d\n", tempSensCnt);
}

-------------  The ABOVE WORKED THE SAME As BELOW ------------------
#include <stdio.h>
#include <string.h>

char *path = "/sys/bus/w1/devices";    //char pointer to path (that doesn't change)

int getTempSensCnt (char *p) {
	int cnt = 3;
	
	printf ("The string is: %s\n", p);
	return cnt;	
}

int main( void ){
	int tempSensCnt = getTempSensCnt(path);
	printf ("The count is: %d\n", tempSensCnt);
}


 */
