#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <direct.h>

#define mkdir _mkdir

typedef struct CommitNode {
    char name[256];
    struct CommitNode *next;
} CommitNode;

CommitNode *head = NULL;

void init();
void commit_file(const char *filename);
void status();
void delete_last_commit();
void show_log();
void create_readme();
void load_commits();
void save_commit(const char *foldername);
void clear_commits();
void help();

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: mygit <command> [file]\n");
        return 1;
    }

    load_commits();

    if (strcmp(argv[1], "init") == 0) {
        init();
    } else if (strcmp(argv[1], "commit") == 0) {
        if (argc < 3) {
            printf("Specify a file to commit.\n");
            return 1;
        }
        commit_file(argv[2]);
    } else if (strcmp(argv[1], "status") == 0) {
        status();
    } else if (strcmp(argv[1], "delete") == 0) {
        delete_last_commit();
    } else if (strcmp(argv[1], "log") == 0) {
        show_log();
    } else if (strcmp(argv[1], "readme") == 0) {
        create_readme();
    } else if (strcmp(argv[1], "help") == 0) {
        help();
    } else {
        printf("Unknown command.\n");
        help();
    }

    clear_commits();
    return 0;
}

void init() {
    int created1 = mkdir(".mygit");
    int created2 = mkdir(".mygit\\commits");
    FILE *fp = fopen(".mygit\\commits_list.txt", "w");
    if (fp) fclose(fp);

    if (created1 == 0 || created2 == 0) {
        printf("Repository initialized successfully.\n");
    } else {
        printf("Repository already exists.\n");
    }
}

void commit_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("File '%s' not found.\n", filename);
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

    sprintf(foldername, ".mygit\\commits\\commit_%s_%s", datepart, timepart);

    _mkdir(foldername);

    char cmd[300];
    sprintf(cmd, "copy \"%s\" \"%s\" >nul", filename, foldername);
    int result = system(cmd);

    if (result == 0) {
        save_commit(foldername);
        printf("File '%s' committed successfully.\n", filename);
    } else {
        printf("Failed to commit file.\n");
    }
}

void status() {
    printf("Current status:\n");
    CommitNode *temp = head;
    int count = 0;
    while (temp) {
        count++;
        temp = temp->next;
    }
    printf("Total commits: %d\n", count);
}

void delete_last_commit() {
    if (!head) {
        printf("No commits available to delete.\n");
        return;
    }

    FILE *fp = fopen(".mygit\\commits_list.txt", "w");
    if (!fp) {
        printf("Unable to open commit list.\n");
        return;
    }

    CommitNode *temp = head;
    CommitNode *prev = NULL;

    while (temp->next) {
        prev = temp;
        temp = temp->next;
    }

    char cmd[512];
    sprintf(cmd, "rmdir /S /Q \"%s\"", temp->name);
    system(cmd);

    if (prev) {
        prev->next = NULL;
    } else {
        head = NULL;
    }

    CommitNode *current = head;
    while (current) {
        fprintf(fp, "%s\n", current->name);
        current = current->next;
    }
    fclose(fp);

    free(temp);

    printf("Last commit deleted successfully.\n");
}

void show_log() {
    printf("Commit History:\n");
    CommitNode *temp = head;
    int count = 1;
    while (temp) {
        printf("%d. %s\n", count++, temp->name);
        temp = temp->next;
    }
}

void create_readme() {
    FILE *f = fopen("README.txt", "w");
    if (!f) {
        printf("Failed to create README.txt\n");
        return;
    }

    fprintf(f, "MyGit - A Simple Version Control System\n");
    fprintf(f, "----------------------------------------\n");
    fprintf(f, "Commands:\n");
    fprintf(f, "init           - Initialize a new repository\n");
    fprintf(f, "commit <file>  - Commit a file\n");
    fprintf(f, "status         - Show repository status\n");
    fprintf(f, "delete         - Delete the latest commit\n");
    fprintf(f, "log            - View commit history\n");
    fprintf(f, "readme         - Generate usage documentation\n");
    fprintf(f, "help           - Display available commands\n");
    fprintf(f, "\nDeveloped by:\n");
    fprintf(f, "Vineet Kumar\n");
    fclose(f);

    printf("README.txt generated successfully.\n");
}

void load_commits() {
    FILE *fp = fopen(".mygit\\commits_list.txt", "r");
    if (!fp) return;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';

        CommitNode *new_node = (CommitNode *)malloc(sizeof(CommitNode));
        strcpy(new_node->name, line);
        new_node->next = head;
        head = new_node;
    }

    fclose(fp);
}

void save_commit(const char *foldername) {
    CommitNode *new_node = (CommitNode *)malloc(sizeof(CommitNode));
    strcpy(new_node->name, foldername);
    new_node->next = NULL;

    if (!head) {
        head = new_node;
    } else {
        CommitNode *temp = head;
        while (temp->next) temp = temp->next;
        temp->next = new_node;
    }

    FILE *fp = fopen(".mygit\\commits_list.txt", "a");
    if (fp) {
        fprintf(fp, "%s\n", foldername);
        fclose(fp);
    }
}

void clear_commits() {
    CommitNode *current = head;
    while (current) {
        CommitNode *next = current->next;
        free(current);
        current = next;
    }
    head = NULL;
}

void help() {
    printf("Available Commands:\n");
    printf("init           - Initialize a new repository\n");
    printf("commit <file>  - Commit a file\n");
    printf("status         - Show repository status\n");
    printf("delete         - Delete the latest commit\n");
    printf("log            - View commit history\n");
    printf("readme         - Generate usage documentation\n");
    printf("help           - Display this help menu\n");
}
