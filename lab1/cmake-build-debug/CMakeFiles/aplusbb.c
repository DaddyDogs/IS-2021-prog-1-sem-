#include <stdio.h>

int main()
{
    int a, b
    FILE *fin, fout;
    fin = fopen("aplusbb.in", "r");
    fout = fopen("aplusbb.out", "w");
    fscanf(fin, "%d %d", &a, &b);
    b = pow(b, 2);
    fprintf(fout, "%d", a + b)
    fclose(fin);
    fclose(fout);
};

