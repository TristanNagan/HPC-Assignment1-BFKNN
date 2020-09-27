void merge(int, int, int, float *, int *);
void serial_mergesort(int, int, float *, int *, int);
void par_mergesort_sections(int, int, float *, int *, int);
void par_mergesort_tasks(int, int, float *, int *, int);
void par_mergesort_sections_low(int l, int r, float *data, int *pos, int low_limit);
void par_mergesort_tasks_low(int l, int r, float *data, int *pos, int low_limit);
