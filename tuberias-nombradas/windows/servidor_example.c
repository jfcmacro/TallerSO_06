#include <windows.h>
#include <stdio.h>

#define PIPE_NAME   "\\\\.\\pipe\\half_duplex_pipe"
#define BUFFER_SIZE 256

int main(void) {
    HANDLE hPipe;
    DWORD  dwWritten;
    char   messages[][BUFFER_SIZE] = {
        "Message 1: Hello from server",
        "Message 2: Half-duplex pipe demo",
        "Message 3: Server -> Client only",
        "EXIT",
    };

    printf("[Server] Creating named pipe: %s\n", PIPE_NAME);

    /*
     * PIPE_ACCESS_OUTBOUND  -> server can only write
     * Client must open with GENERIC_READ
     */
    hPipe = CreateNamedPipe(
        PIPE_NAME,
        PIPE_ACCESS_OUTBOUND,          /* half-duplex: write-only */
        PIPE_TYPE_MESSAGE |
        PIPE_READMODE_MESSAGE |
        PIPE_WAIT,
        1,                             /* max instances */
        BUFFER_SIZE,
        BUFFER_SIZE,
        0,
        NULL
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "[Server] CreateNamedPipe failed: %lu\n", GetLastError());
        return 1;
    }

    printf("[Server] Waiting for client to connect...\n");

    if (!ConnectNamedPipe(hPipe, NULL)) {
        DWORD err = GetLastError();
        if (err != ERROR_PIPE_CONNECTED) {
            fprintf(stderr, "[Server] ConnectNamedPipe failed: %lu\n", err);
            CloseHandle(hPipe);
            return 1;
        }
    }

    printf("[Server] Client connected. Sending messages...\n\n");

    for (int i = 0; i < 4; i++) {
        DWORD len = (DWORD)(strlen(messages[i]) + 1);

        if (!WriteFile(hPipe, messages[i], len, &dwWritten, NULL)) {
            fprintf(stderr, "[Server] WriteFile failed: %lu\n", GetLastError());
            break;
        }
        printf("[Server] Sent: \"%s\"\n", messages[i]);
    }

    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);

    printf("\n[Server] Done.\n");
    return 0;
}
