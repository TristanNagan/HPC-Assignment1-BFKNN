#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <string.h>
#include <sys/time.h>
#include "qsort.h"
#include "allocate.h"
#include "populate.h"
#include "distance_metrics.h"
#include "mergesort.h"
#include "bitonicsort.h"
#include "printer.h"
#include "runner.h"

void run(int m_vals[], int m_size, int n_vals[], int n_size, int d_vals[], int d_size, int iter);

int main() {
	int dv[] = {32, 64, 128, 256, 512};
	int nv[] = {200, 400, 800, 1600};
	int mv[] = {100000, 50000};
	/*int dv[] = {1, 2, 4, 8};
	int nv[] = {2, 5};
	int mv[] = {10, 20};
	run(mv, 2, nv, 2, dv, 4, 1);*/
	run(mv, 2, nv, 4, dv, 5, 1);
	return 0;
}

void run(int m_vals[], int m_size, int n_vals[], int n_size, int d_vals[], int d_size, int iter){
	int arrsize = m_size*n_size*d_size*4*(4*8+9);
	static void (*DistmetricM[2])() = {run_serial_manhattan, run_para_manhattan};
	static void (*DistmetricE[2])() = {run_serial_euclid,  run_para_euclid};
	int metricDetailsM[2][2] = {{1, 0}, {1, 1}};
	int metricDetailsE[2][2] = {{0, 0}, {0, 1}};
	
	static void(*Sort[13])() = {run_serial_qsort, run_para_qsort_sections, run_para_qsort_sections_low, run_para_qsort_tasks, run_para_qsort_tasks_low, run_serial_mergesort, run_para_mergesort_sections, run_para_mergesort_sections_low, run_para_mergesort_tasks, run_para_mergesort_tasks_low, run_serial_bitonicsort, run_para_bitonicsort_sections, run_para_bitonicsort_tasks};
	int sortDetails[13][3] = {{0, 0, 0}, {0, 1, 0}, {0, 1, 1}, {0, 2, 0}, {0, 2, 1}, {1, 0, 0}, {1, 1, 0}, {1, 1, 1}, {1, 2, 0}, {1, 2, 1}, {2, 0, 0}, {2, 1, 0}, {2, 2, 0}};
	
	int low[] = {2, 5, 10, 50, 100, 500, 1000, 5000};
	
	/*char dType[arrsize][10];
	char sType[arrsize][10];
	char dPar[arrsize][10];
	char sPar[arrsize][10];
	int low_lim[arrsize];
	int mv[arrsize];
	int nv[arrsize];
	int dv[arrsize];
	double dtime[arrsize];
	double stime[arrsize];
	double otime[arrsize];
	int count = 0;*/
	FILE *distfile = fopen("dresults.txt","w");
	FILE *sortfile = fopen("sresults.txt","w");
	fprintf(distfile, "+===========+===========+===========+===========+===========+===========+\n");
	fprintf(distfile, "| Metric    | S or P    | m         | n         | d         | S Time    |\n");
	fprintf(distfile, "+===========+===========+===========+===========+===========+===========+\n");
	
	fprintf(sortfile, "+===========+===========+===========+===========+===========+===========+===========+\n");
	fprintf(sortfile, "| Sort      | S or P    | Low Limit | m         | n         | d         | S Time    |\n");
	fprintf(sortfile, "+===========+===========+===========+===========+===========+===========+===========+\n");
	for(int x = 0; x < m_size; x++){
		int m = m_vals[x];
		for(int y = 0; y < n_size; y++){
			int n = n_vals[y];
			for(int z = 0; z < d_size; z++){
				int d = d_vals[z];
				int **pos;
				float **ref, **query, **distM, **distE;
				
				ref = alloc2dFloat(m, d);
				query = alloc2dFloat(n, d);
				
				distM = alloc2dFloat(n, m);
				distE = alloc2dFloat(n, m);
				pos = alloc2dInt(n, m);
				
				ref = fill2dFloatWithRand(ref, m, d);
				query = fill2dFloatWithRand(query, n, d);
				pos = fill2dIntWithSeq(pos, n, m);
				
				for(int met = 0; met < 2; met++){
					printf("m = %i\n", m);
					printf("n = %i\n", n);
					printf("d = %i\n", d);
					printf("Euclidean = %i\n", met);
					double start = omp_get_wtime();
					DistmetricE[met](&distE[0], &ref[0], &query[0], n, m, d);
					double drun_time = omp_get_wtime() - start;
					char dPar[10];
					if(metricDetailsE[met][1] == 0){
						strcpy(dPar, "Serial");
					} else{
						strcpy(dPar, "Parallel");
					}
					fprintf(distfile, "| %-10s| %-10s| %-10i| %-10i| %-10i| %-10f|\n", "Euclidean", dPar, m, n, d, drun_time);
				}
				
				for(int met = 0; met < 2; met++){
					printf("m = %i\n", m);
					printf("n = %i\n", n);
					printf("d = %i\n", d);
					printf("Manhattan = %i\n", met);
					double start = omp_get_wtime();
					DistmetricM[met](&distM[0], &ref[0], &query[0], n, m, d);
					double drun_time = omp_get_wtime() - start;
					char dPar[10];
					if(metricDetailsM[met][1] == 0){
						strcpy(dPar, "Serial");
					} else{
						strcpy(dPar, "Parallel");
					}
					fprintf(distfile, "| %-10s| %-10s| %-10i| %-10i| %-10i| %-10f|\n", "Manhattan", dPar, m, n, d, drun_time);
				}
				fprintf(distfile, "+===========+===========+===========+===========+===========+===========+\n");
				for(int u = 0; u < m; u++){
					free(ref[u]);
				}
				free(ref);
				for(int u = 0; u < n; u++){
					free(query[u]);
				}
				free(query);
				
				for(int met = 0; met < 2; met++){
					for(int s = 0; s < 13; s++){
						printf("m = %i\n", m);
						printf("n = %i\n", n);
						printf("d = %i\n", d);
						printf("Metric = %i\n", met);
						printf("sort = %i\n", s);
						if(sortDetails[s][2] == 1){
							for(int l = 0; l < 8; l++){
								printf("low lim = %i\n", l);
								float **dist = alloc2dFloat(n, m);
								int **posT = alloc2dInt(n, m);
								copy2dInt(pos, posT, n, m);
								if(met == 0){
									copy2dFloat(distE, dist, n, m);
								} else{
									copy2dFloat(distE, dist, n, m);
								}
								char sType[10];
								char sPar[10];
								double start = omp_get_wtime();
								Sort[s](dist, posT, n, m, low[l]);
								double stime = omp_get_wtime() - start;
								if(sortDetails[s][0] == 0){
									strcpy(sType, "Quick");
								} else if(sortDetails[s][0] == 1){
									strcpy(sType, "Merge");
								} else{
									strcpy(sType, "Bitonic");
								}
								
								if(sortDetails[s][1] == 0){
									strcpy(sPar, "Serial");
								} else if(sortDetails[s][1] == 1){
									strcpy(sPar, "Section");
								} else{
									strcpy(sPar, "Task");
								}
								for(int u = 0; u < n; u++){
									free(dist[u]);
									free(posT[u]);
								}
								free(dist);
								free(posT);
								fprintf(sortfile, "| %-10s| %-10s| %-10i| %-10i| %-10i| %-10i| %-10f|\n", sType, sPar, low[l], m, n, d, stime);
							}
							fprintf(sortfile, "+===========+===========+===========+===========+===========+===========+===========+\n");
						} else{
							float **dist = alloc2dFloat(n, m);
							int **posT = alloc2dInt(n, m);
							copy2dInt(pos, posT, n, m);
							if(met == 0){
								copy2dFloat(distE, dist, n, m);
							} else{
								copy2dFloat(distM, dist, n, m);
							}
							char sType[10];
							char sPar[10];
							double start = omp_get_wtime();
							Sort[s](dist, posT, n, m, 0);
							double stime = omp_get_wtime() - start;
							if(sortDetails[s][0] == 0){
								strcpy(sType, "Quick");
							} else if(sortDetails[s][0] == 1){
								strcpy(sType, "Merge");
							} else{
								strcpy(sType, "Bitonic");
							}
							
							if(sortDetails[s][1] == 0){
								strcpy(sPar, "Serial");
							} else if(sortDetails[s][1] == 1){
								strcpy(sPar, "Section");
							} else{
								strcpy(sPar, "Task");
							}
							for(int u = 0; u < n; u++){
								free(dist[u]);
								free(posT[u]);
							}
							free(dist);
							free(posT);
							fprintf(sortfile, "| %-10s| %-10s| %-10i| %-10i| %-10i| %-10i| %-10f|\n", sType, sPar, 0, m, n, d, stime);
						}
						printf("\n\n");
					}
					fprintf(sortfile, "+===========+===========+===========+===========+===========+===========+===========+\n");
				}
				for(int u = 0; u < n; u++){
					free(distE[u]);
					free(distM[u]);
					free(pos[u]);
				}
				free(distE);
				free(distM);
				free(pos);
			}
		}
	}
	fclose(distfile);
	fclose(sortfile);
}

