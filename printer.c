#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <string.h>
#include <sys/time.h>

void printV(float *arr, int r){
	for(int i = 0; i < r; i++){
		printf("%f\n", *(arr+i));
	}
}

void print2V(int *a1, float *a2, int r){
	for(int i = 0; i < r; i++){
		printf("%i %f\n", a1[i], a2[i]);
	}
}

void print2Vf(float *a1, float *a2, int r){
	for(int i = 0; i < r; i++){
		printf("%f %f\n", *(a1+i), *(a2+i));
		printf("%f\n\n", fabs(a1[i] - a2[i]));
	}
}

void printArr(float **arr, int r, int c){
	for(int i = 0; i < r; i++){
		for(int j = 0; j < c; j++){
			printf("%f ", arr[i][j]);
		}
		printf("\n");
	}
}

void printTab(char dtype[][10], char stype[][10], char dPar[][10], char sPar[][10], int low_lim[], int m[], int n[], int d[], double dtime[], double stime[], double otime[], int size){
FILE *file = fopen("results.txt","w");	fprintf(file, "+===========+===========+===========+===========+===========+===========+===========+===========+===========+===========+===========+===========+===========+\n");
	fprintf(file, "| Metric    | S or P    | Sort      | S or P    | Low Limit | m         | n         | d         | D Time    | S Time    | D(Percent)| S(Percent)| Overall   |\n");
	fprintf(file, "+===========+===========+===========+===========+===========+===========+===========+===========+===========+===========+===========+===========+===========+\n");
	for(int i = 0; i < size; i++){
		fprintf(file, "| %-10s| %-10s| %-10s| %-10s| %-10i| %-10i| %-10i| %-10i| %-10f| %-10f| %-10f| %-10f| %-10f|\n", dtype[i], dPar[i], stype[i], sPar[i], low_lim[i], m[i], n[i], d[i], dtime[i], stime[i], (dtime[i]/otime[i])*100, (stime[i]/otime[i])*100, otime[i]);
		if((i != size-1) && (strcmp(dPar[i+1], dPar[i]) != 0 || strcmp(dtype[i+1], dtype[i]) || m[i+1] != m[i] || n[i+1] != n[i] || d[i+1] != d[i])){
			fprintf(file, "+===========+===========+===========+===========+===========+===========+===========+===========+===========+===========+===========+===========+===========+\n");
		}
	}
	fprintf(file, "+===========+===========+===========+===========+===========+===========+===========+===========+===========+===========+===========+===========+===========+\n");
}
