#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name;
    int studentID;
    char *major;
} Student;

int main(void)
{
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");
    int student_count;
    fscanf(input, "%d", &student_count);

    Student *list = malloc(sizeof(Student) * student_count);
    for (int i = 0; i < student_count; i++) {
        list[i].major = malloc(sizeof(char) * 31);
        list[i].name = malloc(sizeof(char) * 31);
        fscanf(input, "%s %d %s", list[i].name, &list[i].studentID, list[i].major);
    }
    for (int i = 0; i < student_count; i++)
        fprintf(output, "%s %d %s\n", list[i].name, list[i].studentID, list[i].major);
    for (int i = 0; i < student_count; i++) {
        free(list[i].major);
        free(list[i].name);
    }
    free(list);

    fclose(input);
    fclose(output);
    return 0;
}