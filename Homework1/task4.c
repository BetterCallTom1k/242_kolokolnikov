#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

static Node *reverse(Node *head) {
    Node *prev = NULL;
    while (head) {
        Node *next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }
    return prev;
}

int main() {
    Node *head = NULL, *tail = NULL;
    int val;
    while (scanf("%d", &val) == 1 && val != 0) {
        Node *node = malloc(sizeof(Node));
        node->value = val;
        node->next = NULL;
        if (tail) tail->next = node;
        else head = node;
        tail = node;
    }

    head = reverse(head);

    for (Node *cur = head; cur; cur = cur->next) {
        if (cur != head) printf(" ");
        printf("%d", cur->value);
    }
    printf("\n");

    while (head) {
        Node *next = head->next;
        free(head);
        head = next;
    }
    return 0;
}
