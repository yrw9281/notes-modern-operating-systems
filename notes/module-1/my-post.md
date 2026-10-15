# Module 1 Discussion: System Calls

## Main idea

A system call is how a program asks the OS kernel to perform an operation that needs privileged access. The OS provides abstractions for programs and manages computer resources. User programs cannot directly access hardware or kernel memory, so their requests go through the kernel. The kernel checks each request and its permissions, which helps isolate programs and gives them a consistent way to access files and devices.

## How a system call works

The mechanics depend on the machine. In the `read()` example, a C program calls a library procedure. The procedure puts the system-call number where the OS expects it, then executes a `TRAP` (or dedicated instructions like `syscall` on modern architectures). This switches execution from user mode to kernel mode. The kernel examines the parameters, dispatches the request to the right handler, and carries it out. Control then returns through the library procedure to the program.

For `read(fd, buffer, nbytes)`, the return value is the number of bytes read. It is usually `nbytes`, but can be smaller at the end of a file. If an invalid parameter or disk error occurs, the library wrapper returns `-1` and sets `errno`. A program should check the result.

## Example: `write()`

I chose `write()` for my C example. It is a POSIX file-management call that writes data from a buffer into a file.

```c
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

        written += (size_t)result;
    }

    return 0;
}
```

## Result

Run the executable file. It prints the message and exits with status `0`. 

Closing standard output with `1>&-` makes `write()` fail; the error is printed to standard error and the program exits with status `1`.

## Conclusion

I find it interesting that a system call can leave a process waiting. If a process is reading from the keyboard and no input has been typed, it blocks while the OS checks whether another process can run. Once input is available, the waiting process resumes.

I understand the request-and-return process best through the `write()` example. Each call returns a byte count, and my loop uses it to update `written` before continuing. That makes the idea less abstract to me: my program requests a service, then uses the result the OS returns.
