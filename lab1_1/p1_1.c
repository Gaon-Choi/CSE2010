#include <stdio.h>
#include <stdlib.h>

int numeric_limit_int_max()
{
    return 1 << (sizeof(int) * 8 - 2);
}
void selection_sort(int *arr, int size)
{
    int i, j;
    for (int i = 0; i < size; i++)
    {
        int index = -1;
        int min = numeric_limit_int_max();
        // maximum integer
        for (j = i; j < size; j++)
        {
            if (min > arr[j])
            {
                min = arr[j];
                index = j;
            }
        }
        int temp = arr[i];
        arr[i] = arr[index];
        arr[index] = temp;
    }
}
int main(void)
{
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");
    int count;
    fscanf(input, "%d", &count);

    int *arr = malloc(sizeof(int) * count);
    for (int i = 0; i < count; i++)
        fscanf(input, "%d", &arr[i]);
    selection_sort(arr, count);
    for (int i = 0; i < count; i++)
        fprintf(output, "%d ", arr[i]);
    fprintf(output, "\n");
    fclose(input);
    fclose(output);

    return 0;
}