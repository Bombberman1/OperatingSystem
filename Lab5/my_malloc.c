#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#define ARENA_SIZE 10240
#define DIVIDER "********************"


void* arena;
struct LinkedList* list;

struct Node {
    int size;
    int* position;
    unsigned char free;

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

void add_node(int size, int* position, unsigned char free) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    if(list -> start == NULL) {
        node -> size = size;
        node -> position = position;
        node -> free = free;
        node -> prev = NULL;
        node -> next = NULL;

        list -> start = node;
        list -> end = node;
    } else {
        if(size > ARENA_SIZE) {
            printf("Error, no memory");
            return;
        }
        node -> size = size;
        if(list -> start == list -> end) node -> position = list -> start -> position;
        else node -> position = (int*)((char*)(list -> end -> position) + (list -> end -> size) + 1);
        node -> free = free;
        node -> prev = list -> end;
        node -> prev -> next = node;
        node -> next = NULL;

        list -> start -> position = (int*)((char*)(list -> start -> position) + (node -> size) + 1);
        list -> start -> size -= (node -> size) + 1;
        list -> end -> next = node;
        list -> end = node;
    }
}

void* get_node() {
    return (void*)(list -> end -> position);
}

void delete_node(int* position) {  
    struct Node* temp = list -> start;
    
    if(list -> start == list -> end) {
        printf("You can\'t delete arena\n");
        return;
    }

    while(temp) {
        if(temp -> position == position) temp -> free = 1;
        while(temp && temp -> free == 1 && temp -> prev && temp -> prev -> free == 1) {
            if(temp -> prev && temp -> prev == list -> start && temp -> next == NULL) {
                list -> start = list -> start -> next;
                list -> start -> position = (int*)((char*)(list -> start -> prev -> position) - (list -> start -> size) - 1);
                list -> start -> size += list -> start -> prev -> size + 1;
                free(list -> start -> prev);
                list -> start -> prev = NULL;
                temp = list -> start;
            } else if(temp -> next == NULL) {
                temp -> prev -> size += temp -> size + 1;
                list -> end = list -> end -> prev;
                free(list -> end -> next);
                list -> end -> next = NULL;
                temp = list -> end;
            } else {
                if(temp -> prev == list -> start) break;
                temp -> prev -> size += temp -> size + 1;
                temp -> prev -> next = temp -> next;
                temp -> next -> prev = temp -> prev;
                free(temp);
                temp = temp -> next;
            }
        }

        temp = temp -> next;
    }
    
    if(list -> end -> free == 1 && list -> start -> free == 1 && list -> end != list -> start) {
        list -> start -> position = (int*)((char*)(list -> start -> position) - (list -> end -> size) - 1);
        list -> start -> size += list -> end -> size + 1;
        list -> end = list -> end -> prev;
        free(list -> end -> next);
        list -> end -> next = NULL;
    }
}

void print_list_start() {
    struct Node* temp = list -> start;
    int counter = 0;
    int free_size = 0;
    int free_counter = 0;
    int occupied_size = 0;
    int occupied_counter = 0;

    printf("%s\n", DIVIDER);
    while(temp) {
        printf("Area: %d,  Size: %d,  Start: %p,  End: %p,  Free: %d\n",
            counter,
            temp -> size,
            temp -> position,
            (char*)(temp -> position) + (temp -> size),
            temp -> free);
        counter++;
        
        if(temp -> free == 1) {
            free_size += temp -> size;
            free_counter++;
        } else {
            occupied_size += temp -> size;
            occupied_counter++;
        }
        
        temp = temp -> next;
    }
    
    printf("\n");
    printf("Free areas: %d,  Size: %d\n", free_counter, free_size);
    printf("Occupied areas: %d,  Size: %d\n", occupied_counter, occupied_size);
    printf("%s\n", DIVIDER);
}

void init_malloc() {
    list = create_list();
    arena = mmap(NULL, ARENA_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(arena == MAP_FAILED) {
        printf("Arena Error\n");
        return;
    }
    add_node(ARENA_SIZE, (int*)arena, 1);
    
    printf("%s\n", DIVIDER);
    printf("%s\n", DIVIDER);
    printf("Arena Memory Start: %p,  Size: %d\n", (int*)arena, ARENA_SIZE);
    printf("%s\n", DIVIDER);
}

void* oleg_malloc(int count) {
    add_node(count, 0, 0);
    print_list_start();
    return get_node();
}

void oleg_free(void* ptr) {
    delete_node((int*)ptr);
    print_list_start();
}

int main() {
    init_malloc();
    int* arr1 = oleg_malloc(4 * sizeof(int));
    int* arr2 = oleg_malloc(4 * sizeof(int));
    int* arr3 = oleg_malloc(8 * sizeof(int));
    int* arr4 = oleg_malloc(3 * sizeof(int));
    int* arr5 = oleg_malloc(6 * sizeof(int));
    oleg_free(arr5);
    oleg_free(arr4);
    oleg_free(arr1);
    oleg_free(arr3);
    oleg_free(arr2);
}
