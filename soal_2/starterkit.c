#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include <signal.h>

#define STARTER_KIT_DIR "starter_kit"
#define QUARANTINE_DIR "quarantine"
#define PID_FILE ".daemon/decrypt.pid"
#define LOG_FILE "activity.log"

int return_mode = 0;

void run_process(char *args[]) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("/ ❦ . . { ERROR } Proses execvp gagal.");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("/ ❦ . . { ERROR } Proses fork gagal.");
        exit(EXIT_FAILURE);
    }
}

int folder_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

int is_dir_empty(const char *path) {
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (!dir) return 1;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            closedir(dir);
            return 0;
        }
    }

    closedir(dir);
    return 1;
}

void download_kit() {
    if (return_mode) {
        return;
    }

    char downloaded_flag[512];
    snprintf(downloaded_flag, sizeof(downloaded_flag), ".daemon/.downloaded");

    if (access(".daemon/.downloaded", F_OK) == 0) {
        printf("୨♡୧ ... Starter kit sudah pernah di download. Lewati download ulang.\n");
        return;
    }

    printf("୨♡୧ ... Starter kit belum pernah di download. Mulai download...\n");
    mkdir(".daemon", 0755); 

    char *curl_args[] = {
        "curl", "-v", "-L", "-o", "starter_kit.zip",
        "https://drive.google.com/uc?export=download&id=1_5GxIGfQr3mNKuavJbte_AoRkEQLXSKS",
        NULL
    };    
    run_process(curl_args);

    char *unzip_args[] = {
        "unzip", "starter_kit.zip", "-d", STARTER_KIT_DIR,
        NULL
    };
    run_process(unzip_args);

    char *rm_args[] = {
        "rm", "starter_kit.zip",
        NULL
    };
    run_process(rm_args);

    FILE *flag = fopen(".daemon/.downloaded", "w");
    if (flag) fclose(flag);
}

void log_activity(const char *msg) {
    FILE *fp = fopen(LOG_FILE, "a");
    if (!fp) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char buf[100];
    strftime(buf, sizeof(buf), "[%d-%m-%Y][%H:%M:%S]", t);

    fprintf(fp, "%s - %s\n", buf, msg);
    fclose(fp);
}

int is_base64_encoded(const char *str) {
    for (int i = 0; str[i]; i++) {
        if (!((str[i] >= 'A' && str[i] <= 'Z') ||
              (str[i] >= 'a' && str[i] <= 'z') ||
              (str[i] >= '0' && str[i] <= '9') ||
              str[i] == '+' || str[i] == '/' || str[i] == '=')) {
            return 0;
        }
    }
    return 1;
}

char *base64_decode(const char *data) {
    int len = strlen(data);
    char *decoded = malloc(len);
    int val = 0, valb = -8, idx = 0;
    for (int i = 0; i < len; i++) {
        char c = data[i];
        if (c >= 'A' && c <= 'Z') c -= 'A';
        else if (c >= 'a' && c <= 'z') c = c - 'a' + 26;
        else if (c >= '0' && c <= '9') c = c - '0' + 52;
        else if (c == '+') c = 62;
        else if (c == '/') c = 63;
        else continue;
        val = (val << 6) + c;
        valb += 6;
        if (valb >= 0) {
            decoded[idx++] = (char)((val >> valb) & 0xFF);
            valb -= 8;
        }
    }
    decoded[idx] = '\0';
    return decoded;
}

int is_daemon_running() {
    FILE *fp = fopen(PID_FILE, "r");
    if (!fp) return 0;

    pid_t pid;
    if (fscanf(fp, "%d", &pid) != 1) {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    if (kill(pid, 0) == 0) {
        return 1;
    } else {
        return 0;
    }
}

void decrypt_daemon() {
    if (is_daemon_running()) {
        printf("୨♡୧ ... Daemon udah jalan.\n");
        exit(0);
    }    
    pid_t pid = fork();
    if (pid < 0) {
        printf("Dekripsi gagal.");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        printf("୨♡୧ ... Dekripsi  Daemon berhasil dengan PID %d.\n", pid);
        exit(EXIT_SUCCESS);
    }

    setsid();
    chdir(".");
    umask(0);

    if (!folder_exists(".daemon")) {
        mkdir(".daemon", 0755);
    }

    FILE *fp = fopen(PID_FILE, "w");
    if (fp) {
        fprintf(fp, "%d", getpid());
        fclose(fp);
    }

    char msg[100];
    snprintf(msg, sizeof(msg), "Successfully started decryption process with PID %d.", getpid());
    log_activity(msg);

    while (1) {
        DIR *d = opendir(STARTER_KIT_DIR);
        if (!d) {
            sleep(3);
            continue;
        }

        struct dirent *entry;
        while ((entry = readdir(d))) {
            if (entry->d_type == DT_REG) {
                char path[512];
                snprintf(path, sizeof(path), "%s/%s", STARTER_KIT_DIR, entry->d_name);

                if (!is_base64_encoded(entry->d_name)) continue;

                char *decoded_name = base64_decode(entry->d_name);
                char new_path[512];
                snprintf(new_path, sizeof(new_path), "%s/%s", STARTER_KIT_DIR, decoded_name);

                if (access(new_path, F_OK) == 0) {
                    free(decoded_name);
                    continue;
                }
                if (rename(path, new_path) == 0)
                free(decoded_name);
            }
        }
        closedir(d);
        sleep(5);
    }
}

void move_files(const char *src, const char *dst, const char *log_template) {
    if (is_dir_empty(src)) {
        printf("Folder %s kosong. Tidak ada file yang dipindahkan.\n", src);
        return;
    }
    DIR *d = opendir(src);
    if (!d) return;
    struct dirent *entry;
    while ((entry = readdir(d))) {
        if (entry->d_type == DT_REG) {
            char src_path[512], dst_path[512];
            snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
            snprintf(dst_path, sizeof(dst_path), "%s/%s", dst, entry->d_name);
            if (rename(src_path, dst_path) == 0) {
                char msg[512];
                snprintf(msg, sizeof(msg), log_template, entry->d_name);
                log_activity(msg);
            }
        }
    }
    closedir(d);
}

void eradicate_quarantine() {
    if (is_dir_empty("quarantine")) {
        printf("/ ❦ . . { ERROR } Tidak ada file di quarantine. Tidak ada yang dihapus.\n");
        return;
    }

    DIR *d = opendir(QUARANTINE_DIR);
    if (!d) return;
    struct dirent *entry;
    while ((entry = readdir(d))) {
        if (entry->d_type == DT_REG) {
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", QUARANTINE_DIR, entry->d_name);
            if (remove(path) == 0) {
                char msg[512];
                snprintf(msg, sizeof(msg), "%s - Successfully deleted.", entry->d_name);
                log_activity(msg);
            }
        }
    }
    closedir(d);
    remove(".daemon/.downloaded");
    printf("୨♡୧ ... Berhasil hapus semua file di quarantine.\n");
}

void shutdown_decrypt_daemon() {
    FILE *fp = fopen(PID_FILE, "r");
    if (!fp) {
        printf("/ ❦ . . { ERROR } Daemon tidak ditemukan. Emang ada dekripsi daemon yang jalan?\n");
        return;
    }

    pid_t pid;
    if (fscanf(fp, "%d", &pid) != 1) {
        printf("/ ❦ . . { ERROR } Gagal membaca PID dari file.\n");
        fclose(fp);
        return;
    }
    fclose(fp);

    if (kill(pid, SIGTERM) == 0) {
        printf("୨♡୧ ... Berhasil membunuh daemon dengan PID %d.\n", pid);
        char msg[128];
        snprintf(msg, sizeof(msg), "Successfully shut off decryption process with PID %d.\n", pid);
        log_activity(msg);
        remove(PID_FILE);
    } else {
        perror("/ ❦ . . { ERROR } Gagal membunuh daemon.");
    }
}

int main(int argc, char *argv[]) {
    if (mkdir(STARTER_KIT_DIR, 0777) == -1 && errno != EEXIST) {
        perror("/ ❦ . . { ERROR } Gagal membuat starter_kit directory.");
    }
    if (mkdir(QUARANTINE_DIR, 0777) == -1 && errno != EEXIST) {
        perror("/ ❦ . . { ERROR } Gagal membuat quarantine directory.");
    }

    if (argc < 2) {
        printf("୨♡୧ ... Contoh: ./starterkit --[decrypt|quarantine|return|eradicate|shutdown]\n");
        return 1;
    }

    if (strcmp(argv[1], "--return") == 0) {
        return_mode = 1;
        move_files(QUARANTINE_DIR, STARTER_KIT_DIR, "%s - Successfully returned to starter kit directory.");
        printf("୨♡୧ ... Berhasil balikin semua file ke starter_kit.\n");
    } else if (strcmp(argv[1], "--quarantine") == 0) {
        move_files(STARTER_KIT_DIR, QUARANTINE_DIR, "%s - Successfully moved to quarantine directory.");
        printf("୨♡୧ ... Berhasil mindahin semua file ke quarantine.\n");
    } else if (strcmp(argv[1], "--eradicate") == 0) {
        eradicate_quarantine();
    } else if (strcmp(argv[1], "--decrypt") == 0) {
        download_kit();
        decrypt_daemon();
    } else if (strcmp(argv[1], "--shutdown") == 0) {
        shutdown_decrypt_daemon();
    } else {
        printf("/ ❦ . . { ERROR } Argumen tidak dikenal: %s.\n", argv[1]);
        printf("୨♡୧ ... Contoh: ./starterkit --[decrypt|quarantine|return|eradicate|shutdown].\n");
        return 1;
    }

    return 0;
}