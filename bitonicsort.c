#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void bitonic_compare_swap(int i, int j, int dir, float *data, int *pos){
	if(dir == (data[i] > data[j])){
		float temp = data[i];
		int tp = pos[i];
		data[i] = data[j];
		pos[i] = pos[j];
		data[j] = temp;
		pos[j] = tp;
	}
}

int pow_of_2_less_than(int n){
	int k = 1;
	while(k > 0 && k < n){
		k = k<<1;
	}
	return (unsigned int) k >> 1;
}

void bitonic_merge(int index, int size, int dir, float *data, int *pos){
	if(size > 1){
		int m = pow_of_2_less_than(size);
		for(int i = index; i < index + size - m; i++){
			bitonic_compare_swap(i, i + m, dir, data, pos);
		}
		bitonic_merge(index, m, dir, data, pos);
		bitonic_merge(index + m, size - m, dir, data, pos);
	}
}

void serial_bitonicsort(int index, int size, int dir, float *data, int *pos){
	if(size > 1){
		int m = size/2;
		serial_bitonicsort(index, m, !dir, data, pos);
		serial_bitonicsort(index + m, size - m, dir, data, pos);
		bitonic_merge(index, size, dir, data, pos);
	}
}

void par_bitonicsort_sections(int index, int size, int dir, float *data, int *pos){
	if(size > 1){
		int m = size/2;
		omp_set_num_threads(4);
		#pragma omp parallel sections
		{
			#pragma omp section
			{
				par_bitonicsort_sections(index, m, !dir, data, pos);
			}
			#pragma omp section
			{
				par_bitonicsort_sections(index + m, size - m, dir, data, pos);
			}
		}
		bitonic_merge(index, size, dir, data, pos);
	}
}

void par_bitonicsort_tasks(int index, int size, int dir, float *data, int *pos){
	if(size > 1){
		int m = size/2;
		#pragma omp task
		{
			par_bitonicsort_tasks(index, m, !dir, data, pos);
		}
		#pragma omp task
		{
			par_bitonicsort_tasks(index + m, size - m, dir, data, pos);
		}
		#pragma omp taskwait
		bitonic_merge(index, size, dir, data, pos);
	}
}

