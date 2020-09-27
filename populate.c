#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

float** fill2dFloatWithRand(float **arr, int rows, int cols){
	//srand(time(0));
	for(int r = 0; r < rows; r++){
		for(int c = 0; c < cols; c++){
			arr[r][c] = rand();
		}
	}
	return arr;
}

float* fill1dFloatWith0(float *arr, int rows){
	for(int r = 0; r < rows; r++){
		arr[r] = 0;
	}
	return arr;
}

int** fill2dIntWithSeq(int **arr, int rows, int cols){
	for(int r = 0; r < rows; r++){
		for(int c = 0; c < cols; c++){
			arr[r][c] = c;
		}
	}
	return arr;
}
