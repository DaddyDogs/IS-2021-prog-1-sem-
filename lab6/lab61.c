#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    char* arc_name;
    if (!strncmp(argv[2],"--file",6)) {
        arc_name = argv[3];
    }
    else {
        printf("Command '--file' was not recognized.\n");
        exit(1);
    }
    if (!strncmp(argv[4],"--create",8)) {
        FILE* arc_file = fopen(arc_name, "wb");
        FILE* file;
        unsigned long long int size[128];
        for (int i = 5; i < argc; i++){
            file = fopen(argv[i], "rb");
            if (file == NULL) {
                printf("Cannot open file %s for reading.\n", argv[i]);
                exit(2);
            }
            fseek(file, 0, SEEK_END);
            size[i - 5] = ftell(file);
            fseek(file, 0, SEEK_SET);
            fclose(file);
            }
        for (int i = 0; i < argc - 5; i++) {
            fprintf(arc_file, "< %s : %llu >", argv[i + 5], size[i]);
        }
        fprintf(arc_file, "\n");
        for (int i = 5; i < argc; i++) {
            file = fopen(argv[i], "rb");
            int buff;
            while ((buff = getc(file)) != EOF){
                putc(buff, arc_file);
            }
            fclose(file);
        }
        printf("\n Zipping was successfully made \n");
    }
    else if(!strncmp("--extract", argv[4], 9)){
        FILE* archive_file = fopen(arc_name, "rb+wb");
        unsigned long long int  cur = 0;
        unsigned long long int  start = 0;
        int byte;
        while ((byte = getc(archive_file)) != EOF) {
            start++;
            if (byte == '\n')
                break;
        }
        fseek(archive_file, 0, SEEK_SET);
        char filename[128] = {0};
        unsigned long long int  filesize;
        FILE *file;
        while (fscanf(archive_file, "< %s : %llu >", filename, &filesize) != 0) {
            file = fopen(filename, "wb");
            if (file == NULL)
                break;
            cur = ftell(archive_file);
            fseek(archive_file, start, SEEK_SET);
            start += filesize;
            while (filesize-- > 0)
                putc((byte = getc(archive_file)), file);
            fseek(archive_file, cur, SEEK_SET);
            fclose(file);
        }
        printf("\nUnzipping was successfully made\n");
    }
    else if(!strcmp("--list", argv[4])){
        FILE* arc_file = fopen(arc_name, "rb");
        char filename[128] = {0};
        unsigned long long int  filesize;
        printf(":\nList of archived files:\n");
        int i = 0;
        while (fscanf(arc_file, "< %s : %llu >", filename, &filesize) != 0) {
            i ++;
            printf("%d. %s\n", i, filename);
        }
        fclose(arc_file);
    }
    else {
        printf("Command %s was not recognized", argv[4]);
        exit(1);
    }
}

