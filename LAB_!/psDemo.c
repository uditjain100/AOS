#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>

void printProcesses(int showAll, int showUser, int showNotTTY, char *username)
{
    struct dirent *file;
    DIR *dir = opendir("/proc");

    if (dir == 0)
    {
        printf("Error !!");
        exit(1);
    }

    while ((file = readdir(dir)) != NULL)
    {
        char *pid = file->d_name;

        int isNumber = 1;
        for (int i = 0; pid[i] != '\0'; i++)
            if (pid[i] < '0' || pid[i] > '9')
                isNumber = 1;

        if (isNumber == 0)
            continue;

        // Terminal Attached
        char statPath[256];
        snprintf(statPath, sizeof(statPath), "/proc/%s/stat", pid);

        FILE *statFile = fopen(statPath, "r");
        if (!statFile)
            continue;

        int tty_nr;
        fscanf(statFile, "%*d %*s %*c %*d %*d %*d %*d %d", &tty_nr);
        fclose(statFile);

        if (showNotTTY == 0 && tty_nr > 0)
            continue;

        // Owner
        char statusPath[500];
        snprintf(statusPath, sizeof(statusPath), "/proc/%s/status", pid);

        FILE *statusFile = fopen(statusPath, "r");
        if (!statusFile)
            continue;

        char status[10];
        uid_t uid;
        while (fgets(status, sizeof(status), statusFile))
        {
            if (strncmp(status, "Uid:", 4) == 0)
            {
                sscanf(status, "Uid: %u", &uid);
                break;
            }
        }
        fclose(statusFile);

        struct passwd *pw = getpwuid(uid);
        char *owner = pw ? pw->pw_name : NULL;

        if (showUser == 1)
            if (owner == NULL || strcmp(owner, username) != 0)
                continue;

        // Command Name
        char command[1000];

        char commandPath[500];
        snprintf(commandPath, sizeof(commandPath), "/proc/%s/comm", pid);

        FILE *commandFile = fopen(commandPath, "r");
        if (!commandFile)
            strcpy(command, "unknown");

        fgets(command, 256, commandFile);
        command[strcspn(command, "\n")] = 0;

        fclose(commandFile);

        printf("%s\t%s\t%s\n", pid, owner, command);
    }

    closedir(dir);
}

int main(int argc, char *argv[])
{
    int n = argc;

    int showAll = 0;
    int showUser = 0;
    int showNotTTY = 1;
    char *attr = n > 1 ? argv[1] : NULL;
    char *username = n > 2 ? argv[2] : NULL;

    if (n > 3)
    {
        printf("Invalid Command !!");
        return -1;
    }

    if (n > 1)
    {
        if ((strcmp(attr, "-a") == 0 || strcmp(attr, "-e") == 0) && n != 2)
        {
            printf("Invalid Command !!");
            return -1;
        }

        if (strcmp(attr, "-a") == 0 || strcmp(attr, "-e") == 0)
            showAll = 1;
        else if (strcmp(attr, "-u") == 0 && n == 3)
            showUser = 1;

        if (strcmp(attr, "-a") == 0)
            showNotTTY = 0;
    }

    printProcesses(showAll, showUser, showNotTTY, username);
    return 0;
}