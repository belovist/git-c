#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

// Function declarations
void init();
void commit();
void status();
void delete_last_commit();
void log_commits();
void create_readme();

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Please provide a command like: init, commit, status, delete, log, readme\n");
        return 1;
    }

    // Checking which command was given
    if (strcmp(argv[1], "init") == 0) {
        init();
    } else if (strcmp(argv[1], "commit") == 0) {
        commit();
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

// Create the hidden .mygit folder and commits subfolder
void init() {
    int res1 = mkdir(".mygit");
    int res2 = mkdir(".mygit/commits");

    if (res1 == 0 || res2 == 0) {
        printf("✅ Repo initialized in .mygit\n");
    } else {
        printf("⚠️ Repo might already exist!\n");
    }
}

// Commit creates a folder with the current time as its name
void commit() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char foldername[100];

    strftime(foldername, sizeof(foldername), ".mygit/commits/commit_%Y%m%d_%H%M%S", t);

#ifdef _WIN32
    _mkdir(foldername);
#else
    mkdir(foldername, 0700);
#endif

    printf("📦 Committed changes to %s\n", foldername);
}

// Dummy status command
void status() {
    printf("📂 Status: All files are fine (I guess)\n");
}

// Delete the most recent commit
void delete_last_commit() {
    FILE *fp;
    char command[512];
    char latest_commit[256] = "";

#ifdef _WIN32
    fp = _popen("dir /B /O-D .mygit\\commits", "r");
#else
    fp = popen("ls -1t .mygit/commits", "r");
#endif

    if (fp == NULL) {
        perror("Error reading commits");
        return;
    }

    if (fgets(latest_commit, sizeof(latest_commit), fp) == NULL) {
        printf("❌ No commits to delete.\n");
#ifdef _WIN32
        _pclose(fp);
#else
        pclose(fp);
#endif
        return;
    }

    latest_commit[strcspn(latest_commit, "\n")] = '\0'; // remove newline

#ifdef _WIN32
    _pclose(fp);
    snprintf(command, sizeof(command), "rmdir /S /Q .mygit\\commits\\%s", latest_commit);
#else
    pclose(fp);
    snprintf(command, sizeof(command), "rm -rf .mygit/commits/%s", latest_commit);
#endif

    int result = system(command);

    if (result == 0) {
        printf("🗑️ Deleted last commit: %s\n", latest_commit);
    } else {
        printf("⚠️ Failed to delete commit.\n");
    }
}

// Show all commits in order
void log_commits() {
    printf("📜 Commit Log:\n");

#ifdef _WIN32
    system("dir /B /O-D .mygit\\commits");
#else
    system("ls -1t .mygit/commits");
#endif
}

// Generates a README.txt file
void create_readme() {
    FILE *f = fopen("README.txt", "w");

    if (f == NULL) {
        printf(" Couldn't create README.txt\n");
        return;
    }

    fprintf(f, "MyGit - A Mini Git\n");
    fprintf(f, "----------------------------------------------\n");
    fprintf(f, "Commands:\n");
    fprintf(f, "1. init    - Make a new repo (only once!)\n");
    fprintf(f, "2. commit  - Save your work snapshot\n");
    fprintf(f, "3. delete  - Remove the last commit\n");
    fprintf(f, "4. log     - See all your commits\n");
    fprintf(f, "5. status  - Dummy status (just vibes)\n");
    fprintf(f, "6. readme  - Generate this help file\n");
    fprintf(f, "\nbye bye!! 🧠🧃\n");

    fclose(f);
    printf("📄 README.txt created successfully!\n");
}
