#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

float euclidean(float *p0, float *p1, int d){
	float sum = 0.0;
	for(int i = 0; i < d; i++){
		sum += powf(p0[i] - p1[i], 2);
	}
	return sqrtf(sum);
}

float manhattan(float *p0, float *p1, int d){
	float sum = 0.0;
	for(int i = 0; i < d; i++){
		sum += fabs(p0[i] - p1[i]);
	}
	return sum;
}
