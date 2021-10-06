#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Person {
    int age;
    char *name;
} Person;

Person *person_new(char *name, int age) {
    Person *person = (Person *) malloc(sizeof(Person));
    person->name = name;
    person->age = age;
    return person;
}

void person_free(Person *person) {
    free(person->name);
    free(person);
}

void person_print(const Person *p) {
    printf("%s %d", p->name, p->age);
}

typedef void ListType;
typedef struct List {
    ListType **data;
    size_t size;
    size_t capacity;
} List;

List *list_new() {
    List *list = (List *) malloc(sizeof(List));
    if (list == NULL) return list;
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
    return list;
}

void list_free(List *list, void (*func) (ListType *)) {
    if (func != NULL) {
        for (int i = 0; i < list->size; ++i) {
            func(list->data[i]);
        }
    }
    free(list->data);
    free(list);
}

void list_push(List *list, ListType *p_value) {
    if (list->data == NULL) {
        list->capacity = 10;
        list->data = (ListType **) malloc(
                sizeof(ListType *) * list->capacity);
    }
    if (list->size >= list->capacity) {
        list->capacity = (list->capacity + 1) * 2;
        list->data = (ListType **) realloc(list->data,
                    sizeof(ListType *) * list->capacity);
    }
    list->data[list->size++] = p_value;
}

size_t list_get_size(List *list) {
    return list->size;
}

ListType *list_get(List *list, size_t index) {
    return list->data[index];
}

char *list_to_str(List *list) {
    char *str = (char *) malloc(list->size + 1);
    for (int i = 0; i < list_get_size(list); ++i) {
        str[i] = *((char *) list_get(list, i));
    }
    str[list->size] = '\0';
    return str;
}

char *readline(FILE *filestream) {
    List *list = list_new();
    char c;
    int i = 0;
    while (true) {
        c = fgetc(filestream);
        if (c == '\n' || c == EOF || c == '\0') {
            break;
        }
        char *c_for_push = (char *) malloc(sizeof(char));
        *c_for_push = c;
        list_push(list, c_for_push);
    }
    char *str = list_to_str(list);
    list_free(list, free);
    return str;
}

int main() {
    List *persons = list_new();
    
    size_t buffer_len_max = 100;
    char *age_str = (char *) malloc(sizeof(char) * buffer_len_max);
    for (int i = 0;;) {
        char *name = NULL;
        int age = 0;

        printf("Write name:\n");
        name = readline(stdin);
        if (strlen(name) == 0) {
            free(name);
            break;
        }

        printf("Write age:\n");
        fgets(age_str, buffer_len_max, stdin);
        age = atoi(age_str);

        if (age == 0) {
            break;
        }

        list_push(persons, person_new(name, age));
    }
    free(age_str);

    for (int i = 0; i < list_get_size(persons); ++i) {
        person_print(list_get(persons, i));
        printf("\n");
    }

    list_free(persons, (void (*) (ListType *)) person_free);

    return EXIT_SUCCESS;
}
