.phony: clean

SRCS=reg_eventos.c
EVENTS=eventos.mc
#RESOURCES=$(EVENTS:.mc=.rc)
#RESOURCES_OBJ=$(EVENTS:.mc=.o)
#HEADERS=$(EVENTS:.mc=.h)
OBJS=$(SRCS:.c=.o)
EXECS=reg_eventos

all: ${EXECS}

reg_eventos: reg_eventos.o # eventos.o
	${CC} -o $@ $^

# eventos.o: eventos.rc
#	windres -o $@ -i $<

# Produce eventos.h eventos.rc *.bin
# eventos.rc: eventos.mc
#	windmc -U $<

clean:
	rm -f ${OBJS} # ${RESOURCES_OBJ}
	rm -f ${EXECS}
# rm -f ${HEADERS} ${RESOURCES} *bin          # Ficheros intermedios
	rm -f *~
