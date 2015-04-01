GCC       = gcc -g -O0 -Wall -Wextra -std=gnu99

CSOURCE   = sortComp.c insertionSort.c insertionSort.h heapSort.c heapSort.h heap.c heap.h List.c List.h
CPRINT    = sortPrint.c insertionSort.c insertionSort.h heapSort.c heapSort.h heap.c heap.h List.c List.h
CDRIVER   = heapDriver.c heap.c heap.h
OBJECTS   = ${CSOURCE:.c=.o}
PROBJEC   = ${CPRINT:.c=.o}
DROBJEC   = ${CDRIVER:.c=.o}
EXECBIN   = sortComp
EXEC      = sortPrint
DREC      = heapDriver
SOURCES   = ${CSOURCE} Makefile


all : ${EXECBIN}

print : ${EXEC}

driver : ${DREC}

${EXECBIN} : ${OBJECTS}
	${GCC} -o $@ ${OBJECTS}

${EXEC} : ${PROBJEC}
	${GCC} -o $@ ${PROBJEC}

${DREC} : ${DROBJEC}
	${GCC} -o $@ ${DROBJEC}

%.o : %.c
	${GCC} -c $<
	


