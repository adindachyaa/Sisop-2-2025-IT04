# PRAKTIKUM SISOP-2-2025-MH-IT04

KELOMPOK IT04
| No | Nama                              | NRP         |
|----|-----------------------------------|-------------|
| 1  | Raya Ahmad Syarif                 | 5027241041  |
| 2  | Salsa Bil Ula         | 5027241052 |
| 3  | Adinda Cahya Pramesti   | 5027241117  |

# Pengantar

Laporan resmi ini dibuat untuk praktikum modul 2 yang dilaksanakan pada tanggal 7 Maret 2025 sampai dengan 11 Maret 2025. Praktikum Modul 2 ini terdiri dari empat soal dan dikerjakan oleh tiga orang anggota dengan pembagian tertentu.

Berikut ini pembagian pengerjaan dari kelompok IT04:

    Soal 1 dikerjakan oleh Raya Ahmad Syarif
    Soal 2 dikerjakan oleh Salsa Bil Ula
    Soal 3 dikerjakan oleh Adinda Cahya Pramesti
    Soal 4 dibagi menjadi 3 bagian:
    - 4a & 4b dikerjakan oleh Adinda Cahya Pramesti
    - 4c & 4d dikerjakan oleh Raya Ahmad Syarif
    - 4e & 4f dikerjakan oleh Salsa Bil Ula

Sehingga dengan demikian, Pembagian bobot pengerjaan soal menjadi (Raya 33.3%, Salsa 33.3%, Dinda 33.3%).

Kelompok IT04 juga telah menyelesaikan tugas praktikum modul 2 yang telah diberikan dan telah melakukan demonstrasi kepada Asisten lab. Dari hasil ikum yang telah dilakukan sebelumnya, maka diperoleh hasil sebagaimana yang dituliskan pada setiap bab di bawah ini.
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



