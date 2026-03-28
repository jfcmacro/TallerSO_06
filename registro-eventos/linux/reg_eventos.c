#include <syslog.h>
#include <unistd.h>

int
main(int argc, char const *argv[]) {

  openlog("reg_eventos", LOG_CONS | LOG_ERR | LOG_PID, LOG_USER);

  syslog(LOG_INFO | LOG_USER, "Iniciar");
  syslog(LOG_INFO | LOG_USER, "Dormir");
  sleep(5);
  syslog(LOG_INFO | LOG_ERR,  "Terminado");

  closelog();
}
