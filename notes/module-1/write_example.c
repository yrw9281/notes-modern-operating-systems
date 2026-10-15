#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    const char message[] = "Hi Class, it's a write() example\n";
    const size_t length = sizeof(message) - 1;
    size_t written = 0;

    while (written < length) {
        ssize_t result = write(STDOUT_FILENO,
                               message + written,
                               length - written);

        if (result == -1) {
            if (errno == EINTR) {
                continue;
            }
            perror("write");
            return 1;
        }
        if (result == 0) {
            fprintf(stderr, "write returned 0 before all bytes were written\n");
            return 1;
        }

        written += (size_t)result;
    }

    return 0;
}
