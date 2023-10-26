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

#define BLUE "\x1b[34;1m"
#define DEFAULT "\x1b[0m"
#define PATH_MAX 4096

/* 
Isabel Sutedjo
I pledge my honor that I have abided by the Stevens Honor System.
*/

//checks if the signal has been sent
volatile sig_atomic_t interrupted = 0;
void sig_handler(int sig){
    interrupted = sig;
    write(1, "\n", 1);
}

int main(){
    char input[PATH_MAX];
    char *command;
    char *arg = NULL;
    char *home_dir;
    struct passwd *pw = getpwuid(getuid());
    char* cwd = (char *)malloc(PATH_MAX);
    char** m_argv = malloc(PATH_MAX * sizeof(char*));

    //error checking mallocs
    if (m_argv == NULL){
        fprintf(stderr, "Error: malloc() failed. %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (cwd == NULL){
        fprintf(stderr, "Error: malloc() failed. %s.\n", strerror(errno));
        free(m_argv);
        exit(EXIT_FAILURE);
    } 

    while (1) {
        //track signal
        struct sigaction action;
        action.sa_handler = sig_handler;
        if (sigaction(SIGINT, &action, NULL) < 0){
            fprintf(stderr, "Error: Cannot register signal handler. %s.\n", strerror(errno));
            continue;
        } 

        // Get current working directory
        if (getcwd(cwd, PATH_MAX) == NULL) {
            fprintf(stderr, "Error: Cannot get current working directory. %s.\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        //prints the blue path
        printf("[%s%s%s]>", BLUE, cwd, DEFAULT);
        //clearing stdout
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            if (interrupted == SIGINT) {
                interrupted = 0;
                continue;
            }
            fprintf(stderr, "Error: Failed to read from stdin. %s.\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        if (strcmp(input,"\n\0") == 0) {
            continue;
        }

        if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        }

        // tokenize user input
        int argc = 0;
        char* token = strtok(input, " ");
        while (token != NULL) {
            if (argc == 0) {
                command = token;}
            else {
                arg = token;
            }
            m_argv[argc] = token;
            token = strtok(NULL, " ");
            argc++;
        }
        m_argv[argc] = NULL;

        //error checks for password entry
        if (pw == NULL){
            fprintf(stderr, "Error: Cannot get passwd entry. %s.\n", strerror(errno));
            free(m_argv);
            free(cwd);
            exit(EXIT_FAILURE);
        }
    //check if command is exit or cd
    else if (strcmp(command, "exit") == 0) {
        free(cwd);
        free(m_argv);
        exit(EXIT_SUCCESS);
    }
    else if (strcmp(command, "cd") == 0) {
    //if no argument is given, change to home directory
    home_dir = pw->pw_dir;
    if (argc == 1) {
        if (chdir(home_dir) == -1) {
            fprintf(stderr, "Error: Cannot change directory to '%s'. %s.\n", home_dir, strerror(errno));
        }
    }
    else if (argc == 2) {
        if (strcmp(arg, "~") == 0){
            arg = home_dir;
        }
        if (chdir(arg) == -1) {
            fprintf(stderr, "Error: Cannot change directory to '%s'. %s.\n", arg, strerror(errno));
        }
    }
    else {
        fprintf(stderr, "Error: Too many arguments to cd.\n");
    }
    continue;
    }

    // create a child process
    pid_t pid = fork();

    if (pid == -1) {
        fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
        continue;
    }

    if (pid == 0) {
        // child process
        if (execvp(command, m_argv) == -1) {
            fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    else {
        // parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            fprintf(stderr, "Error: wait() failed. %s.\n", strerror(errno));
            continue;
        }
    }

    interrupted = 0; //reset signal flag
}
return 0;
}

        