#include <stdlib.h>
#include <stdio.h>
#include <string.h>

float** alloc2dFloat(int rows, int cols){
	float **out  = (float**)malloc(rows*sizeof(float*));
	for(int i = 0; i < rows; i++){
		out[i] = (float*)malloc(cols*sizeof(float));
	}
	return out;
}

float* alloc1dFloat(int size){
	float *out = (float*)malloc(size*sizeof(float));
	return out;
}

int* alloc1dInt(int size){
	int *out = (int*)malloc(size*sizeof(int));
	return out;
}

int** alloc2dInt(int row, int cols){
	int **out = (int**)malloc(row*sizeof(int*));
	for(int i = 0; i < row; i++){
		out[i] = (int*)malloc(cols*sizeof(int));
	}
	return out;
}

void copy2dFloat(float **src, float **dest, int row, int col){
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			dest[i][j] = src[i][j];
		}
	}
}

void copy2dInt(int **src, int **dest, int row, int col){
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			dest[i][j] = src[i][j];
		}
	}
}
