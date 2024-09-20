#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

void cat_work(char *fileName) {

	FILE *file = fopen(fileName, "r");

        int ch;
        while((ch = fgetc(file)) != EOF)
		printf("%c", ch);

        fclose(file);
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

	char cwd[1000];
	getcwd(cwd, sizeof(cwd));

	for(int i = 1; i < n; i++){

		char *fileName = argv[i];

		if(isFilePresent(fileName, cwd) == 0) {
			printf("No file found named : %s \n", fileName);
			continue;
		}

		cat_work(fileName);
	}
}
