#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    FILE *fin;
    fin = fopen("C:\\Users\\fedor\\Desktop\\access_log_Jul95", "r");
    size_t n = 500 * sizeof(char);
    char line[500];
    int count = 0;
    fgets(line, n, fin);
    while (fgets(line, n, fin) != NULL) {
        for (int i = 0; i < strlen(line); i++) {
            if ((line[i] == ' ') && (line[i + 1] == '5') && (line[i + 4] == ' ')) {
                printf("%s", line);
                count++;
            }
        }
    }
    printf("%d", count);
    int time;
    time = 5;
    int day;
    char month[4];
    int flag = 0;
    fin = fopen("C:\\Users\\fedor\\Desktop\\access_log_Jul95", "r");
    while (fgets(line, n, fin) != NULL) {
        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == ']') {
                if (line[i + 1] != 0) {
                    day = atoi(line[i + 2]);
                } else {
                    day = atoi(line[i + 1]) * 10 + atoi(line[i + 2]);
                }
            }
            if ((line[i] == '/') && (flag == 0)) {
                flag++;
                month[0] = line[i + 1]
            }

        }

    }
}