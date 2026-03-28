.phony: clean

SRCS=crear_tuberia.c tuberias_nombradas.c borrar_tuberia.c servidor.c cliente.c servidor_example.c cliente_example.c
OBJS=$(SRCS:.c=.o)
EXECS=crear_tuberia borrar_tuberia servidor cliente servidor_example cliente_example

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

servidor_example: servidor_example.o
	${CC} -o $@ $<

servidor_example.o: servidor_example.c

cliente_example: cliente_example.o
	${CC} -o $@ $<

cliente_example.o: cliente_example.c

clean:
	rm -f ${OBJS}
	rm -f ${EXECS}
	rm -f *~
