#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

int partition(int p, int r, float *data, int *pos){
	float x = data[p];
	int k = p; 
	int l = r+1;
	float t;
	int tp;
	while(1){
		do{
			k++;
		}
		while((data[k] <= x) && (k < r));
		do{
			l--;
		}
		while(data[l] > x);
		while(k < l){
			t = data[k];
			tp = pos[k];
			data[k] = data[l];
			pos[k] = pos[l];
			data[l] = t;
			pos[l] = tp;
			do{
				k++;
			}
			while(data[k] <= x);
			do{
				l--;
			}
			while(data[l] > x);
		}
		t = data[p];
		tp = pos[p];
		data[p] = data[l];
		pos[p] = pos[l];
		data[l] = t;
		pos[l] = tp;
		return l;
	}
}

void serial_qsort(int p, int r, float *data, int *pos, int low_limit){
	if(p < r){
		int q = partition(p, r, data, pos);
		serial_qsort(p, q-1, data, pos, low_limit);
		serial_qsort(q+1, r, data, pos, low_limit);
	}
}

void par_qsort_sections(int p, int r, float *data, int *pos, int low_limit){
	if(p < r){
		int q = partition(p, r, data, pos);
		omp_set_num_threads(4);
		#pragma omp parallel sections
		{
			#pragma omp section
			{
				par_qsort_sections(p, q-1, data, pos, low_limit);
			}
			#pragma omp section
			{
				par_qsort_sections(q+1, r, data, pos, low_limit);
			}
		}
	}
}

void par_qsort_tasks(int p, int r, float *data, int *pos, int low_limit){
	if(p < r){
		int q=partition(p, r, data, pos);
		#pragma omp task
		{
			par_qsort_tasks(p, q-1, data, pos, low_limit);
		}
		#pragma omp task
		{
			par_qsort_tasks(q+1, r, data, pos, low_limit);
		} 
		#pragma omp taskwait
	}
}

void par_qsort_sections_low(int p, int r, float *data, int *pos, int low_limit){
	if(p < r){
		if((r - p) < low_limit){
			serial_qsort(p, r, data, pos, low_limit);
		} else{
			int q = partition(p, r, data, pos);
			omp_set_num_threads(4);
			#pragma omp parallel sections
			{
				#pragma omp section
				{
					par_qsort_sections(p, q-1, data, pos, low_limit);
				}
				#pragma omp section
				{
					par_qsort_sections(q+1, r, data, pos, low_limit);
				}
			}
		}
	}
}

void par_qsort_tasks_low(int p, int r, float *data, int *pos, int low_limit){
	if(p < r){
		if((r - p) < low_limit){
			serial_qsort(p, r, data, pos, low_limit);
		} else{
			int q=partition(p, r, data, pos);
			#pragma omp task
			{
				par_qsort_tasks(p, q-1, data, pos, low_limit);
			}
			#pragma omp task
			{
				par_qsort_tasks(q+1, r, data, pos, low_limit);
			} 
			#pragma omp taskwait
		}
	}
}
