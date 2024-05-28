#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void list_directory(const char *path, int show_all, int long_format) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (!show_all && entry->d_name[0] == '.') {
            continue;
        }
        if (long_format) {
            struct stat file_stat;
            if (stat(entry->d_name, &file_stat) == 0) {
                printf("%c%10ld %5d %5d %10lld %s\n",
                       (S_ISDIR(file_stat.st_mode)) ? 'd' : '-',
                       (long)file_stat.st_nlink,
                       file_stat.st_uid,
                       file_stat.st_gid,
                       (long long)file_stat.st_size,
                       entry->d_name);
            } else {
                perror("stat");
            }
        } else {
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *path = ".";
    int show_all = 0;
    int long_format = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) {
            show_all = 1;
        } else if (strcmp(argv[i], "-l") == 0) {
            long_format = 1;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            exit(EXIT_FAILURE);
        } else {
            path = argv[i];
        }
    }

    list_directory(path, show_all, long_format);

    return 0;
}
