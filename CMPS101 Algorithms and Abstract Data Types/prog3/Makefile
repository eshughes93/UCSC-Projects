# Evan Hughes [eshughes@ucsc.edu]
# Cristian Gonzalez [cfgonzal@ucsc.edu]
GCC       = gcc -g -O0 -Wall -Wextra -std=gnu99

CSOURCE   = Main.c bookCountList.h bookCountList.c bookListList.h bookListList.c hash.h hash.c
CLDRIVER  = bookCountListDr.c bookCountList.c bookCountList.h
LLDRIVER  = bookListListDr.c bookListList.c bookListList.h bookCountList.c bookCountList.h
HTDRIVER  = bookCountList.c bookCountList.h bookListList.c bookListList.h hash.c hash.h hashDr.c 
OBJECTS   = ${CSOURCE:.c=.o}
CLOBJCT   = ${CLDRIVER:.c=.o}
LLOBJCT   = ${LLDRIVER:.c=.o}
HTOBJCT   = ${HTDRIVER:.c=.o}
EXECBIN   = prog3

SOURCES   = ${CSOURCE} Makefile


all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${GCC} -o $@ ${OBJECTS}

countList : ${CLOBJCT}
	${GCC} -o $@ ${CLOBJCT}

chainList : ${LLOBJCT}
	${GCC} -o $@ ${LLOBJCT}

hashDriver : ${HTOBJCT}
	${GCC} -o $@ ${HTOBJCT}

%.o : %.c
	${GCC} -c $<
	


