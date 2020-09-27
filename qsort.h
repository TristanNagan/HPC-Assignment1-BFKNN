int partition(int, int, float *, int *);
void serial_qsort(int, int, float *, int *, int);
void par_qsort_sections(int, int, float *, int *, int);
void par_qsort_tasks(int, int, float *, int *, int);
void par_qsort_sections_low(int, int, float *, int *, int);
void par_qsort_tasks_low(int, int, float *, int *, int);
