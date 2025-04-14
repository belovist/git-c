#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <direct.h>
#define mkdir _mkdir

// Function declarations
void init();
void commit(const char *filename);
void status();
void delete_last_commit();
void log_commits();
void create_readme();

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Please provide a command like: init, commit <file>, status, delete, log, readme\n");
        return 1;
    }

    if (strcmp(argv[1], "init") == 0) {
        init();
    } else if (strcmp(argv[1], "commit") == 0) {
        if (argc < 3) {
            printf(" Please provide a file to commit. Usage: commit myfile.txt\n");
            return 1;
        }
        commit(argv[2]);
    } else if (strcmp(argv[1], "status") == 0) {
        status();
    } else if (strcmp(argv[1], "delete") == 0) {
        delete_last_commit();
    } else if (strcmp(argv[1], "log") == 0) {
        log_commits();
    } else if (strcmp(argv[1], "readme") == 0) {
        create_readme();
    } else {
        printf("Unknown command: %s\n", argv[1]);
    }

    return 0;
}

// Initializes the repo
void init() {
    int res1 = mkdir(".mygit");
    int res2 = mkdir(".mygit\\commits");

    if (res1 == 0 || res2 == 0) {
        printf(" Repo initialized in .mygit\n");
    } else {
        printf(" Repo might already exist!\n");
    }
}

// Commit the specified file
void commit(const char *filename) {
    // Check if file exists first
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf(" File '%s' not found!\n", filename);
        return;
    }
    fclose(file);

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char foldername[100];

    strftime(foldername, sizeof(foldername), ".mygit\\commits\\commit_%Y%m%d_%H%M%S", t);
    _mkdir(foldername); // Create the commit folder

    // Build copy command
    char command[200];
    snprintf(command, sizeof(command), "copy %s %s > nul", filename, foldername);
    int result = system(command);

    if (result == 0) {
        printf(" Committed '%s' to %s\n", filename, foldername);
    } else {
        printf(" Failed to copy the file.\n");
    }
}

// Shows dummy status
void status() {
    printf(" Status: All files are fine (probably )\n");
}

// Delete the latest commit folder
void delete_last_commit() {
    FILE *fp;
    char command[512];
    char latest_commit[256] = "";

    fp = _popen("dir /B /O-D .mygit\\commits", "r");

    if (fp == NULL) {
        perror("Error reading commits");
        return;
    }

    if (fgets(latest_commit, sizeof(latest_commit), fp) == NULL) {
        printf("No commits to delete.\n");
        _pclose(fp);
        return;
    }

    latest_commit[strcspn(latest_commit, "\n")] = '\0'; // Remove newline
    _pclose(fp);

    snprintf(command, sizeof(command), "rmdir /S /Q .mygit\\commits\\%s", latest_commit);

    int result = system(command);

    if (result == 0) {
        printf(" Deleted last commit: %s\n", latest_commit);
    } else {
        printf(" Failed to delete commit.\n");
    }
}

// List commits in reverse order
void log_commits() {
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
    fprintf(f, "\nTANUSHREE KURALKAR- 24070122217\nVINEET KUMAR – 24070122234\nYASH SAMTANI – 24070122237\nYASHASVI AGARWAL - 24070122238 \n");
    fclose(f);
    printf(" README.txt created successfully!\n");
}