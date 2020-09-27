INC="./inc"
FLAGS=-I$(INC)
OMPFLAG=-fopenmp
CC=gcc

all: knn

knn: knn.c
	$(CC) knn.c qsort.c allocate.c populate.c distance_metrics.c mergesort.c bitonicsort.c printer.c runner.c -lm -o knn $(OMPFLAG)

clean:
	rm knn
