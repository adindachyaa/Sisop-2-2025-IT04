#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>     
#include <ctype.h>     
#include <unistd.h>      
#include <signal.h>     
#include <pwd.h>       
#include <sys/types.h>  
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#define FAIL_FLAG "/tmp/debugmon_failed.flag"


void get_cpu_memory_usage(const char *pid, float *cpu_usage, float *memory_usage) {
    char path[256];
    FILE *file;
    char line[256];

    snprintf(path, sizeof(path), "/proc/%s/stat", pid);
    file = fopen(path, "r");
    if (!file) return;

    long unsigned int utime, stime, starttime;
    long unsigned int total_time;
    unsigned long vsize;
    int rss;
    unsigned long long uptime;

    fscanf(file,
           "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u "
           "%lu %lu %*lu %*lu %*lu %lu",
           &utime, &stime, &starttime);
    fclose(file);

   
    total_time = utime + stime;

       file = fopen("/proc/uptime", "r");
    if (file) {
        fscanf(file, "%llu", &uptime);
        fclose(file);
    }

    *cpu_usage = (total_time * 100.0) / sysconf(_SC_CLK_TCK) / uptime;

   
    snprintf(path, sizeof(path), "/proc/%s/statm", pid);
    file = fopen(path, "r");
    if (file) {
        fscanf(file, "%lu %d", &vsize, &rss);
        fclose(file);
    }

   
    *memory_usage = rss * (sysconf(_SC_PAGESIZE) / 1024.0 / 1024.0);
}

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

    printf("PID\tCommand\t\tCPU(%%)\tMemory(MB)\n");
    printf("-------------------------------------------------\n");

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
            float cpu_usage = 0, memory_usage = 0;
            get_cpu_memory_usage(entry->d_name, &cpu_usage, &memory_usage);
            printf("%s\t%-15s%.2f\t%.2f\n", entry->d_name, comm, cpu_usage, memory_usage);
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

   
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

   
    char log_file[256];
    snprintf(log_file, sizeof(log_file), "/tmp/debugmon_%s.log", username);
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
                float cpu_usage = 0, memory_usage = 0;
                get_cpu_memory_usage(entry->d_name, &cpu_usage, &memory_usage);
                dprintf(log_fd, "PID: %s | Command: %s | CPU: %.2f%% | Memory: %.2f MB\n",
                        entry->d_name, comm, cpu_usage, memory_usage);
            }
        }

        closedir(dir);
        sleep(5); // Monitor every 5 seconds
    }

    close(log_fd);
}
// BAGIAN 3: Menghentikan Pengawassan
void stop(const char* username){
    FILE *pid_file;
    pid_t pid;
    char pid_filename[64];
    snprintf(pid_filename, sizeof(pid_filename), "debugmon_%s.pid", username);
    
    pid_file = fopen(pid_filename, "r");

    if(!pid_file){
        fprintf(stderr, "Can't find PID file! Make sure daemon running");
        return;
    }

    if(fscanf(pid_file, "%d", &pid) != 1){
        fprintf(stderr, "Fail to read PID! \n");
        fclose(pid_file);
        return;
    }

    fclose(pid_file);
    if (kill(pid, SIGTERM) == 0) {
        printf("Succesfully stopped user %s (PID: %d) Debugmon\n", username, pid);
        remove(pid_filename); 
    } else {
        perror("Fail to stop process...");
    }
} 

uid_t get_uid(const char* username) {
    struct passwd *pwd = getpwnam(username);
    if (!pwd) {
        fprintf(stderr, "User %s not found.\n", username);
        exit(EXIT_FAILURE);
    }
    return pwd->pw_uid;
}

void fail(const char* username){
    DIR *proc;
    proc = opendir("/proc");
    struct dirent *entry;
    uid_t target_uid = get_uid(username);

    char logpath[256];
    snprintf(logpath, sizeof(logpath), "logs/%s.log", username);
    FILE *logfile = fopen(logpath, "a");

    if (!logfile) {
        perror("Fail to open logfile");
        closedir(proc);
        return;
    }

    while ((entry = readdir(proc)) != NULL) {
        if (!isdigit(entry->d_name[0])) continue;

        char status_path[256];
        snprintf(status_path, sizeof(status_path), "/proc/%s/status", entry->d_name);

        FILE *status_file = fopen(status_path, "r");
        if (!status_file) continue;

        char line[256];
        uid_t uid = -1;
        char cmdline[256] = "unknown";

        while (fgets(line, sizeof(line), status_file)) {
            if (strncmp(line, "Uid:", 4) == 0) {
                sscanf(line, "Uid:\t%u", &uid);
            } else if (strncmp(line, "Name:", 5) == 0) {
                sscanf(line, "Name:\t%255s", cmdline);
            }
        }
        fclose(status_file);

        if (uid == target_uid) {
            pid_t pid = atoi(entry->d_name);
            if (kill(pid, SIGKILL) == 0) {
                fprintf(logfile, "[FAILED] PID %d (command: %s)\n", pid, cmdline);
                printf("Killed PID %d (%s)\n", pid, cmdline);
            } else {
                fprintf(logfile, "[ERROR] Failed killing PID %d (%s)\n", pid, cmdline);
            }
        }
    }

    fclose(logfile);
    closedir(proc);
}

// ----- BAGIAN E: Mengizinkan User Kembali Menjalankan Proses -----
void revert(const char *username) {
    printf("Mode normal dipulihkan untuk user %s. Proses baru dapat dijalankan kembali.\n", username);
    
    char log_entry[256];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(log_entry, sizeof(log_entry), "[%d:%m:%Y]-[%H:%M:%S]_%s_REVERT\n", t);
    FILE *logFile = fopen("debugmon.log", "a");
    if (logFile != NULL) {
        fprintf(logFile, "%s", log_entry);
        fclose(logFile);
    }
}

// ----- BAGIAN F: Mencatat ke dalam File Log -----
void writeLog(const char *processName, const char *status) {
    FILE *logFile = fopen("debugmon.log", "a");
    if (logFile == NULL) {
        perror("Gagal membuka file log");
        return;
    }

    char timestamp[100];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "[%d:%m:%Y]-[%H:%M:%S]", t);

    fprintf(logFile, "%s_%s_%s\n", timestamp, processName, status);
    fclose(logFile);
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
    } else if (strcmp(command, "stop") == 0) {
        stop(username);
    } 
    else if (strcmp(command, "fail") == 0) {
        fail(username);
    } 
    else if (strcmp(command, "revert") == 0) {
        revert(username);
    }
    else {
        fprintf(stderr, "Invalid command. Use 'list' or 'daemon'.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
