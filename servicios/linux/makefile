SRCS=systemd-service.c
OBJS=$(SRCS:.c=.o)
EXECS=$(OBJS:.o=)
INSTALLDIR=/usr/local/bin

all: ${EXECS}

systemd-service: systemd-service.o

systemd-service.o: systemd-service.c

install: systemd-service systemd-service.service
	install $< -t ${INSTALLDIR}
	cp systemd-service.service /etc/systemd/system/systemd-service.service

clean:
	rm -f ${OBJS}
	rm -f ${EXECS}
	rm -f *~

.phony: clean
