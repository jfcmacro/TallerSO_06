.phony: clean install

# Desde el punto de vista de msys2, en otros sistemas se debe cambiar
# el directorio destino
INSTALLDIR=/usr/local/bin
SRCS=svrwindows.c registrar_servicio.c desregistrar_servicio.c
OBJS=$(SRCS:.c=.o)
EXECS=$(OBJS:.o=)

all: ${EXECS}

svrwindows: svrwindows.o

svrwindows.o: svrwindows.c svrwindows.h

registrar_servicio: registrar_servicio.o

registrar_servicio.o: registrar_servicio.c svrwindows.h

desregistrar_servicio: desregistrar_servicio.o

desregistrar_servicio.o: desregistrar_servicio.c svrwindows.h

install: svrwindows
	install $< -t ${INSTALLDIR}

clean:
	rm -f ${OBJS}
	rm -f ${EXECS}
	rm -f *~
