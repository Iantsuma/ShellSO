#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define MAX_SIZE_CMD 256
#define MAX_SIZE_ARG 16

char cmd[MAX_SIZE_CMD];
char *argv[MAX_SIZE_ARG];
pid_t pid;
pid_t pidlista[256];
int i;

void get_cmd();
void convert_cmd();
void c_shell();
void addpath(char *argv[]);
void cdir(char *argv[]);
void batch(const char *filename);

int main() {
    while (1) {
        get_cmd();

        convert_cmd();
        
        if (argv[0] == NULL) continue;
        if (strcmp("run", argv[0]) == 0) {
            if (argv[1] != NULL) {
                batch(argv[1]);
            } else {
                fprintf(stderr, "Erro, arquivo batch não especificado\n");
            }
        } else {
            c_shell();
        }
    }

    return 0;
}

void c_shell() {
    if (argv[0] == NULL || strcmp("", argv[0]) == 0) return;

    if (strcmp("exit", argv[0]) == 0) kill(0, SIGKILL);

    if (strcmp("path", argv[0]) == 0) {
        addpath(argv);
        return;
    }
    
    if (strcmp("cd", argv[0]) == 0) {
        cdir(argv);
        return;
    }

    pid = fork();
    if (pid == -1) {
        printf("Falha ao criar um filho (sucesso?)\n");
    } else if (pid == 0) {
        execvp(argv[0], argv);
        perror("execvp falhou");
        exit(EXIT_FAILURE);
    } else {
        waitpid(pid, NULL, 0);
    }
}

void get_cmd() {
    printf("@: ");
    if (fgets(cmd, MAX_SIZE_CMD, stdin) == NULL) {
        perror("fgets");
        exit(EXIT_FAILURE);
    }

    if ((strlen(cmd) > 0) && (cmd[strlen(cmd) - 1] == '\n'))
        cmd[strlen(cmd) - 1] = '\0';
}

void convert_cmd() {
    char *ptr;
    i = 0;
    ptr = strtok(cmd, " ");
    while (ptr != NULL && i < MAX_SIZE_ARG - 1) {
        argv[i] = ptr;
        i++;
        ptr = strtok(NULL, " ");
    }
    argv[i] = NULL;
}

void addpath(char *argv[]) {
    const char *path_og = getenv("PATH");
    const char *path_ng = argv[1];
    char buf[4096];

    if (path_ng == NULL) {
        fprintf(stderr, "Erro, endereço não existe\n");
        return;
    }

    if (snprintf(buf, sizeof(buf), "%s:%s", path_og, path_ng) >= sizeof(buf)) {
        fprintf(stderr, "Errorendereço mto comprido\n");
        return;
    }

    if (setenv("PATH", buf, 1) != 0) {
        perror("setenv");
        return;
    }
    printf("%s added to PATH\n", path_ng);
}

void cdir(char *argv[]) {
    const char *dir = argv[1];
    if (dir == NULL) {
        fprintf(stderr, "Erro, diretório não existe. Cabeçudo\n");
        return;
    }

    if (chdir(dir) != 0) {
        perror("chdir");
        return;
    }
}

void batch(const char *filename) {
    FILE *bfile = fopen(filename, "r");
    char line[1024];

    if (bfile == NULL) {
        perror("Erro ao abrir arquivo batch, vazio");
        return;
    }

    while (fgets(line, sizeof(line), bfile) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        strncpy(cmd, line, sizeof(cmd) - 1);
        cmd[sizeof(cmd) - 1] = '\0';

        convert_cmd();
        c_shell();
    }

    fclose(bfile);
}
