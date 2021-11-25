#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int CHAR_COUNT = 400;

int main() {
    FILE* fin;
    FILE* fp;
    fp = fopen("tekst.txt", "w");
    fin = fopen("access_log_Jul95", "r");
    size_t n = CHAR_COUNT * sizeof(char);
    char* line = (char*) malloc(CHAR_COUNT);
    size_t count = 0;
    int counter = 0;
    printf("Requests with 5xx fault:");
    while (fgets(line, n, fin) != NULL) {
        counter++;
        for (int i = 70; i < strlen(line); i++) {
            if ((line[i] == ' ') && (line[i + 1] == '5') && (line[i + 4] == ' ')) {
                printf("%s\n", line);
                count++;
            }
        }
    }

    printf("Quantity of requests with 5xx fault = %llu\n", count);
    free(line);
    int day = 0, hour = 0, minutes = 0, seconds = 0, time = 0;
    int first = 0, last = 0, maximum = 0;
    int parametr;
    count = 0;
    int j = 0;
    char* lines = (char*)malloc(CHAR_COUNT);
    int* times = (int*) malloc(counter * sizeof(int));
    if (times == NULL) exit(1);
    memset(times, '0', counter * sizeof(int));
    printf("Enter time in seconds\n");
    scanf("%d", &parametr);
    printf("\n");
    fin = fopen("access_log_Jul95", "r");


    while (fgets(lines, n, fin) != NULL) {
        int flag = 0;
        for (int i = 15; i < strlen(lines); i++) {
            if (lines[i] == '[') {
                day = (lines[i + 1] - '0') * 10 + (lines[i + 2] - '0');
            }
            else if ((lines[i] == ':') && (flag == 0)) {
                flag++;
                hour = (lines[i + 1] - '0') * 10 + lines[i + 2] - '0';
            }
            else if ((lines[i] == ':') && (flag == 1)) {
                flag++;
                minutes = (lines[i + 1] - '0') * 10 + lines[i + 2] - '0';
            }
            else if ((lines[i] == ':') && (flag == 2)) {
                flag = -10;
                seconds = (lines[i + 1] - '0') * 10 + (lines[i + 2] - '0');
            }
            else if (flag == -10) {
                continue;
            }
        }

        time = hour * 3600 + minutes * 60 + seconds + (day - 1) * 24 * 3600;
        times[j] = time;
        j++;
    }
        while(last <= counter) {
            while (times[last] - times[first] > parametr) {
                first++;
            }
               if (last - first + 1> maximum)
                {
                    maximum = last - first + 1;
                }
            last++;
        }


    free(lines);
    free(times);
    printf("%d", maximum);
    return 0;
}
