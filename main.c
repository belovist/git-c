#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include <direct.h>
#define mkdir _mkdir

void init();
void commit(const char *name);
void status();
void deletelastcommit();
void log();
void create_readme();

int main(int i, char *ch[]) {
    if (i < 2) {
        printf("Please provide a command like: init, commit <file>, status, delete, log, readme\n");
        return 1;
    }

    if (strcmp(ch[1], "init") == 0) {
        init();
    } else if (strcmp(ch[1], "commit") == 0) {
        if (i < 3) {
            printf(" Please provide a file to commit. Usage: commit myfile.txt\n");
            return 1;
        }
        commit(ch[2]);
    } else if (strcmp(ch[1], "status") == 0) {
        status();
    } else if (strcmp(ch[1], "delete") == 0) {
        deletelastcommit();
    } else if (strcmp(ch[1], "log") == 0) {
        log();
    } else if (strcmp(ch[1], "readme") == 0) {
        create_readme();
    } else {
        printf("Unknown command: %s\n", ch[1]);
    }

    return 0;
}

// Initializes the repo
void init() {
    int f1 = mkdir(".mygit");
    int f2 = mkdir(".mygit\\commits");

    if (f1 == 0 || f2 == 0) {
        printf(" Repo initialized in .mygit\n");
    } else {
        printf(" Repo might already exist!\n");
    }
}

// Commit the specified file
void commit(const char *name) {

    FILE *file = fopen(name, "r");
    if (!file) {
        printf(" File '%s' not found!\n", name);
        return;
    }
    fclose(file);

    time_t now = time(NULL);

    struct tm *t = localtime(&now);

    char foldername[100];

    char datepart[20];
    char timepart[20];

    strftime(datepart, sizeof(datepart), "%Y%m%d", t);
    strftime(timepart, sizeof(timepart), "%H%M%S", t);

    strcpy(foldername, ".mygit\\commits\\commit_");
    strcat(foldername, datepart);
    strcat(foldername, "_");
    strcat(foldername, timepart);

    _mkdir(foldername);

    char cmd[200];
    strcpy(cmd, "copy ");
    strcat(cmd, name);
    strcat(cmd, " ");
    strcat(cmd, foldername);

    int r = system(cmd);

    if (r == 0) {
        printf(" Committed '%s' to %s\n", name, foldername);
    } else {
        printf(" Failed to copy the file.\n");
    }

}

// doesnt do anything
void status() {
    printf(" Status: All files are fine (probably)\n");
}

// Delete the latest commit folder
void deletelastcommit() {
    FILE *fp;
    char cmd[512];
    char commit[256] = "";

    fp = _popen("dir /B /O-D .mygit\\commits", "r");

    if (fp == NULL) {
        perror("Error reading commits");
        return;
    }

    if (fgets(commit, sizeof(commit), fp) == NULL) {
        printf("No commits to delete.\n");
        _pclose(fp);
        return;
    }

    size_t newline_index = strcspn(commit, "\n");
    commit[newline_index] = '\0';

    _pclose(fp);

    strcpy(cmd, "rmdir /S .mygit\\commits\\");
    strcat(cmd, commit);

    int r = system(cmd);

    if (r == 0) {
        printf(" Deleted last commit: %s\n", commit);
    } else {
        printf(" Failed to delete commit.\n");
    }

}

void log() {
    printf(" Commit Log:\n");
    system("dir /B /O-D .mygit\\commits");
}

// Create a README.txt with usage
void create_readme() {
    FILE *f = fopen("README.txt", "w");

    if (f == NULL) {
        printf(" Couldn't create README.txt\n");
        return;
    }

    fprintf(f, "MyGit - A Mini Git\n");
    fprintf(f, "----------------------------------------------\n");
    fprintf(f, "Commands:\n");
    fprintf(f, "1. init           - Make a new repo (only once!)\n");
    fprintf(f, "2. commit <file>  - Save your work snapshot\n");
    fprintf(f, "3. delete         - Remove the last commit\n");
    fprintf(f, "4. log            - See all your commits\n");
    fprintf(f, "5. status         - Dummy status (just vibes)\n");
    fprintf(f, "6. readme         - Generate this help file\n");
    fprintf(f, "\nbye bye!! \n");
    fprintf(f, "\nwith love from \n");
    fprintf(f, "\nTANUSHREE KURALKAR- 24070122217\nVINEET KUMAR – 24070122234\nYASH SAMTANI – 24070122237\nYASHASVI AGARWAL-24070122238 \n");
    fclose(f);
    printf(" README.txt created successfully!\n");
}