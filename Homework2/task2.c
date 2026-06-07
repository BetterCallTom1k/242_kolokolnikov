#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUF_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 2) {
        write(STDERR_FILENO, "Usage: task2 <file>\n", 20);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        write(STDERR_FILENO, "Error: cannot open file\n", 24);
        return 1;
    }

    struct stat st;
    fstat(fd, &st);

    lseek(fd, st.st_size / 2, SEEK_SET);

    char rbuf[BUF_SIZE];
    char wbuf[BUF_SIZE];
    int  wpos  = 0;
    int  state = 0;
    ssize_t n;

    while (state < 2 && (n = read(fd, rbuf, BUF_SIZE)) > 0) {
        for (int i = 0; i < n && state < 2; i++) {
            char c = rbuf[i];
            if (state == 0) {
                if (c == '\n') state = 1;
            } else {
                wbuf[wpos++] = c;
                if (wpos == BUF_SIZE) {
                    write(STDOUT_FILENO, wbuf, wpos);
                    wpos = 0;
                }
                if (c == '\n') state = 2;
            }
        }
    }

    if (wpos > 0)
        write(STDOUT_FILENO, wbuf, wpos);

    close(fd);
    return 0;
}
