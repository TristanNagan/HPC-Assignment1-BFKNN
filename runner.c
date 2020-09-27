#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <string.h>
#include <sys/time.h>
#include "qsort.h"
#include "distance_metrics.h"
#include "mergesort.h"
#include "bitonicsort.h"

void run_serial_euclid(float **dist, float **ref, float **query, int n, int m, int d){
	for(int q = 0; q < n; q++){
		for(int r = 0; r < m; r++){
			dist[q][r] = euclidean(query[q], ref[r], d);
		}
	}
}

void run_para_euclid(float **dist, float **ref, float **query, int n, int m, int d){
	int i;
	omp_set_num_threads(4);
	#pragma omp parallel for private(i) shared(dist)
	for(i = 0; i < n*m; i++){
		int q = i/m, r = i%m;
		dist[q][r] = euclidean(query[q], ref[r], d);
	}
}

void run_serial_manhattan(float **dist, float **ref, float **query, int n, int m, int d){
	for(int q = 0; q < n; q++){
		for(int r = 0; r < m; r++){
			dist[q][r] = manhattan(query[q], ref[r], d);
		}
	}
}

void run_para_manhattan(float **dist, float **ref, float **query, int n, int m, int d){
	int i;
	omp_set_num_threads(4);
	#pragma omp parallel for private(i) shared(dist)
	for(i = 0; i < n*m; i++){
		int q = i/m, r = i%m;
		dist[q][r] = manhattan(query[q], ref[r], d);
	}
}

void run_serial_qsort(float **dist, int **pos, int n, int m, int low){
	for(int i = 0; i < n; i++){
		serial_qsort(0, m-1, &dist[i][0], &pos[i][0], low);
	}	
}

void run_para_qsort_sections(float **dist, int **pos, int n, int m, int low){
	for(int i = 0; i < n; i++){
		par_qsort_sections(0, m-1, &dist[i][0], &pos[i][0], low);
	}
}

void run_para_qsort_tasks(float **dist, int **pos, int n, int m, int low){
	for(int i = 0; i < n; i++){
		omp_set_num_threads(4);
		#pragma omp parallel
		{
			#pragma omp single nowait
			{
	    			par_qsort_tasks(0, m-1, &dist[i][0], &pos[i][0], low);
	    		}
		}
	}
}

void run_para_qsort_sections_low(float **dist, int **pos, int n, int m, int low){
	for(int i = 0; i < n; i++){
		par_qsort_sections_low(0, m-1, &dist[i][0], &pos[i][0], low);
	}
}

void run_para_qsort_tasks_low(float **dist, int **pos, int n, int m, int low){
	for(int i = 0; i < n; i++){
		omp_set_num_threads(4);
		#pragma omp parallel
		{
			#pragma omp single nowait
			{
	    			par_qsort_tasks_low(0, m-1, &dist[i][0], &pos[i][0], low);
	    		}
		}
	}
}

void run_serial_mergesort(float **dist, int **pos, int n, int m, int low){
	for(int i = 0; i < n; i++){
		serial_mergesort(0, m-1, &dist[i][0], &pos[i][0], low);
	}	
}

void run_para_mergesort_sections(float **dist, int **pos, int n, int m, int low){
	for(int i = 0; i < n; i++){
		par_mergesort_sections(0, m-1, &dist[i][0], &pos[i][0], low);
	}
}

void run_para_mergesort_tasks(float **dist, int **pos, int n, int m, int low){
	for(int i = 0; i < n; i++){
		omp_set_num_threads(4);
		#pragma omp parallel
		{
			#pragma omp single nowait
			{
	    			par_mergesort_tasks_low(0, m-1, &dist[i][0], &pos[i][0], low);
	    		}
		}
	}
}

void run_para_mergesort_sections_low(float **dist, int **pos, int n, int m, int low){
	for(int i = 0; i < n; i++){
		par_mergesort_sections_low(0, m-1, &dist[i][0], &pos[i][0], low);
	}
}

void run_para_mergesort_tasks_low(float **dist, int **pos, int n, int m, int low){
	for(int i = 0; i < n; i++){
		omp_set_num_threads(4);
		#pragma omp parallel
		{
			#pragma omp single nowait
			{
	    			par_mergesort_tasks(0, m-1, &dist[i][0], &pos[i][0], low);
	    		}
		}
	}
}

void run_serial_bitonicsort(float **dist, int **pos, int n, int m, int l){
	for(int i = 0; i < n; i++){
		serial_bitonicsort(0, m, 1, &dist[i][0], &pos[i][0]);
	}	
}

void run_para_bitonicsort_sections(float **dist, int **pos, int n, int m, int l){
	for(int i = 0; i < n; i++){
		par_bitonicsort_sections(0, m, 1, &dist[i][0], &pos[i][0]);
	}
}

void run_para_bitonicsort_tasks(float **dist, int **pos, int n, int m, int l){
	for(int i = 0; i < n; i++){
		omp_set_num_threads(4);
		#pragma omp parallel
		{
			#pragma omp single nowait
			{
	    			par_bitonicsort_tasks(0, m, 1, &dist[i][0], &pos[i][0]);
	    		}
		}
	}
}

