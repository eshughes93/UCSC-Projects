# Evan Hughes [eshughes@ucsc.edu]
GCC       = gcc -g -O0 -Wall -Wextra -std=gnu99

CSOURCE   = main.c graph.c graph.h List.c List.h
GDRIVER   = graphDr.c graph.c graph.h List.c List.h
CLDRIVER  = Listdr.c List.c List.h
OBJECTS   = ${CSOURCE:.c=.o}
GOBJCT    = ${GDRIVER:.c=.o}
CLOBJCT   = ${CLDRIVER:.c=.o}
EXECBIN   = prog4

SOURCES   = ${CSOURCE} Makefile


all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${GCC} -o $@ ${OBJECTS}

graphDr : ${GOBJCT}
	${GCC} -o $@ ${GOBJCT}

listDr : ${CLOBJCT}
	${GCC} -o $@ ${CLOBJCT}

%.o : %.c
	${GCC} -c $<
	


