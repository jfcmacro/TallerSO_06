.phony: clean

SRCS=crear_tuberia.c tuberias_nombradas.c borrar_tuberia.c servidor.c cliente.c
OBJS=$(SRCS:.c=.o)
EXECS=crear_tuberia borrar_tuberia servidor cliente

all: ${EXECS}

crear_tuberia: crear_tuberia.o tuberias_nombradas.o
	${CC} -o $@ $^

borrar_tuberia: borrar_tuberia.o tuberias_nombradas.o
	${CC} -o $@ $^

crear_tuberia.o: crear_tuberia.c tuberias_nombradas.h

tuberias_nombradas.o: tuberias_nombradas.c tuberias_nombradas.h

servidor: servidor.o
	${CC} -o $@ $<

servidor.o: servidor.c tuberias_nombradas.h servidor.h

cliente: cliente.o
	${CC} -o $@ $<

cliente.o: cliente.c tuberias_nombradas.h servidor.h

clean:
	rm -f ${OBJS}
	rm -f ${EXECS}
	rm -f *~
