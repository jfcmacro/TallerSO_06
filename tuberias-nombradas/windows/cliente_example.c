#include <windows.h>
#include <stdio.h>
#include <string.h>

#define PIPE_NAME   "\\\\.\\pipe\\half_duplex_pipe"
#define BUFFER_SIZE 256

int main(void) {
    HANDLE hPipe;
    DWORD  dwRead;
    char   buffer[BUFFER_SIZE];
    BOOL   success;

    printf("[Client] Connecting to pipe: %s\n", PIPE_NAME);

    /* Wait until the server creates the pipe */
    while (1) {
        hPipe = CreateFile(
            PIPE_NAME,
            GENERIC_READ,              /* half-duplex: read-only */
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (hPipe != INVALID_HANDLE_VALUE)
            break;

        if (GetLastError() != ERROR_PIPE_BUSY) {
            fprintf(stderr, "[Client] CreateFile failed: %lu\n", GetLastError());
            return 1;
        }

        /* Pipe busy, wait up to 5 seconds */
        if (!WaitNamedPipe(PIPE_NAME, 5000)) {
            fprintf(stderr, "[Client] WaitNamedPipe timed out.\n");
            return 1;
        }
    }

    /* Switch to message-read mode to match the server */
    /* DWORD dwMode = PIPE_READMODE_MESSAGE; */
    /* if (!SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL)) { */
    /*     fprintf(stderr, "[Client] SetNamedPipeHandleState failed: %lu\n", GetLastError()); */
    /*     CloseHandle(hPipe); */
    /*     return 1; */
    /* } */

    printf("[Client] Connected. Receiving messages...\n\n");

    do {
        success = ReadFile(hPipe, buffer, BUFFER_SIZE, &dwRead, NULL);

        if (!success && GetLastError() != ERROR_MORE_DATA)
            break;

        printf("[Client] Received: \"%s\"\n", buffer);

    } while (strcmp(buffer, "EXIT") != 0);

    CloseHandle(hPipe);
    printf("\n[Client] Done.\n");
    return 0;
}
