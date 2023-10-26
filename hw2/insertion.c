#include "insertion.h"
#include <stdio.h>
#include <string.h>

/*
	We emphasize this: because this is a generic implementation, in insertion.c you must not use if-else to
	discriminate different data types in your code. Your code must be able to deal with any data type possible.
	Therefore, we limit the usage of the following primitive types:
	- int or size_t: only allowed as loop iterators;
	- char or u_int8_t: can be used as you wish;
	- Types that are used in the declarations of iSort() and iPrint(): feel free to use them for their _original_
      purpose;
	- All other types are not allowed to appear in insertion.c: no exceptions, no matter if you actually used
	  them, or what purpose it is. Once it appears, you will receive 30% reduction of the homework.

	You are free to create helper functions only in insertion.c; you must declare and implement them in the
    .c file instead of the header files. Do not change any of the header files.

	Isabel Sutedjo
	I pledge my honor that I have abided by the Stevens Honor System
	
*/

void iSort(void* base, size_t nel, size_t width, int (*compare)(void*,void*)) {
	void* key = malloc(width);
	for (size_t x = 1; x < nel; x++){
		void* z = base + x * width; //intermediary temp address
		size_t y = x;
		while (y-- > 0){
			//using compare from utils.c
			int comp = ((*compare) (z, base + y * width));
			if (0 <= comp){
				break;
		  	}
		}
		y++;
		//coping to key
		memcpy(key, z, width);
		//variables
		int* dest = (base + (y + 1) * width);
		int* orig = (base + y * width);
		size_t size = ((x - y) * width);
		//overlapping source and destination, need memmove
		memmove(dest, orig, size);
		memcpy(orig, key, width);
	}
	free(key);
}

void iPrint(void* base, size_t nel, size_t width, void (*print)(void*)) {
	int temp = 0;
	while (temp < nel){
		(print)(base);
		base += width;
		temp++;
	}
}