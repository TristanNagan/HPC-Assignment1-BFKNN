#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

void merge(int l, int m, int r, float *data, int *pos) {
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;
	
	float *L = (float*)malloc(n1*sizeof(float));
	float *R = (float*)malloc(n2*sizeof(float));
	int *Lpos = (int*)malloc(n1*sizeof(int));
	int *Rpos = (int*)malloc(n1*sizeof(int));
	
	for (i = 0; i < n1; i++){
		L[i] = data[l + i];
		Lpos[i] = pos[l + i];
	}
	for (j = 0; j < n2; j++){
		R[j] = data[m + 1 + j];
		Rpos[j] = pos[m + 1 + j];
	}
	
	i = 0, j = 0, k = l;
	
	while (i < n1 && j < n2){
		if (L[i] <= R[j]){
			data[k] = L[i];
			pos[k] = Lpos[i];
			i++;
		} else{
			data[k] = R[j];
			pos[k] = Rpos[j];
			j++;
		}
		k++;
	}
	
	while (i < n1){
		data[k] = L[i];
		pos[k] = Lpos[i];
		i++;
		k++;
	}
	
	while (j < n2){
		data[k] = R[j];
		pos[k] = Rpos[j];
		j++;
		k++;
	}
	free(L);
	free(R);
	free(Lpos);
	free(Rpos);
}

void serial_mergesort(int l, int r, float *data, int *pos, int low_limit) {
	if (l < r){
		int m = l+(r-l)/2;
		serial_mergesort(l, m, data, pos, low_limit);
		serial_mergesort(m+1, r, data, pos, low_limit);
		merge(l, m, r, data, pos);
	}
}

void par_mergesort_sections(int l, int r, float *data, int *pos, int low_limit){
	if (l < r){
		int m = l+(r-l)/2;
		omp_set_num_threads(4);
		#pragma omp parallel sections
		{
			#pragma omp section
			{
				par_mergesort_sections(l, m, data, pos, low_limit);
			}
			#pragma omp section
			{
				par_mergesort_sections(m+1, r, data, pos, low_limit);
			}
		}
		merge(l, m, r, data, pos);
	}
}

void par_mergesort_tasks(int l, int r, float *data, int *pos, int low_limit) {
	if (l < r){
		int m = l+(r-l)/2;
		#pragma omp task
		{
			par_mergesort_tasks(l, m, data, pos, low_limit);
		}
		#pragma omp task
		{
			par_mergesort_tasks(m+1, r, data, pos, low_limit);
		}
		#pragma omp taskwait
		merge(l, m, r, data, pos);
	}
}

void par_mergesort_sections_low(int l, int r, float *data, int *pos, int low_limit){
	if (l < r){
		if((r - l) < low_limit){
			serial_mergesort(l,r,data,pos, low_limit);
		} else{
			int m = l+(r-l)/2;
			omp_set_num_threads(4);
			#pragma omp parallel sections
			{
				#pragma omp section
				{
					par_mergesort_sections_low(l, m, data, pos, low_limit);
				}
				#pragma omp section
				{
					par_mergesort_sections_low(m+1, r, data, pos, low_limit);
				}
			}
			merge(l, m, r, data, pos);
		}
	}
}

void par_mergesort_tasks_low(int l, int r, float *data, int *pos, int low_limit) {
	if (l < r){
		if((r - l) < low_limit){
			serial_mergesort(l,r,data,pos, low_limit);
		} else{
			int m = l+(r-l)/2;
			#pragma omp task
			{
				par_mergesort_tasks_low(l, m, data, pos, low_limit);
			}
			#pragma omp task
			{
				par_mergesort_tasks_low(m+1, r, data, pos, low_limit);
			}
			#pragma omp taskwait
			merge(l, m, r, data, pos);
		}
	}
}
