#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/*
	You are free to use any data type you like in this file.
    However, other than the five functions declared in "utils.h",
    DO NOT create any other functions.
	Isabel Sutedjo
	I pledge my honor that I have abided by the Stevens Honor System.
	
*/

int cmpr_int(void* int1, void* int2) {
	if ((*(int*)int1 - *(int*)int2) > 0){
		return 1;
	}
	else if ((*(int*)int1 - *(int*)int2) < 0){
		return -1;
	}
	else{
		return 0;
	}
}

int cmpr_float(void* float1, void* float2) {
	if ((*(float*)float1 - *(float*)float2) > 0){
		return 1;
	}
	else if ((*(float*)float1 - *(float*)float2) < 0){
		return -1;
	}
	else {
		return 0;
	}
}

void print_int(void* elem) {
	printf("%d\n", *(int*) elem);
}

void print_float(void* elem) {
	printf("%f\n", *(float*) elem);
}

void* read_array(char* filename, char* format, size_t* len) {
	FILE* fp = fopen(filename, "r");
	int* arr;
	int* array;

	//error checking if can read file
	if ((fp = fopen(filename, "r")) == NULL){
		fprintf(stderr, "File failed to open.\n");
		exit(1);
	}

	//declaring new array
	arr = (void*)malloc(sizeof(int));

	//count loop to update len
	while (fscanf(fp, format, arr) >= 0){
		(*len) += 1;
	}
	fseek(fp, 0, SEEK_SET);
	//dynamically change the memory of previously allocated memory
	array = (void*)realloc(arr, ((*len) * sizeof(int)));

	//read file stream
	size_t x = 0;
	while (x < (*len)){
		fscanf(fp, format, &array[x]);
		x++;
	}
	
	//close the file
	fclose(fp);
	return array;
}