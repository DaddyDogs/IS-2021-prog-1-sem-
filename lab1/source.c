#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[]) {
    FILE *fin;
    fin = fopen(argv[2], "r");
    if (argc != 3) {
        printf("Not enough of arguments");
    }
    else if ((strcmp(argv[1], "-c") == 0) || (strcmp(argv[1], "--bytes") == 0)) {
        long count_bites;
        count_bites = 0;
        char symbole;
        symbole = fgetc(fin);
        while (symbole != EOF) {
            if (symbole == '\n') {
                count_bites = count_bites + 1;
            }
            count_bites = count_bites + 1;
            symbole = fgetc(fin);
        }
        printf("%ld\n", count_bites);
    } else if ((strcmp(argv[1], "-l") == 0) || (strcmp(argv[1], "--lines") == 0)) {
        int symbole, count_lines;
        count_lines = 0;
        symbole = 50;
        while (symbole != EOF) {
            symbole = fgetc(fin);
            if (symbole == '\n')
                ++count_lines;
        }
        printf("%d\n", count_lines + 1);
    } else if ((strcmp(argv[1], "-w") == 0) || (strcmp(argv[1], "--words") == 0)) {
        int symbole, count_words = 0, is_word;
        symbole = 50;
        while (symbole != EOF) {
            symbole = fgetc(fin);
            if (symbole == ' ' || symbole == '\n' || symbole == '\t' || symbole == EOF) {
                if (is_word == 1) {
                    is_word = 0;
                    ++count_words;
                }
            } else is_word = 1;
        }
        printf("%d\n", count_words);
    }
    else {
        printf("Invalid option");
    }
    fclose(fin);
    return 0;
}