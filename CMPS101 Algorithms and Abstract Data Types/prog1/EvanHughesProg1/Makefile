GCC       = gcc -g -O0 -Wall -Wextra -std=gnu99

CSOURCE   = store.c List.c List.h
CDRIVER   = Listdr.c List.c List.h
OBJECTS   = ${CSOURCE:.c=.o}
DROBJEC   = ${CDRIVER:.c=.o}
EXECBIN   = store
EXEC      = Listdr
SOURCES   = ${CSOURCE} Makefile


all : ${EXECBIN}

driver : ${EXEC}

${EXECBIN} : ${OBJECTS}
	${GCC} -o $@ ${OBJECTS}

${EXEC} : ${DROBJEC}
	${GCC} -o $@ ${DROBJEC}

%.o : %.c
	cid + $<
	${GCC} -c $<
	


