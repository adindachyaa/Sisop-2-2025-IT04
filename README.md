# PRAKTIKUM SISOP-2-2025-MH-IT04

KELOMPOK IT04
| No | Nama                              | NRP         |
|----|-----------------------------------|-------------|
| 1  | Raya Ahmad Syarif                 | 5027241041  |
| 2  | Salsa Bil Ulla         | 5027241052 |
| 3  | Adinda Cahya Pramesti   | 5027241117  |

# Pengantar

Laporan resmi ini dibuat untuk praktikum modul 2 yang dilaksanakan pada tanggal 7 Maret 2025 sampai dengan 11 Maret 2025. Praktikum Modul 2 ini terdiri dari empat soal dan dikerjakan oleh tiga orang anggota dengan pembagian tertentu.

Berikut ini pembagian pengerjaan dari kelompok IT04:

    Soal 1 dikerjakan oleh Raya Ahmad Syarif
    Soal 2 dikerjakan oleh Salsa Bil Ulla
    Soal 3 dikerjakan oleh Adinda Cahya Pramesti
    Soal 4 dibagi menjadi 3 bagian:
    - 4a & 4b dikerjakan oleh Adinda Cahya Pramesti
    - 4c & 4d dikerjakan oleh Raya Ahmad Syarif
    - 4e & 4f dikerjakan oleh Salsa Bil Ulla

Sehingga dengan demikian, Pembagian bobot pengerjaan soal menjadi (Raya 33.3%, Salsa 33.3%, Dinda 33.3%).

Kelompok IT04 juga telah menyelesaikan tugas praktikum modul 2 yang telah diberikan. Dari hasil praktikum yang telah dilakukan sebelumnya, maka diperoleh hasil sebagaimana yang dituliskan pada setiap bab di bawah ini.
# Ketentuan
Berikut ini struktur dari repositori praktikum modul 2:

```
—soal_1:
  	— action.c
—soal_2:
  	— starterkit.c
—soal_3:
  	— malware.c
—soal_4:
  	— debugmon.c

```
## Soal 1
Kamu terbangun secara tiba-tiba di suatu lokasi yang tidak diketahui, saat kamu melihat sekitar yang terlihat hanyalah kegelapan dan sebuah pintu dengan dua mata berwarna yang melihatmu dari lubang pintu tersebut.

Ia merupakan naga bernama Cyrus yang menjaga pintu tersebut dan kamu harus meng-input password yang benar untuk masuk. Karena tidak mungkin untuk menebak password, Ia memberikanmu sebuah clue Clues.zip. Untungnya, kamu merupakan Directory Lister yang jago sehingga clue ini dapat berubah menjadi sebuah password. [Author: Fico/ purofuro]

> Penyelesaian
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>

void filter() {
        system("mkdir -p Filtered && echo Filtered Directory Created.");
        system("find Clues -type f -regex '.*/[a-zA-Z0-9]\\.txt' -exec sh -c 'mv \"$0\" Filtered/ && echo Moved files: \"$0\"' {} \\;");
        system("find Clues -type f ! -regex '.*/[a-zA-Z0-9]\\.txt' -exec sh -c 'echo Deleted files: \"$1\" && rm \"$1\"' _ {} \\;");
        printf("Filter Completed.");
}

void combine() {
    DIR *dir = opendir("Filtered");
    if(!dir){
        printf("Can't find Filtered directory. Make sure you filter first! \n");
        return;
    }

    struct dirent *entry;
    char *num[10], *alpha[10];
    int num_idx = 0, a_idx = 0;

    //categorize file (num or alphabet)
    while((entry = readdir(dir)) != NULL){
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        if (isdigit(entry->d_name[0])) {
            num[num_idx++] = strdup(entry->d_name);
        } else if (isalpha(entry->d_name[0])) {
            alpha[a_idx++] = strdup(entry->d_name);
        }
    }
        closedir(dir);

        //sort number
        for (int i = 0; i < num_idx - 1; i++) {
            for (int j = 0; j < num_idx - i - 1; j++) {
                if (atoi(num[j]) > atoi(num[j + 1])) {
                    char *temp = num[j];
                    num[j] = num[j + 1];
                    num[j + 1] = temp;
                }
            }
        }

        //sort alphabet
        for (int i = 0; i < a_idx - 1; i++) {
            for (int j = 0; j < a_idx - i - 1; j++) {
                if (strcmp(alpha[j], alpha[j + 1]) > 0) {
                    char *temp = alpha[j];
                    alpha[j] = alpha[j + 1];
                    alpha[j + 1] = temp;
                }
            }
        }


        FILE *combined = fopen("Combined.txt", "w");
        char path[256], buffer[256];
        int i = 0;
        int j = 0;
        FILE *f;

        while (i < num_idx || j < a_idx) {
            if (i < num_idx) {
                snprintf(path, sizeof(path), "Filtered/%s", num[i]);
                f = fopen(path, "r");
                if (f) {
                    while (fgets(buffer, sizeof(buffer), f))
                        fputs(buffer, combined);
                    fclose(f);
                    remove(path);
                    printf("Copied: %s\n", num[i]);
                }
                free(num[i]);
                i++;
            }

            if (j < a_idx) {
                snprintf(path, sizeof(path), "Filtered/%s", alpha[j]);
                f = fopen(path, "r");
                if (f) {
                    while (fgets(buffer, sizeof(buffer), f))
                        fputs(buffer, combined);
                    fclose(f);
                    remove(path);
                    printf("Copied: %s\n", alpha[j]);
                }
                free(alpha[j]);
                j++;
            }
        }
    fclose(combined);
    printf("Combine success. \n");
}

char rot13decoder(char c){
    if('A' <= c && c <= 'Z'){
        return ((c - 'A' + 13) % 26) + 'A';
    }
    else if ('a' <= c && c <= 'z'){
        return ((c - 'a' + 13) % 26) + 'a';
    }
    else{
        return c;
    }
}

void decode(){
    FILE *code = fopen("Combined.txt", "r");
    int c;
    FILE *decode = fopen("Decoded.txt", "w");
    while((c = fgetc(code)) != EOF) {
        fputc(rot13decoder(c), decode);
    }
    fclose(code);
    fclose(decode);
    printf("Decode success!");
}


int clue_folder(const char *foldername){
    struct stat st;
    return stat(foldername, &st) == 0 && S_ISDIR(st.st_mode);
}

int main(int argc, char *argv[]) {
    char wget[200];
    char unzip[100];
    char rmv[20];

    if(argc == 3){
        if(strcmp(argv[1], "-m") == 0){
            if(strcmp(argv[2], "Filter") == 0){
                filter();
            }else if(strcmp(argv[2], "Combine") == 0){
                combine();
                }
            else if(strcmp(argv[2], "Decode") == 0){
                decode();
            }
        else{
                printf("please input the right mode \n");
            }
        }
    }
    else{
        if(!clue_folder("Clues")){
            strcpy(wget, "wget --no-check-certificate 'https://docs.google.com/uc?export=download&id=1xFn1OBJUuSdnApDseEczKhtNzyGekauK' -O clue.zip");
            strcpy(unzip, "unzip clue.zip");
            strcpy(rmv, "rm clue.zip");

            system(wget);
            system(unzip);
            system(rmv);
    } else {
            printf("Clues folder has been extracted. \n");
        }
    }
    return 0;
}

```
> Penjelasan:
1. Header Inclusion
```                                                             #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
```
Header inclusion mengimpor library yang diperlukan untuk operasi I/O, manipulasi string, dan lain-lain.

2. Function clue_folder
```
int clue_folder(const char *foldername){
    struct stat st;
    return stat(foldername, &st) == 0 && S_ISDIR(st.st_mode);
}
```
Function ini digunakan untuk mengecek keberadaan folder "Clues" dalam direktori. Jika ada akan mengembalikan nilai 0 sehingga apabila folder sudah ada maka tidak akan mendownload "clues.zip".  

3. Function filter
```
void filter() {
        system("mkdir -p Filtered && echo Filtered Directory Created.");
        system("find Clues -type f -regex '.*/[a-zA-Z0-9]\\.txt' -exec sh -c 'mv \"$0\" Filtered/ && echo Moved files: \"$0\"' {} \\;");
        system("find Clues -type f ! -regex '.*/[a-zA-Z0-9]\\.txt' -exec sh -c 'echo Deleted files: \"$1\" && rm \"$1\"' _ {} \\;");
        printf("Filter Completed.");
}
```
Filter di sini untuk membuat folder "Filtered" yang akan digunakan untuk menyimpan file .txt hasil filter. Function ini juga digunakan untuk mem-filter file.txt sesuai dengan kriterianya yaitu filenamenya hanya mengandung satu huruf dan satu angka lalu menghapus file yang tidak memenuhi kriteria.
4. Function combine
```
void combine() {
    DIR *dir = opendir("Filtered");
    if(!dir){
        printf("Can't find Filtered directory. Make sure you filter first! \n");
        return;
    }

    struct dirent *entry;
    char *num[10], *alpha[10];
    int num_idx = 0, a_idx = 0;

    //categorize file (num or alphabet)
    while((entry = readdir(dir)) != NULL){
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        if (isdigit(entry->d_name[0])) {
            num[num_idx++] = strdup(entry->d_name);
        } else if (isalpha(entry->d_name[0])) {
            alpha[a_idx++] = strdup(entry->d_name);
        }
    }
        closedir(dir);

        //sort number
        for (int i = 0; i < num_idx - 1; i++) {
            for (int j = 0; j < num_idx - i - 1; j++) {
                if (atoi(num[j]) > atoi(num[j + 1])) {
                    char *temp = num[j];
                    num[j] = num[j + 1];
                    num[j + 1] = temp;
                }
            }
        }

        //sort alphabet
        for (int i = 0; i < a_idx - 1; i++) {
            for (int j = 0; j < a_idx - i - 1; j++) {
                if (strcmp(alpha[j], alpha[j + 1]) > 0) {
                    char *temp = alpha[j];
                    alpha[j] = alpha[j + 1];
                    alpha[j + 1] = temp;
                }
            }
        }


        FILE *combined = fopen("Combined.txt", "w");
        char path[256], buffer[256];
        int i = 0;
        int j = 0;
        FILE *f;

        while (i < num_idx || j < a_idx) {
            if (i < num_idx) {
                snprintf(path, sizeof(path), "Filtered/%s", num[i]);
                f = fopen(path, "r");
                if (f) {
                    while (fgets(buffer, sizeof(buffer), f))
                        fputs(buffer, combined);
                    fclose(f);
                    remove(path);
                    printf("Copied: %s\n", num[i]);
                }
                free(num[i]);
                i++;
            }

            if (j < a_idx) {
                snprintf(path, sizeof(path), "Filtered/%s", alpha[j]);
                f = fopen(path, "r");
                if (f) {
                    while (fgets(buffer, sizeof(buffer), f))
                        fputs(buffer, combined);
                    fclose(f);
                    remove(path);
                    printf("Copied: %s\n", alpha[j]);
                }
                free(alpha[j]);
                j++;
            }
        }
    fclose(combined);
    printf("Combine success. \n");
}
```
Function combine digunakan untuk menggabungkan isi dari file .txt dari hasil filter. Cara kerjanya adalah pertama dengan menyiapkan array num dan char terlebih dahulu untuk menyimpan file yang termasuk huruf dan angka. Terdapat looping dengan function isdigit untuk mengecek filename angka, lalu memasukkannya ke array num. Begitu juga dengan filename huruf. Setelah itu, akan dibuka file "combined.txt" dengan mode write untuk menulis hasilnya. Pertama, akan dilakukan sorting terlebih dahulu agar sesuai dengan urutan. Setelah sesuai, akan dilakukan penggabungan isi secara selang-seling.

4. Function rot13decoder
```
char rot13decoder(char c){
    if('A' <= c && c <= 'Z'){
        return ((c - 'A' + 13) % 26) + 'A';
    }
    else if ('a' <= c && c <= 'z'){
        return ((c - 'a' + 13) % 26) + 'a';
    }
    else{
        return c;
    }
}
```
Function ini untuk mendecode kode rot13 hasil dari combine sebelumnya dan akan digunakan di function decode.

5. Function decode
```
void decode(){
    FILE *code = fopen("Combined.txt", "r");
    int c;
    FILE *decode = fopen("Decoded.txt", "w");
    while((c = fgetc(code)) != EOF) {
        fputc(rot13decoder(c), decode);
    }
    fclose(code);
    fclose(decode);
    printf("Decode success!");
}

```
Hasil dari combine tadi akan dibuka dan membuka file decode.txt tadi dalam mode write yang mana akan menuliskan hasil decode dari combine. Function ini akan mendecode satu persatu char dalam file combine dengan memanfaatkan function rot13decoder, jika sudah akan ditulis ke file "decode.txt" dan akan menutup kedua file yang sudah dibuka.

## Soal 2
Ada seorang perempuan nolep yang bernama Kanade Yoisaki. Beliau adalah siswi SMA yang mengambil kelas online (tipsen dulu ygy) karena malas sekali untuk keluar rumah. Sebagai gantinya, ia bekerja sebagai seorang composer dan hanya tinggal di dalam rumah saja untuk membuat musik secara terus - menerus. Berkat usahanya, musik ciptaan Kanade pun mulai terkenal dan akhirnya Kanade pun membuat sebuah grup musik bernama 25-ji, Nightcord de., atau yang biasa dikenal dengan N25 atau Niigo.

Pada suatu hari, Kanade ingin membuat sebuah musik baru beserta dengan anggota grup musik lainnya, yaitu Mizuki Akiyama, Mafuyu Asahina, dan Ena Shinonome. Namun sialnya, komputer Kanade terkena sebuah virus yang tidak diketahui. Setelah dianalisis oleh Kanade sendiri, ternyata virus ini bukanlah sebuah trojan, ransomware, maupun tipe virus berbahaya lainnya, melainkan hanya sebuah malware biasa yang hanya bisa membuat sebuah perangkat menjadi lebih lambat dari biasanya. [Author: Haidar / **scar** / **hemorrhager** / 恩赫勒夫]

a. Sebagai teman yang baik, Mafuyu merekomendasikan Kanade untuk **mendownload** dan **unzip** sebuah starter kit berisi file - file acak (sudah termasuk virus) melalui <ins>[link berikut](https://drive.google.com/file/d/1_5GxIGfQr3mNKuavJbte_AoRkEQLXSKS/view)</ins> agar dapat membantu Kanade dalam mengidentifikasi virus - virus yang akan datang. Jangan lupa untuk **menghapus file zip asli** setelah melakukan **unzip**.

b. Setelah mendownload starter kit tersebut, Mafuyu ternyata lupa bahwa pada starter kit tersebut, tidak ada alat untuk **mendecrypt nama** dari file yang diencrypt menggunakan algoritma **Base64**. Oleh karena itu, bantulah Mafuyu untuk **membuat sebuah directory karantina** yang dapat **mendecrypt nama file** yang ada di dalamnya (Hint: gunakan **daemon**).
Penggunaan:
   i. ./starterkit --decrypt

c. Karena Kanade adalah orang yang sangat pemalas (kecuali jika membuat musik), maka tambahkan juga **fitur** untuk **memindahkan file** yang ada pada directory starter kit ke directory karantina, dan begitu juga sebaliknya.
Penggunaan:
   i. ./starterkit --quarantine (pindahkan file dari directory starter kit ke karantina)
   ii. ./starterkit --return (pindahkan file dari directory karantina ke starter kit)

d. Ena memberikan ide kepada mereka untuk menambahkan fitur untuk menghapus file - file yang ada pada directory karantina. Mendengar ide yang bagus tersebut, Kanade pun mencoba untuk menambahkan fitur untuk **menghapus seluruh file** yang ada di dalam **directory karantina**.
Penggunaan:
   i. ./starterkit --eradicate 

e. Karena tagihan listrik Kanade sudah membengkak dan tidak ingin komputernya menyala secara terus - menerus, ia ingin program decrypt nama file miliknya dapat **dimatikan** secara **aman** berdasarkan **PID** dari **proses** program tersebut.
Penggunaan:
   i. ./starterkit --shutdown

f. Mafuyu dan Kanade juga ingin program mereka dapat digunakan dengan aman dan nyaman tanpa membahayakan penggunanya sendiri, mengingat Mizuki yang masih linglung setelah keluar dari labirin Santerra De Laponte. Oleh karena itu, tambahkan **error handling** sederhana untuk mencegah penggunaan yang salah pada program tersebut.

g. Terakhir, untuk **mencatat** setiap penggunaan program ini, Kanade beserta Mafuyu ingin menambahkan **log** dari **setiap penggunaan** program ini dan **menyimpannya** ke dalam file bernama **activity.log**.
Format:
   i. Decrypt: 
[dd-mm-YYYY][HH:MM:SS] - Successfully started decryption process with PID <pid>.

   ii. Quarantine:
[dd-mm-YYYY][HH:MM:SS] - <nama file> - Successfully moved to quarantine directory.

   iii. Return:
[dd-mm-YYYY][HH:MM:SS] - <nama file> - Successfully returned to starter kit directory.

   iv. Eradicate:
[dd-mm-YYYY][HH:MM:SS] - <nama file> - Successfully deleted.

   v. Shutdown:
[dd-mm-YYYY][HH:MM:SS] - Successfully shut off decryption process with PID <pid>.

Contoh struktur akhir directory untuk soal ini adalah sebagai berikut.
**soal_2**
    ├── **activity.log**
    ├── **quarantine**
    ├── **starter_kit**
    │           └── <file hasil unzip>
    ├── **starterkit**
    └── **starterkit.c**

### > Penyelesaian
```
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
    mkdir(STARTER_KIT_DIR, 0777);
    mkdir(QUARANTINE_DIR, 0777);

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
```

### > Penjelasan
1. Buat `starterkit.c`
```
touch starterkit.c
```
2. Tambahkan library yang dibutuhkan
```
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
```
`stdio.h` = Untuk operasi input/output standar, seperti `printf`, `scanf`, `fopen`, `fgets`, dll.
`string.h` =
`stdlib.h` =
`unistd.h` =
`sys/types.h` =
`sys/wait.h` =
`sys/stat.h` =
`fcntl.h` =
`dirent.h` =
`time.h` =
`signal.h` =




## SOAL 4
Suatu hari, Nobita menemukan sebuah alat aneh di laci mejanya. Alat ini berbentuk robot kecil dengan mata besar yang selalu berkedip-kedip. Doraemon berkata, "Ini adalah Debugmon! Robot super kepo yang bisa memantau semua aktivitas di komputer!" Namun, alat ini harus digunakan dengan hati-hati. Jika dipakai sembarangan, bisa-bisa komputer Nobita malah error total!

A. Mengetahui semua aktivitas user
Doraemon ingin melihat apa saja yang sedang dijalankan user di komputernya. Maka, dia mengetik:
./debugmon list <user>
Debugmon langsung menampilkan daftar semua proses yang sedang berjalan pada user tersebut beserta PID, command, CPU usage, dan memory usage.

B. Memasang mata-mata dalam mode daemon
Doraemon ingin agar Debugmon terus memantau user secara otomatis. Doraemon pun menjalankan program ini secara daemon dan melakukan pencatatan ke dalam file log dengan menjalankan:
./debugmon daemon <user>

C. Menghentikan pengawasan
User mulai panik karena setiap gerak-geriknya diawasi! Dia pun memohon pada Doraemon untuk menghentikannya dengan:
./debugmon stop <user>

D. Menggagalkan semua proses user yang sedang berjalan
Doraemon yang iseng ingin mengerjai user dengan mengetik:
./debugmon fail <user>
Debugmon langsung menggagalkan semua proses yang sedang berjalan dan menulis status proses ke dalam file log dengan status FAILED. Selain menggagalkan, user juga tidak bisa menjalankan proses lain dalam mode ini.

E. Mengizinkan user untuk kembali menjalankan proses
Karena kasihan, Shizuka meminta Doraemon untuk memperbaiki semuanya. Doraemon pun menjalankan:
./debugmon revert <user>
Debugmon kembali ke mode normal dan bisa menjalankan proses lain seperti biasa.

F. Mencatat ke dalam file log
Sebagai dokumentasi untuk mengetahui apa saja yang debugmon lakukan di komputer user, debugmon melakukan pencatatan dan penyimpanan ke dalam file debugmon.log untuk semua proses yang dijalankan dengan format
[dd:mm:yyyy]-[hh:mm:ss]_nama-process_STATUS(RUNNING/FAILED)
Untuk poin b, c, dan e, status proses adalah RUNNING. Sedangkan untuk poin d, status proses adalah FAILED. 


### struktur soal 
- debugmon.c


## Jawaban

### A. Mengetahui semua aktivitas user
```
void list_user_processes(const char *username) {
    DIR *proc_dir;
    struct dirent *entry;

    proc_dir = opendir("/proc");
    if (!proc_dir) {
        perror("Failed to open /proc directory");
        exit(EXIT_FAILURE);
    }

    printf("%-10s %-10s %-10s %-10s\n", "PID", "COMMAND", "CPU%", "MEM%");
    while ((entry = readdir(proc_dir)) != NULL) {
        if (!isdigit(entry->d_name[0]))
            continue;

        char stat_path[256], cmdline_path[256], status_path[256];
        FILE *stat_file, *cmdline_file, *status_file;
        char line[1024];
        int pid = atoi(entry->d_name);

        snprintf(stat_path, sizeof(stat_path), "/proc/%d/stat", pid);
        snprintf(cmdline_path, sizeof(cmdline_path), "/proc/%d/cmdline", pid);
        snprintf(status_path, sizeof(status_path), "/proc/%d/status", pid);

        status_file = fopen(status_path, "r");
        if (!status_file) continue;

        int uid;
        while (fgets(line, sizeof(line), status_file)) {
            if (strncmp(line, "Uid:", 4) == 0) {
                sscanf(line, "Uid: %d", &uid);
                break;
            }
        }
        fclose(status_file);

        char *user_from_uid = getpwuid(uid)->pw_name;
        if (strcmp(user_from_uid, username) != 0)
            continue;

        stat_file = fopen(stat_path, "r");
        if (!stat_file) continue;

        unsigned long utime, stime, rss;
        fscanf(stat_file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %*d %*d %*d %*d %*d %*d %lu", 
                &utime, &stime, &rss);
        fclose(stat_file);

        unsigned long total_time = utime + stime;

        cmdline_file = fopen(cmdline_path, "r");
        if (!cmdline_file) continue;

        char cmdline[256];
        fgets(cmdline, sizeof(cmdline), cmdline_file);
        fclose(cmdline_file);

        if (strlen(cmdline) == 0)
            snprintf(cmdline, sizeof(cmdline), "[%d]", pid);

        printf("%-10d %-10s %-10lu %-10lu\n", pid, cmdline, total_time, rss);
    }

    closedir(proc_dir);
}
```

### > Penjelasan
#### 1. Deklarasi Variabel dan Direktori
```
void list_user_processes(const char *username) {
    DIR *proc_dir;
    struct dirent *entry;

``` 
```DIR *proc_dir``` mengarah ke direktori ```/proc``` dan dia akan membaca informasi proses yang sedang berjalan.

```struct dirent *entry``` untuk membaca entri file atau direktori dalam ```/proc```.

#### 2. Membuka Direktori ```/proc```
```
proc_dir = opendir("/proc");
if (!proc_dir) {
    perror("Failed to open /proc directory");
    exit(EXIT_FAILURE);
}
```
```opendir``` untuk membuka direktori ```/proc``` dan membaca entri proses.

```perror dan exit``` untuk menangani error jika direktori ```/proc``` tidak dapat diakses.

#### 3. Header Tabel
```
printf("%-10s %-10s %-10s %-10s\n", "PID", "COMMAND", "CPU%", "MEM%");
```
Untuk menampilkan header tabel untuk kolom: PID, COMMAND, CPU%, dan MEM%.

#### 4. Iterasi Direktori ```/proc```
```
while ((entry = readdir(proc_dir)) != NULL) {
    if (!isdigit(entry->d_name[0]))
        continue;
```

```readdir``` untuk membaca setiap entri dalam direktori ```/proc```.

```isdigit(entry->d_name[0])``` untuk memeriksa apakah nama entri dimulai dengan angka, karena hanya entri dengan angka yang mewakili proses.

#### 5. Path untuk File Proses
```
char stat_path[256], cmdline_path[256], status_path[256];
snprintf(stat_path, sizeof(stat_path), "/proc/%d/stat", pid);
snprintf(cmdline_path, sizeof(cmdline_path), "/proc/%d/cmdline", pid);
snprintf(status_path, sizeof(status_path), "/proc/%d/status", pid);
```

```snprintf``` untuk membuat path absolut untuk file yang berisi informasi proses:

```/proc/[PID]/stat``` berisi statistik proses.

```/proc/[PID]/cmdline``` berisi command line yang digunakan untuk memulai proses.

```/proc/[PID]/status``` berisi metadata proses, termasuk UID.


#### 6. Membaca UID dari file status
```
status_file = fopen(status_path, "r");
if (!status_file) continue;
while (fgets(line, sizeof(line), status_file)) {
    if (strncmp(line, "Uid:", 4) == 0) {
        sscanf(line, "Uid: %d", &uid);
        break;
    }
}
fclose(status_file);
```

akan membuka file ```/proc/[PID]/status``` untuk membaca UID proses dan mencocokkannya dengan username target.

#### 7. Membaca statistik proses

```
stat_file = fopen(stat_path, "r");
if (!stat_file) continue;
unsigned long utime, stime, rss;
fscanf(stat_file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %*d %*d %*d %*d %*d %*d %lu", 
        &utime, &stime, &rss);
fclose(stat_file);
```
akan membaca file ```/proc/[PID]/stat``` untuk mendapatkan waktu CPU (utime dan stime) serta penggunaan memori (rss) dari proses.


#### 8. Membaca nama command
```
cmdline_file = fopen(cmdline_path, "r");
if (!cmdline_file) continue;
char cmdline[256];
fgets(cmdline, sizeof(cmdline), cmdline_file);
fclose(cmdline_file);

if (strlen(cmdline) == 0)
    snprintf(cmdline, sizeof(cmdline), "[%d]", pid);
```

Membaca file ```/proc/[PID]/cmdline``` untuk mendapatkan command line yang digunakan untuk menjalankan proses. Jika kosong, menggantinya dengan [PID].

#### 9. Menampilkan informasi proses
```
printf("%-10d %-10s %-10lu %-10lu\n", pid, cmdline, total_time, rss);
```
Menampilkan PID, nama command, waktu CPU, dan memori proses dalam format tabel.

#### 10. Menutup direktori
```
closedir(proc_dir);
```
Menutup direktori ```/proc``` setelah selesai membaca semua proses.


### B. Memasang mata-mata dalam mode daemon
```
FILE *log_file = NULL;

void log_process(const char* process_name, const char* status, const char* username) {
    if (!log_file) {
        char log_path[256];
        snprintf(log_path, sizeof(log_path), "/home/dinda/debugmon.log", username);
        
        log_file = fopen(log_path, "a");
        if (!log_file) {
            perror("Failed to open debugmon.log");
            return;
        }
    }

    time_t now = time(NULL);
    struct tm* t = localtime(&now);

    fprintf(log_file, "[%02d/%02d/%04d %02d:%02d:%02d] %s : %s\n",
        t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
        t->tm_hour, t->tm_min, t->tm_sec,
        process_name, status);

    fflush(log_file);
}

void cleanup_log() {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

void signal_handler(int sig) {
    cleanup_log();
    exit(0);
}

void daemon_mode(const char* user) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("First fork failed");
        exit(1);
    }
    if (pid > 0) exit(0);

    if (setsid() < 0) {
        perror("setsid failed");
        exit(1);
    }

    signal(SIGHUP, SIG_IGN);
    signal(SIGTERM, signal_handler);

    pid = fork();
    if (pid < 0) {
        perror("Second fork failed");
        exit(1);
    }
    if (pid > 0) exit(0);

    umask(0);
    chdir("/");

    // Alihkan file descriptor ke /dev/null
    int fd = open("/dev/null", O_RDWR);
    if (fd != -1) {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        if (fd > 2) close(fd);
    }

    while (1) {
        struct dirent* entry;
        DIR* dir = opendir("/proc");
        if (!dir) {
            log_process("debugmon", "ERROR: Failed to open /proc", user);
            cleanup_log();
            sleep(10);
            continue;
        }

        while ((entry = readdir(dir)) != NULL) {
            if (isdigit(entry->d_name[0])) {
                char path[256], cmdline[256];
                snprintf(path, sizeof(path), "/proc/%s/comm", entry->d_name);

                FILE* fp = fopen(path, "r");
                if (fp) {
                    if (fgets(cmdline, sizeof(cmdline), fp)) {
                        cmdline[strcspn(cmdline, "\n")] = '\0';
                        log_process(cmdline, "RUNNING", user);
                    }
                    fclose(fp);
                }
            }
        }

        closedir(dir);
        sleep(10);
    }
}
```
> Output
> ![WhatsApp Image 2025-04-18 at 17 18 19_fcb8acdf](https://github.com/user-attachments/assets/61a6448a-467a-40cf-bab3-788f0d6efd5c)

### > Penjelasan

#### 1. Logging aktivitas proses
```
void log_process(const char* process_name, const char* status, const char* username) {
    if (!log_file) {
        char log_path[256];
        snprintf(log_path, sizeof(log_path), "/home/dinda/debugmon.log", username);
        log_file = fopen(log_path, "a");
        if (!log_file) {
            perror("Failed to open debugmon.log");
            return;
        }
    }

    time_t now = time(NULL);
    struct tm* t = localtime(&now);

    fprintf(log_file, "[%02d/%02d/%04d %02d:%02d:%02d] %s : %s\n",
        t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
        t->tm_hour, t->tm_min, t->tm_sec,
        process_name, status);

    fflush(log_file);
}
```
- Akan membuka file log ```(/home/dinda/debugmon.log)``` untuk mencatat aktivitas proses.

- Akan menulis informasi log berupa waktu, nama proses, dan statusnya (misalnya "RUNNING").

- File log dibuka dalam mode append agar data sebelumnya tidak terhapus.

#### 2. Membersihkan file log saat keluar
```
void cleanup_log() {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}
```
Akan menutup file log jika sudah selesai digunakan, memastikan tidak ada kebocoran resource.

#### 3. Menangani sinyal untuk keluar dengan bersih
```
void signal_handler(int sig) {
    cleanup_log();
    exit(0);
}
```
Ketika proses menerima sinyal seperti SIGTERM, file log ditutup dengan bersih sebelum program keluar.

#### 4. Mode daemon
```
void daemon_mode(const char* user) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("First fork failed");
        exit(1);
    }
    if (pid > 0) exit(0);

    if (setsid() < 0) {
        perror("setsid failed");
        exit(1);
    }

    signal(SIGHUP, SIG_IGN);
    signal(SIGTERM, signal_handler);

    pid = fork();
    if (pid < 0) {
        perror("Second fork failed");
        exit(1);
    }
    if (pid > 0) exit(0);

    umask(0);
    chdir("/");
}
```
Fungsi ini membuat proses menjadi daemon

- Fork pertama: Mengakhiri proses induk agar daemon berjalan di background.

- Fork kedua: Menghindari daemon menjadi pemimpin sesi.

Lalu, akan memutuskan hubungan dengan terminal dan mengatur direktori kerja ke root ```(chdir("/"))```.

#### 5. Pengalihan file descriptor
```
int fd = open("/dev/null", O_RDWR);
if (fd != -1) {
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    if (fd > 2) close(fd);
}
```

Mengalihkan input, output, dan error ```ke /dev/null``` agar daemon tidak mengganggu terminal.

#### 6. Pemantauan proses di ```/proc```
```
while (1) {
    struct dirent* entry;
    DIR* dir = opendir("/proc");
    if (!dir) {
        log_process("debugmon", "ERROR: Failed to open /proc", user);
        cleanup_log();
        sleep(10);
        continue;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (isdigit(entry->d_name[0])) {
            char path[256], cmdline[256];
            snprintf(path, sizeof(path), "/proc/%s/comm", entry->d_name);

            FILE* fp = fopen(path, "r");
            if (fp) {
                if (fgets(cmdline, sizeof(cmdline), fp)) {
                    cmdline[strcspn(cmdline, "\n")] = '\0';
                    log_process(cmdline, "RUNNING", user);
                }
                fclose(fp);
            }
        }
    }

    closedir(dir);
    sleep(10);
}
```
Akan membuka direktori ```/proc``` untuk membaca semua proses yang sedang berjalan.

Untuk setiap proses:

- Membaca nama proses dari file ```/proc/[PID]/comm```.

- Menuliskan aktivitas proses (misalnya, status "RUNNING") ke file log.

Lalu proses ini dilakukan terus-menerus setiap 10 detik.

### C. Menghentikan pengawasan

### D. Menggagalkan semua proses user yang sedang berjalan

### E. Mengizinkan user untuk kembali menjalankan proses

### F. Mencatat ke dalam file log
