#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct id3v2 {
    FILE *file;
    int size;
    unsigned char *tag;
} id3v2;


int frame_search(const char *prop_name, id3v2 mp3) {
    for(int i = 0; i < mp3.size; ++i) {
        int flag = 1;
        for (int j = 0; j < 4; ++j) {
            if (prop_name[0 + j] != mp3.tag[i + j]){
                flag = 0;
            }
        }
        if (flag) {
            return i;
        }
    }
    return 1;
}


void show(id3v2 mp3) {
    printf("\n");
    for (int i = 0; i < mp3.size; ++i) {
        unsigned char* pointer = mp3.tag + i;
        int flag = 1;
        for (int j = 0; j < 4; ++j)
            if (!(((pointer)[j] >= 'A' && pointer[j] <= 'Z') || ((pointer)[j] >= '0' && (pointer)[j] <= '9'))){
                flag = 0;
            }
        for (int j = 4; j < 8; ++j)
            if ((pointer)[j] >= 128){
                flag = 0;
            }
        if (flag == 1) {
            for (int j = 0; j < 4; ++j){
                printf("%c", mp3.tag[i + j]);
            }
            printf(":");
            int size = 0;
            size += mp3.tag[i + 4] << 21;
            size += mp3.tag[i + 5] << 14;
            size += mp3.tag[i + 6] << 7;
            size += mp3.tag[i + 7];
            for (int j = i + 10; j < i + 10 + size; ++j)
                printf("%c", mp3.tag[j]);
            printf("\n");
            i += size + 9;
        }
    }
}


void get(char *prop_name, id3v2 mp3) {
    int pos = frame_search(prop_name, mp3);
    if (pos == 1) printf("Frame cannot be found");
    else {
        int size = 0;
        for(int i = 0; i < 4; i++) {
            size = size + (mp3.tag[pos + i + 4] << (7 * (3 - i))); //сдвигаем побитово
        }
        printf("\nThe value of the frame %s is", prop_name);
        for (int i = pos + 10; i < pos + 10 + size; ++i)
            printf("%c", mp3.tag[i]);
        printf("\n");
    }
}


void set(id3v2 mp3, char *prop_name, char *prop_value) {
    int pos = frame_search(prop_name, mp3);
    if (pos == 1){
        printf("Frame cannot be found");
    }
    else {
        unsigned long long new_size = 11 + strlen(prop_value);
        int size = 0;
        for(int i = 0; i < 4; i++) {
            size = size + (mp3.tag[pos + i + 4] << (7 * (3 - i))); //сдвигаем побитово
        }
            size += 10;
        char *constant = malloc(mp3.size - (pos + size));
        memcpy(constant, mp3.tag + pos + size, mp3.size - (pos + size));

        for(int i = 0; i < 4; ++i) {
            mp3.tag[pos + i] = prop_name[i];
        }
        mp3.tag[pos + 4] = ((new_size - 10) >> 21) % 128;
        mp3.tag[pos + 5] = ((new_size - 10) >> 14) % 128;
        mp3.tag[pos + 6] = ((new_size - 10) >> 7) % 128;
        mp3.tag[pos + 7] = (new_size - 10) % 128;
        mp3.tag[pos + 8] = 0;
        mp3.tag[pos + 9] = 0;
        mp3.tag[pos + 10] = 0;

        for (unsigned long long i = pos + 11; i < pos + new_size; ++i){
            mp3.tag[i] = prop_value[i - pos - 11];
        }
        for (unsigned long long i = pos + new_size; i < mp3.size; ++i){
            mp3.tag[i] = constant[i - pos - new_size];
        }

        free(constant);
        fseek(mp3.file, 0, SEEK_SET);
        for (int i = 0; i < mp3.size; ++i)
            fputc(mp3.tag[i], mp3.file);
    }
    printf("\nThe content of frame %s has been successfully changed for %s\n", prop_name, prop_value);
}


int main(int argc, char *argv[]) {
    char* filename;
    id3v2 mp3;
    if (!strncmp(argv[1],"--filepath=",11)) {
        filename = strpbrk(argv[1], "=") + 1;         // link for next elem after '='
        if ((mp3.file = fopen(filename,"r+")) == NULL) {
            printf("Couldn't open file %s for reading.\n", filename);
            return 0;
        }
    }
    else {
        printf("Command '--filepath= didn't' recognized.\n");
        return 0;
    }
    if (!strncmp(argv[2],"--show",6)) {
        printf("\nFormat of metadata: ");
    }
    for (int i = 0; i < 3; ++i) {
        unsigned char c = fgetc(mp3.file);
        if (!strncmp(argv[2],"--show",6)) {printf("%c", c);}
    }
    unsigned char byte1 = fgetc(mp3.file);
    unsigned char byte2 = fgetc(mp3.file);
    unsigned char byte3 = fgetc(mp3.file);
    unsigned char frame_size[4];
    int bias = 0;
    for(int i = 0; i < 4; i++) {
        frame_size[i] = fgetc(mp3.file);
        bias = bias + (frame_size[i] << (7 * (3 - i))); //сдвигаем побитово
    }

    mp3.size = 10 + bias;

    mp3.tag = (unsigned char*) malloc(mp3.size * sizeof(unsigned char));
    for (int i = 0; i < 6; i++){
        mp3.tag[i] = 0;
    }
    mp3.tag[0] = 'I';
    mp3.tag[1] = 'D';
    mp3.tag[2] = '3';
    mp3.tag[3] = byte1;
    mp3.tag[4] = byte2;
    mp3.tag[5] = byte3;

    for (int i = 6; i < 10; ++i){
        mp3.tag[i] = frame_size[6 - i];
    }

    for (int i = 10; i < mp3.size; ++i) {
        unsigned char c = fgetc(mp3.file);
        if (c != 1) mp3.tag[i] = c;
    }

    if (strcmp(argv[2], "--show") == 0){
        show(mp3);
    }
    else if (strncmp("--get=", argv[2], 6) == 0) {
        char* prop_name = strpbrk(argv[2], "=") + 1;
        get(prop_name, mp3);
        }
    else if (strncmp("--set=", argv[2], 6) == 0) {
        char* prop_name = strpbrk(argv[2], "=") + 1;
        char* prop_value = strpbrk(argv[3], "=") + 1;
            set(mp3, prop_name, prop_value);
        }
    else {
        printf("Unknown expression");
    }
    return 0;
}