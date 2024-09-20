#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

long wc_work(char *fileName, char *cwd, int num) {
	
	FILE *file = fopen(fileName, "r");
	
	int ch;
	long chars = 0;
	long words = 0;
	long lines = 0;

	while((ch = fgetc(file)) != EOF) {
		chars++;
		if(ch == '\n') lines++;
		if(ch == ' ') words++;
	}

	fclose(file);

	if(num == 0) printf("%ld ", chars);
	if(num == 1) printf("%ld ", lines);
	if(num == 2) printf("%ld ", words);

	printf("%s \n", fileName);

	if(num == 0) return chars;
	if(num == 1) return lines;
	if(num == 2) return words;
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

	long count = 0;
	for(int i = 2; i < n; i++) {
		char *fileName = argv[i];
		if (strcmp(argv[1], "-c") == 0) {
            		if (isFilePresent(fileName, cwd) == 0) {
                		printf("No File found named %s !!! \n", fileName);
                		continue;
            		}
            		count += wc_work(fileName, cwd, 0);
        	} else if (strcmp(argv[1], "-l") == 0) {
            		if (isFilePresent(fileName, cwd) == 0) {
                		printf("No File found named %s !!! \n", fileName);
                		continue;
            		}
            		count += wc_work(fileName, cwd, 1);
        	} else if (strcmp(argv[1], "-w") == 0) {
            		if (isFilePresent(fileName, cwd) == 0) {
                		printf("No File found named %s !!! \n", fileName);
                		continue;
            		}
            		count += wc_work(fileName, cwd, 2);
        	} else {
            		printf("Invalid Command !!!");
            		return -1;
        	}
	} 

	printf("%ld total \n", count);

	return 0;
}
