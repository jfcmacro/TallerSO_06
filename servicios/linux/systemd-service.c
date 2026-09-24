#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
char syslog_buffer[BUFFER_SIZE];
void manejador_sigterm(int signo);

int main(int argc, char const *argv[]) {
  
  int running = 0;

  struct sigaction viejo_sa;
  struct sigaction nuevo_sa;
  
  nuevo_sa.sa_handler = manejador_sigterm;
  sigemptyset(&nuevo_sa.sa_mask);
  nuevo_sa.sa_flags = 0;

  sigaction(SIGTERM, &nuevo_sa, &viejo_sa);
  

  openlog("systemd-service", LOG_CONS | LOG_ERR | LOG_PID, LOG_USER);

  for(;;) {
    bzero(syslog_buffer, BUFFER_SIZE);
    sprintf(syslog_buffer, "running step: %d", running++);
    syslog(LOG_INFO | LOG_USER, syslog_buffer);
    sleep(60);
  }

  return EXIT_SUCCESS;
}

void
manejador_sigterm(int signo) {

  sprintf(syslog_buffer, "Receiving signal: %d ending", signo);
  syslog(LOG_INFO | LOG_USER, syslog_buffer);
  exit(0);
}
