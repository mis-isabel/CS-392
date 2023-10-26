#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define PATH_MAX 4096

/* 
Isabel Sutedjo
I pledge my honor that I have abided by the Stevens Honor System.
*/

//check for valid inputs, taken from my pfind.c
int check(char arr[]){
	int x = 0;
    if (strlen(arr) == 9){
        while (x < 9){
            if (x == 0 || x == 3 || x == 6){
                if (arr[x] == 'r' || arr[x] == '-'){
                    x++;
                }
                else return 0;
            }
            else if (x == 1 || x == 4 || x == 7){
                if (arr[x] == 'w' || arr[x] == '-'){
                    x++;
                }
                else return 0;
            }
            else if (x == 2 || x == 5 || x == 8){
                if (arr[x] == 'x' || arr[x] == '-'){
                    x++;
                }
                else return 0;
            }
			x++;
        }
        return 1;
    }
    else{
        return 0;
    }
}

int main(int argc, char* argv[]){
    //error messages
    if (argc == 1){
        fprintf(stderr, "Usage: ./spfind <directory> <permissions string>\n");
        return EXIT_FAILURE;
    }
    if (!check(argv[2])){
        fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", argv[2]);
        return EXIT_FAILURE;
    }
    
    int pfindfd[2], sortfd[2];
    pid_t pfindpid, sortpid;
    char *pfind_args[] = {"pfind", argv[1], argv[2], NULL};
    char *sort_args[] = {"sort", NULL};
    int status;
    char buffer[PATH_MAX];
    int num_lines = 0;
    char character = 0;

    // creating first pipe
    if (pipe(pfindfd) < 0) {
        //fprintf(stdout, "waa");
        fprintf(stderr, "Error: pipe() failed.\n");
        return EXIT_FAILURE;
    }

    // pfind fork
    pfindpid = fork();
    if (pfindpid == -1) {
        fprintf(stderr, "Error: fork() failed.\n");
        return EXIT_FAILURE;
    }
    else if (pfindpid == 0) {
        // child redirects stdout to the write-end of the first pipe
        if(dup2(pfindfd[1], STDOUT_FILENO) == -1){
            fprintf(stderr, "Error: dup2() failed.\n");
            return EXIT_FAILURE;
        }
        // close unused file descriptors
        close(sortfd[0]);
        close(pfindfd[0]);
        close(sortfd[1]);
        // pfind
        if (execv(pfind_args[0], pfind_args) == -1) {
            fprintf(stderr, "Error: pfind failed.\n");
            return EXIT_FAILURE;
        }
    }

    // 2nd pipe
    if (pipe(sortfd) < 0) {
        fprintf(stderr, "Error: pipe() failed.\n");
        return EXIT_FAILURE;
    }        
    sortpid = fork();
    if(sortpid == -1) {
        fprintf(stderr, "Error: fork() failed.\n");
        return EXIT_FAILURE;
    } 
    else if(sortpid == 0){
        // sort-child process
        close(pfindfd[1]); // close read end of pipe
        if (dup2(pfindfd[0], STDIN_FILENO) == -1){ 
            fprintf(stderr, "Error: dup2 failed.\n");
            return EXIT_FAILURE;
        } 
        close(sortfd[0]); // close read end of pipe
        if (dup2(sortfd[1], STDOUT_FILENO) == -1){ 
            fprintf(stderr, "Error: dup2 failed.\n");
            return EXIT_FAILURE;
        } 
        if (execlp("sort", "sort", "-", NULL) == -1){
            fprintf(stderr, "Error: sort failed.\n");
            return EXIT_FAILURE;
        } 
    }
    
        else{
            // parent process
            close(sortfd[1]); // close read end of pipe
            dup2(sortfd[0], STDIN_FILENO); // redirect stdin to write to pipe
            close(pfindfd[1]); // close write end of pipe
            close(pfindfd[0]); // close read end of pipe

            // reads from the read-end of the second pipe until end-of-file
            while (1) {
                ssize_t bytes_read = read(sortfd[0], buffer, sizeof(buffer));
                if (bytes_read == -1) {
                    fprintf(stderr, "Error: read() failed.\n");
                    return EXIT_FAILURE;
                }
                if (bytes_read == 0) {
                    break;
                }
                // counts total matches (num of lines)
                for (int i = 0; i < bytes_read; i++) {
                    character = buffer[i];
                    if (buffer[i] == '\n') {
                        num_lines++;
                    }
                }
                if (write(STDOUT_FILENO, buffer, bytes_read) == -1){
                    fprintf(stderr, "Error: write() failed.\n");
                    return EXIT_FAILURE;
                }
            }
            printf("Total matches: %d\n", num_lines);
            if (waitpid(pfindpid, &status, 0) == -1) {
                fprintf(stderr, "Error: waitpid() failed.\n");
                return EXIT_FAILURE;
            }
            if (waitpid(sortpid, &status, 0) == -1) {
                fprintf(stderr, "Error: waitpid() failed.\n");
                return EXIT_FAILURE;
            }
        }
    return 0;
}