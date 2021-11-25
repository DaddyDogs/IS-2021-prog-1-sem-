#include <stdio.h>
#include <inttypes.h>
#include <string.h>


const uint64_t base = 4294967296;

typedef struct UINT1024_T {
    unsigned int num[32];
} uint1024_t;

uint1024_t from_uint(unsigned int x) {
    uint1024_t result;
    for (int i = 0; i < 32; i++) {
        result.num[i] = 0;
    }
    result.num[0] = x;
    return result;
}

uint1024_t add_op(uint1024_t x, uint1024_t y) {
    uint1024_t result;
    int buff = 0;
    for (int i = 0; i < 32; i++) {
        result.num[i] = ((uint64_t) x.num[i] + (uint64_t) y.num[i] + buff) % base;
        buff = ((uint64_t) x.num[i] + (uint64_t) y.num[i] + buff) / base;
    }
    return result;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
    uint1024_t result;
    int buff = 0;
    for (int i = 0; i < 32; i++) {
        if (x.num[i] - buff >= y.num[i])
        {
            result.num[i] = (uint64_t) x.num[i] - buff - (uint64_t) y.num[i];
            buff = 0;
        }
        else
        {
            result.num[i] = (uint64_t) x.num[i] + base - (uint64_t) y.num[i] - buff;
            buff = 1;
        }

    }
    return result;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y) {
    uint1024_t result = from_uint(0);
    for (int i = 0; i < 32; i++) {
        uint1024_t buff = from_uint(0);
        uint64_t super_buff = 0;
        for (int j = 0; j < 32; j++) {
            buff.num[j] = (x.num[j] * y.num[i] + super_buff) % base;
            super_buff = ((uint64_t) x.num[j] * (uint64_t) y.num[i] + super_buff) / base;
        }
        for (int j = 0; j < i; j++) {
            for (int q = 31; q > 0; q--) {
                buff.num[q] = buff.num[q - 1];
            }
            buff.num[0] = 0;
        }
        result = add_op(result, buff);
    }
    return result;
}

void printf_value(uint1024_t x) {
    uint1024_t res = from_uint(0);
    uint1024_t z = from_uint(0);
    char line[310];
    memset(line, '\0', 310);
    while (memcmp(&x, &z, sizeof(int) * 32) != 0) {
        uint64_t buff = 0;
        for (int i = 31; i >= 0; i--) {
            uint64_t value = ((uint64_t) x.num[i] + (uint64_t) buff * (uint64_t) base);
            res.num[i] = value / 10;
            buff = value % 10;
        }
        line[strlen(line)] = buff + '0';
        x = res;
    }
    for (int i = strlen(line) - 1; i >= 0; i--) {
        printf("%c", line[i]);
    }
}


void scanf_value(uint1024_t *x) {
    char line[310];
    uint1024_t y;
    for (int i = 0; i < 32; i++) {
        y.num[i] = 0;
    }
    scanf("%s", line);
    for (int i = 0; i < strlen(line); i++) {
        y = mult_op(y, from_uint(10));
        y = add_op(y, from_uint(line[i] - '0'));
    }
    for (int i = 0; i < 32; i++) {
        x->num[i] = y.num[i];
    }
}

int main() {
uint1024_t x;
scanf_value(&x);
uint1024_t y;
scanf_value(&y);

uint1024_t number2 = from_uint(4366559);
uint1024_t number3 = from_uint(4564889);


printf_value(mult_op(x, y));
printf("\n");

return 0;
}
