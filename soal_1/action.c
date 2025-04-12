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
        printf("Can't find Filtered directory. \n");
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

