#include <stdio.h>
#include <stdlib.h>

int random_get_num_in_range(int start, int end)
{
    return rand() % (end + 1 - start) + start;
}

void array_set_random_value(
    int* arr, int n, int start_range, int end_range)
{
    for (int i = 0; i < n; ++i) {
        arr[i] = random_get_num_in_range(start_range, end_range);
    }
}

void array_int_print(int* arr, int n)
{
    for (int i = 0; i < n; ++i) {
        printf("%d ", arr[i]);
    }
}

int* array_int_get(int n) { return (int*)malloc(sizeof(int) * n); }

void array_free(void* arr) { free(arr); }

void array_2d_set_random_value(
    int** arr, int n, int m, int start_range, int end_range)
{
    for (int i = 0; i < n; ++i) {
        array_set_random_value(arr[i], m, start_range, end_range);
    }
}

void array_2d_int_print(int** arr, int n, int m)
{
    for (int i = 0; i < n; ++i) {
        array_int_print(arr[i], m);
        printf("\n");
    }
}

int** array_2d_int_get(int n, int m)
{
    int** arr = (int**)malloc(sizeof(int*) * n);
    for (int i = 0; i < n; ++i) {
        arr[i] = array_int_get(m);
    }
    return arr;
}

void array_2d_free(int** arr, int n)
{
    for (int i = 0; i < n; ++i) {
        array_free(arr[i]);
    }
}

int main()
{
    int* a = array_int_get(10);
    array_set_random_value(a, 10, 0, 10);
    array_int_print(a, 10);
    array_free(a);
    printf("\n");

    int b_size_n = 10, b_size_m = 10;
    int** b = array_2d_int_get(b_size_n, b_size_m);
    array_2d_set_random_value(b, b_size_n, b_size_m, 0, 5);
    array_2d_int_print(b, b_size_n, b_size_m);
    array_2d_free(b, b_size_n);
    int wait; scanf("%d", &wait);
    return 0;
}
