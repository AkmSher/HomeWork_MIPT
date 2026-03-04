#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct list {
    uint64_t address;
    size_t size;
    char comment[64];
    struct list *next;
} list;

uint64_t findMaxBlock(list *head) {
 
    if (head == NULL) {
        return 0;
    }

    list *current = head;
    list *maxNode = head;  

    while (current != NULL) {
        if (current->size > maxNode->size) {
            maxNode = current;
        }
        current = current->next;
    }

    return maxNode->address;
}

int main(void) {
    int n;
    scanf("%d", &n);

    if (n == 0) {
        printf("0\n");
        return 0;
    }

    list *head = NULL;
    list *tail = NULL;

    for (int i = 0; i < n; i++) {
        list *node = (list *)malloc(sizeof(list));
        
        scanf("%lu %zu", &node->address, &node->size);
        node->comment[0] = '\0';
        node->next = NULL;

        if (head == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

    printf("%lu\n", findMaxBlock(head));


    list *current = head;
    while (current != NULL) {
        list *tmp = current;
        current = current->next;
        free(tmp);
    }

    return 0;
}

