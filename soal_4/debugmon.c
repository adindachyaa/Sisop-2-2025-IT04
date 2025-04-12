#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <fcntl.h>
#include <time.h>

// ----- BAGIAN A: Mengetahui Semua Aktivitas User -----
void list_processes(const char *username) {
    struct passwd *pwd = getpwnam(username);
    if (!pwd) {
        fprintf(stderr, "Error: User '%s' not found.\n", username);
        return;
    }

    uid_t uid = pwd->pw_uid;
    DIR *dir = opendir("/proc");
    if (dir == NULL) {
        perror("Error opening /proc");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_DIR || !isdigit(entry->d_name[0])) continue;

        char path[256];
        snprintf(path, sizeof(path), "/proc/%s/status", entry->d_name);
        FILE *status_file = fopen(path, "r");
        if (!status_file) continue;

        char line[256];
        char comm[256] = "Unknown";
        uid_t process_uid = -1;

        while (fgets(line, sizeof(line), status_file)) {
            if (strncmp(line, "Name:", 5) == 0) {
                sscanf(line, "Name:\t%255s", comm);
            } else if (strncmp(line, "Uid:", 4) == 0) {
                sscanf(line, "Uid:\t%d", &process_uid);
                break;
            }
        }

        fclose(status_file);

        if (process_uid == uid) {
            printf("PID: %s | Command: %s\n", entry->d_name, comm);
        }
    }

    closedir(dir);
}

// ----- BAGIAN B: Memasang Mata-Mata dalam Mode Daemon -----
void run_daemon(const char *username) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Error forking");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0) {
        perror("Error creating session");
        exit(EXIT_FAILURE);
    }

    umask(0);
    chdir("/");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    char log_file[256];
    snprintf(log_file, sizeof(log_file), "debugmon_%s.log", username);
    int log_fd = open(log_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd < 0) {
        perror("Error opening log file");
        exit(EXIT_FAILURE);
    }

    struct passwd *pwd = getpwnam(username);
    if (!pwd) {
        dprintf(log_fd, "Error: User '%s' not found.\n", username);
        close(log_fd);
        exit(EXIT_FAILURE);
    }

    uid_t uid = pwd->pw_uid;

    while (1) {
        DIR *dir = opendir("/proc");
        if (dir == NULL) {
            dprintf(log_fd, "Error opening /proc\n");
            break;
        }

        struct dirent *entry;
        time_t now = time(NULL);
        dprintf(log_fd, "Timestamp: %s", ctime(&now));

        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type != DT_DIR || !isdigit(entry->d_name[0])) continue;

            char path[256];
            snprintf(path, sizeof(path), "/proc/%s/status", entry->d_name);
            FILE *status_file = fopen(path, "r");
            if (!status_file) continue;

            char line[256];
            char comm[256] = "Unknown";
            uid_t process_uid = -1;

            while (fgets(line, sizeof(line), status_file)) {
                if (strncmp(line, "Name:", 5) == 0) {
                    sscanf(line, "Name:\t%255s", comm);
                } else if (strncmp(line, "Uid:", 4) == 0) {
                    sscanf(line, "Uid:\t%d", &process_uid);
                    break;
                }
            }

            fclose(status_file);

            if (process_uid == uid) {
                dprintf(log_fd, "PID: %s | Command: %s\n", entry->d_name, comm);
            }
        }

        closedir(dir);
        sleep(5); 
    }

    close(log_fd);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <list|daemon> <user>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *command = argv[1];
    const char *username = argv[2];

    if (strcmp(command, "list") == 0) {
        list_processes(username);
    } else if (strcmp(command, "daemon") == 0) {
        run_daemon(username);
    } else {
        fprintf(stderr, "Invalid command. Use 'list' or 'daemon'.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
