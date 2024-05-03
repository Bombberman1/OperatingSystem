#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#define PIPE_DATA_SIZE 16



int main() {
    int pfd[2];
    if(pipe(pfd) < 0) {
        return 0;
    }

    pid_t p = fork();
    if(p > 0) {
        signal(SIGINT, SIG_IGN);
        DIR *dir;
        struct dirent *entry;

        char folder[] = "Parent_Folder";
        dir = opendir(folder);
        while((entry = readdir(dir)) != NULL) {
            if (strcmp(entry -> d_name, ".") == 0 || strcmp(entry -> d_name, "..") == 0) {
                continue;
            }
            write(pfd[1], entry -> d_name, PIPE_DATA_SIZE);
            sleep(1);
        }

        closedir(dir);
        close(pfd[1]);
    } else if(p == 0) {
        signal(SIGINT, SIG_IGN);
        int bytes = -1;
        char read_buffer[256];
        while(bytes) {
            bytes = read(pfd[0], read_buffer, PIPE_DATA_SIZE);
            if(bytes == -1) break;
            read_buffer[PIPE_DATA_SIZE] = 0;

            char path[40] = "Parent_Folder/";
            unsigned char start_add = 14;
            for(unsigned char i = 0; i < PIPE_DATA_SIZE; i++) {
                path[start_add + i] = read_buffer[i];
            }
            path[start_add + PIPE_DATA_SIZE] = 0;
            char file_buffer[1025];
            int fd = open(path, O_RDONLY);
            
            if(fd != -1) {
                int data = -1;
                while(data) {
                    data = read(fd, file_buffer, 1024);
                    if(data == -1) break;
                    file_buffer[data] = 0;
                    puts(file_buffer);
                }
                printf("%s size: %ld bytes\n\n", path, lseek(fd, 0, SEEK_END));
            }

            sleep(1);
            close(fd);
        }
        
        close(pfd[0]);
    }

    return 0;
}
