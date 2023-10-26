#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>

/* 
Isabel Sutedjp
I pledge my honor that I have abided by the Stevens Honor System.
*/

void find(char* dir, int perm){
    DIR *dp = opendir(dir);
    struct dirent *dirp;
    struct stat finfo; 
    char direct[2048];

    //opens directory, if failed throw error
    if (dp == NULL){
        fprintf(stderr, "Cannot open directory '%s'. Permission denied.\n", dir);
        exit(EXIT_FAILURE);
    }

    //checks directory streams
    while((dirp = readdir(dp)) != NULL) {
        bool d = strcmp(dirp -> d_name, ".");
        bool d2 =strcmp(dirp -> d_name, "..");
        if ( d == 0 || d2 == 0){
            continue;
        }
        else {
            strcpy(direct, dir);
            strcat(direct, "/");
            strcat(direct, dirp -> d_name);
            stat(direct, &finfo);
            if (S_ISDIR(finfo.st_mode) != 0) {
                find(direct, perm);
            }
            else {
                if(((S_IRWXG | S_IRWXO | S_IRWXU) & finfo.st_mode) == perm){ 
                    printf("%s\n", direct);
                }
            }   
        }
    }
    closedir(dp);
}

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

// checks permissions from string to int
int string_int(char* arr){
    int perm = 0;
	int x = 0;
    while (x < 9){
        if (arr[x] == '-'){
                perm <<= 1;
            }
        else{
            perm <<= 1;
            perm |= 1;
        }
		x++;
    }
    return perm;
}

int main(int argc, char *argv[]){
    //checking for valid strings
    if (!check(argv[2])){
        fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", argv[2]);
        return EXIT_FAILURE;
    }
    else{
        char direct[2048];
        getcwd(direct, 2048);
        strcat(direct, "/");
        strcat(direct, argv[1]);
        find(direct, string_int(argv[2]));
    }

    return 0;
}
