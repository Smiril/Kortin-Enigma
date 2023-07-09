#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))

static void m(void *array, size_t n, size_t size) {
    char tmp[size];
    char *arr = array;
    size_t stride = size * sizeof(char);

    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; ++i) {
            size_t rnd = (size_t) rand();
            size_t j = i + rnd / (RAND_MAX / (n - i) + 1);

            memcpy(tmp, arr + j * stride, size);
            memcpy(arr + j * stride, arr + i * stride, size);
            memcpy(arr + i * stride, tmp, size);
        }
    }
}

#define print_type(count, stmt) \
    do { \
    printf("["); \
    for (size_t i = 0; i < (count); ++i) { \
        stmt; \
    } \
    printf("]\n"); \
    } while (0)

int main(void) {
    srand(time(NULL));
    char arr[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    //printf("          ");
    //print_type(NELEMS(arr), printf("%c", arr[i]));
    m(arr, NELEMS(arr), sizeof(arr[0]));
    printf("Rotor1    ");
    print_type(NELEMS(arr), printf("%c", arr[i]));
    m(arr, NELEMS(arr), sizeof(arr[0]));
    printf("Rotor2    ");
    print_type(NELEMS(arr), printf("%c", arr[i]));
    m(arr, NELEMS(arr), sizeof(arr[0]));
    printf("Rotor3    ");
    print_type(NELEMS(arr), printf("%c", arr[i]));
    m(arr, NELEMS(arr), sizeof(arr[0]));
    printf("Rotor4    ");
    print_type(NELEMS(arr), printf("%c", arr[i]));
    m(arr, NELEMS(arr), sizeof(arr[0]));
    printf("Rotor5    ");
    print_type(NELEMS(arr), printf("%c", arr[i]));
    m(arr, NELEMS(arr), sizeof(arr[0]));
    printf("Reflector ");
    print_type(NELEMS(arr), printf("%c", arr[i]));
    m(arr, NELEMS(arr), sizeof(arr[0]));
    printf("NOTch     ");
    print_type(5, printf("%c", arr[i]));

    return 0;
}
