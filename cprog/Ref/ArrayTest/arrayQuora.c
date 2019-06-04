//Purpose of this program is to try malloc for 2d char array AND
//  have clang from microsoft compile it successfully.

//Build and run this program with something like:
//gcc -Wall arraymalloc.c -o arraymalloc
//sudo ./arraymalloc

//good book:  https://www.dipmat.univpm.it/~demeio/public/the_c_programming_language_2.pdf

// good example here:  https://www.quora.com/How-can-I-declare-a-2-dimensional-array-of-char-in-C


#include <stdlib.h> // Needed for malloc and free
#include <stdio.h>
#include <string.h>

#define WIDTH 5
#define HEIGHT 10 
 
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
 
int main(int argc, char* argv[]) {
	char **array_2d = make_2d_array(WIDTH, HEIGHT);
	int i;
 
	// DO STUFF WITH THE ARRAY
	strcpy(array_2d[0], "POS0");
	strcpy(array_2d[1], "POS1");
	strcpy(array_2d[2], "POS2");
	
	
	//printf("Dev variable is: %s\n", dev[i][16]);
	//printf("Position value is:\n%s\n", array_2d[2]);
	
	printf("Position value is:\n");
	for (i = 0; i<3; i++){
		printf("%s\n", array_2d[i]);
	}
	printf("\n");


	/*
	 * It is important to free the memory you allocated
	 * in the function using malloc, here that is:
	*/
	for (int i = 0; i < HEIGHT; i++) {
		free(array_2d[i]);
	} 
	free(array_2d);
	
	// First we freed each row, then we freed the
	// 'row holder' outer array 
 
	return 0;
}

