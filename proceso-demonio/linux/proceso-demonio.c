#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int
inicializar_proceso_demonio(void);

int
main(int argc, char* const argv[]) {
  fprintf(stdout, "process id: %d\n", getpid());
  // sleep(40);
  fprintf(stdout, "convirtiendo en proceso demonio\n");
  if (inicializar_proceso_demonio() == -1) {
    fprintf(stderr, "Fallo crear proceso demonio\n");
  }
  for(;;);
  return EXIT_SUCCESS;
}

int
inicializar_proceso_demonio(void) {
  pid_t pid;

  if ((pid = fork()) < 0) {
    return -1;
  }
  else if (pid != 0) {
    _exit(EXIT_SUCCESS);
  }

  pid_t sid;
  
  if ((sid = setsid()) < 0) {
    fprintf(stderr, "Error creando sesion setid() %s %d\n",
	    strerror(errno), errno);
    _exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
      fprintf(stderr, "Error al mover al directorio chdir() %s %d\n",
	      strerror(errno), errno);
      _exit(EXIT_FAILURE);
  }

  umask(0);

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDOUT_FILENO);

  return EXIT_SUCCESS;
}

