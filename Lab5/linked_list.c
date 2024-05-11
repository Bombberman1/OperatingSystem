#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define COUNT 10


struct Node {
    int number;

    struct Node* prev;
    struct Node* next;
};

struct LinkedList {
    struct Node* start;
    struct Node* end;
};

struct LinkedList* create_list() {
    struct LinkedList* list = (struct LinkedList*)malloc(sizeof(struct LinkedList));
    list -> start = NULL;
    list -> end = NULL;

    return list;
}

void add_node(struct LinkedList* list, int number) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node -> number = number;

    if(list -> start == NULL) {
        node -> prev = NULL;
        node -> next = NULL;

        list -> start = node;
        list -> end = node;
    } else {
        node -> prev = list -> end;
        node -> prev -> next = node;
        node -> next = NULL;

        list -> end -> next = node;
        list -> end = node;
    }
}

void delete_node(struct LinkedList* list, int number) {
    struct Node* temp = list -> start;

    if(list -> start == list -> end) {
        free(list -> start);
        list -> start = NULL;
        list -> end = NULL;
        printf("Now list is empty\n");

        return;
    } 

    while(temp) {
        if(temp -> number == number) {
            if(temp -> next == NULL) {
                list -> end = list -> end -> prev;
                free(list -> end -> next);
                list -> end -> next = NULL;
                temp = list -> end;
            } else if(temp -> prev == NULL) {
                list -> start = list -> start -> next;
                free(list -> start -> prev);
                list -> start -> prev = NULL;
                temp = list -> start;
            } else {
                temp -> prev -> next = temp -> next;
                temp -> next -> prev = temp -> prev;
                free(temp);
            }

            return;
        }

        temp = temp -> next;
    }

    printf("This node not found\n");
}

void print_list_start(struct LinkedList* list) {
    struct Node* temp = list -> start;

    while(temp) {
        printf("%d\n", temp -> number);
        temp = temp -> next;
    }
}

void print_list_end(struct LinkedList* list) {
    struct Node* temp = list -> end;

    while(temp) {
        printf("%d\n", temp -> number);
        temp = temp -> prev;
    }
}

void generate_nodes(struct LinkedList* list) {
    for(int i = 0; i < COUNT; i++) {
        add_node(list, rand() % 1000);
    }
    print_list_start(list);
}

void delete_nodes(struct LinkedList* list) {
    int number = 0;
    printf("Enter Node number to delete: ");
    while(scanf("%d", &number) > 0) {
        delete_node(list, number);
        printf("Enter Node number to delete: ");
    }
    printf("*****************\n");
    print_list_start(list);
}

int main() {
    struct LinkedList* list = create_list();

    srand(time(NULL));

    generate_nodes(list);
    printf("*****************\n");
    delete_nodes(list);
}
