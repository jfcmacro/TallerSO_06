.phony: clean

SRCS=reg_eventos.c
OBJS=$(SRCS:.c=.o)
EXECS=reg_eventos

all: ${EXECS}

reg_eventos: reg_eventos.o
	${CC} -o $@ $<

clean:
	rm -f ${OBJS}
	rm -f ${EXECS}
	rm -f *~
