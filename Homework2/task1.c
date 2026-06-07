/*
 * Print N lines from a file starting at line I.
 * Usage: ./task1 <file> <I> <N>
 * Uses only open/close/read/write; buffers I/O at 4096 bytes.
 */
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 4) {
        write(STDERR_FILENO, "Usage: task1 <file> <start_line> <num_lines>\n", 45);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        write(STDERR_FILENO, "Error: cannot open file\n", 24);
        return 1;
    }

    int start = atoi(argv[2]);
    int end   = start + atoi(argv[3]); /* first line NOT to print */

    char rbuf[BUF_SIZE];
    char wbuf[BUF_SIZE];
    int  wpos     = 0;
    int  cur_line = 1;
    ssize_t n;

    while (cur_line < end && (n = read(fd, rbuf, BUF_SIZE)) > 0) {
        for (int i = 0; i < n && cur_line < end; i++) {
            char c = rbuf[i];

            if (cur_line >= start) {
                wbuf[wpos++] = c;
                if (wpos == BUF_SIZE) {
                    write(STDOUT_FILENO, wbuf, wpos);
                    wpos = 0;
                }
            }

            if (c == '\n')
                cur_line++;
        }
    }

    if (wpos > 0)
        write(STDOUT_FILENO, wbuf, wpos);

    close(fd);
    return 0;
}
