#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

void rmWork(char *fileName, int prompt) {

    if (prompt == 1) {
        char res[4];
        printf("Do you want to remove file '%s'? [y/n] ", fileName);
        if (fgets(res, sizeof(res), stdin) != NULL) {
            if (res[0] == 'y' || res[0] == 'Y') {
                if (remove(fileName) != 0)
                    printf("Error in deleting file !! ");
            } 
        } else printf("Invalid input.\n");
    } else {
        if (remove(fileName) != 0)
            printf("Error in deleting file !! ");
    }

}

int isFilePresent(char *fileName, char *path) {
    DIR *dir = opendir(path);
    struct dirent *file;
    while ((file = readdir(dir)) != NULL)
        if(strcmp(file->d_name, fileName) == 0)
            return 1;
    return 0;
}

int main(int argc, char *argv[]) {
    int n = argc;

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    int prompt = 0;
    int si = 1;

    if(n < 2) {
        printf("Invalid command !!! ");
        return -1;
    } else {

        if(strcmp(argv[1], "-i") == 0) {
            prompt = 1;
            si = 2;
        } 

        for(int i = si; i < argc; i++) {
            char *fileName = argv[i];
            if(isFilePresent(fileName, cwd) == 0) {
                printf("No File found named %s !!! \n", fileName);
                return -1;
            }

            rmWork(fileName, prompt);
        }


    }
    return 0;
}