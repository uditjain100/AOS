#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

void ls_a_work(char *path, int hidden) {
    DIR *dir = opendir(path);
    struct dirent *file;
    while ((file = readdir(dir)) != NULL) {
        if(hidden == 0 && file->d_name[0] == '.')
            continue;
        printf("%s\t", file->d_name);
    }
}

void ls_l_work(char *path, int hidden) {
    DIR *dir = opendir(path);
    struct dirent *file;
    while ((file = readdir(dir)) != NULL) {
        if(hidden == 0 && file->d_name[0] == '.')
            continue;

        struct stat file_stat;
        stat(path, &file_stat);

        // file type
        printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
        printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
        printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
        printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
        printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
        printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
        printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
        printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
        printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
        printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
        
        // soft links
        printf(" %lu", (unsigned long)file_stat.st_nlink);
        
        // owner
        struct passwd *pw = getpwuid(file_stat.st_uid);
        printf(" %s", pw ? pw->pw_name : "???");
        
        // group
        struct group *gr = getgrgid(file_stat.st_gid);
        printf(" %s", gr ? gr->gr_name : "???");
        
        // size
        printf(" %ld", (long)file_stat.st_size);
        
        // modification time
        char timebuf[64];
        struct tm *timeinfo = localtime(&file_stat.st_mtime);
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", timeinfo);
        printf(" %s", timebuf);
        
        // file name
        printf(" %s\n", file->d_name);
    }
}

int main(int argc, char *argv[]) {
    int n = argc;

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    if(n == 1)
        ls_a_work(cwd, 0);
    else if(n == 2) {
        if(strcmp(argv[1], "-l") == 0)
            ls_l_work(cwd, 0);
        else if(strcmp(argv[1], "-a") == 0)
            ls_a_work(cwd, 1);
        else {
            printf("Invalid Attribute !!! ");
            return -1;
        }
    } else {
        printf("Invalid Attribute !!! ");
        return -1;
    }
    return 0;
}