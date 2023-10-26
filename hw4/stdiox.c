#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include "stdiox.h"
/*Isabel Sutedjo
I pledge my honor that I have abided by the Stevens Honor System.*/

/*reverse a string, needed to make itoa function*/
void revSt(char* str){
    int len = strlen(str);
    for (int x = 0; x < (len/2); x++){
        char temp = str[x];
        str[x] = str[len - x - 1];
        str[len - x - 1] = temp;
    }
}

/*need to make my own itoa function to convert integer to string*/
char* itoa_new(int num, char* str){
    int lsdex;
    int index = 0;

    //goes through each digit and reassigns values to dex
    while (num > 0){
        lsdex = num % 10;
        str[index] = (char)(lsdex) + 48;
        num /= 10;
        index++;
    }
    str[index] = '\0';
    return str;
}
/*accounts for negative numbers*/
char* cnv(int num, char* str){
    if (num == 0){
        return "0";
    }
    else if (num > 0){
        str = itoa_new(num, str);
    }
    else {
        str = itoa_new(num * -1, str);
        char negative[] = "-";
        strcat(str, negative);
    }
    revSt(str);
    return str;
}

int fprintfx(char* filename, char format, void* data){
    //if null, return -1 and sets errno to EIO
    if (!data){
        errno = EIO;
        return -1;
    }

    if (strcmp(filename, "") == 0){
        if (format == 'd'){
            char str[128];
            cnv(*((int*)data), str);
            write(1, str, strlen(str));
            write(1, "\n", 1);
        }
        else if (format == 's'){
            write(1, data, strlen(data));
            write(1, "\n", 1);
        }
        else{
            errno = EIO;
            return -1;
        }
    }
    else{
        int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR |S_IWUSR | S_IRGRP);
        if (format == 's'){
            write(fd, data, strlen(data));
            write(fd, "\n", 1);
        }
        else if (format == 'd'){
            char str[128];
            cnv(*((int*)data), str);
            write(fd, str, strlen(str));
            write(fd, "\n", 1);
        }
        else{
            errno = EIO;
            return -1;
        }
    }
    return 0;
}

int fscanfx(char* filename, char format, void* data){
    int filedirec;
    char tmp;
    int length = strlen(filename);
    char *buffer = malloc(128);
    int index = 0;
    int current = 128;
    memset(buffer, 0, current);
    int p = 0;

    if (data == NULL){
        errno = EIO;
        return -1;
    }
    //checking for empty file
    if (length == 0){
        filedirec = 0;
    }
    else if (strcmp(filename, "") == 0){
        if (format != 'd' && format != 's'){
            errno = EIO;
            return -1;
        }
    }
    //checking for existing file
    else if (access(filename, F_OK) == -1){
        errno = ENOENT;
        return -1;
    }

    else{
        if (format != 'd' && format != 's'){
            free(buffer);
            errno = EIO;
            return -1;
        }
        char *path = (char *)malloc(PATH_MAX);
        realpath(filename, path);
        struct stat temp;
        stat(path, &temp);
        ino_t inode = temp.st_ino;

        //check fd and compare inode nums
        char pid[PATH_MAX];
        itoa_new(getpid(), pid);
        int count = 5 + strlen("/proc/") + strlen(pid) + strlen("/fd/");
        char *direc = (char *)malloc(count);

        strcpy(direc, "/proc/");
        strcat(direc, pid);
        strcat(direc, "/fd/");

        //gets inode number of every file
        DIR *dp = opendir(direc);

        int openFiles = 0;

        //open file
        struct dirent *dirp;
        while ((dirp = readdir(dp)) != NULL){
            if (strcmp(dirp -> d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0){
                //checks for being out of range
                if (atoi(dirp -> d_name) >= 3){
                    if (atoi(dirp -> d_name) == 1024){
                        break;
                    }
                    struct stat fi;
                    fstat(atoi(dirp -> d_name), &fi);
                    ino_t inode_ = fi.st_ino;
                    if (inode_ = inode){
                        filedirec = atoi(dirp -> d_name);
                        openFiles = 1;
                        break;
                    }
                }
            }
        }

        //freeing
        closedir(dp);
        free(direc);
        //if not opened, open
        if (openFiles == 0){
            filedirec = open(path, O_RDONLY);
        }
        free(path);
        //end of file?
        if (read(filedirec, &tmp, 1) != 0){
            lseek(filedirec, -1, SEEK_CUR);
        }
        return -1;
    }

    while(read(filedirec, &tmp, 1) != 0){
        if (tmp == '\n'){
            break;
        }
        if (index == current){
            char *newbuf = malloc(current + 128);
            memset(newbuf, 0, current);
            for (size_t j = 0; j < current; j++){
                newbuf[j] = buffer[j];
            }
            free(buffer);
            buffer = newbuf;
            current += 128;
        }
        buffer[index] = tmp;
        index++;
    }
    while (((char *)data)[p] != 0){
        ((char *)data)[p] = 0;
        p++;
    }
    if (format != 's' && format != 'd'){
        free(buffer);
        errno = EIO;
        return -1;
    }
    if (format == 's'){
        for (int x = 0; x < index; x++){
            ((char *)data)[x] = buffer[x];
        }
        free(buffer);
        return 0;
    }
    *(int *)data = atoi(buffer);
    free(buffer);
    return 0;
}

int clean(){
    DIR* dp;
    int fd;
    struct dirent* dirp;
    char* path = "/proc/self/fd";
    dp = opendir(path);

    while((dirp=readdir(dp)) != NULL){
        //fd number
        fd = atoi(dirp->d_name);

        //checks for open files
        if (fd < 0){
            errno = EIO;
            return -1;
        }
        else if (fd > 4 && fd < 1024){
            close(fd);
        }
    }
    closedir(dp);
    return 0;
}
