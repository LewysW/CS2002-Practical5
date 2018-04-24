#include "msg_queue.h"

// enqueue
void send_msg( MQueue *s, int value ) {
    Node* node = makeNode(value);

    if (is_empty(s)) {
        s->tail = s->head = node;
    } else {
        s->tail->next = node;
        s->tail = node;
    }

}

// dequeue
Node *read_msg(MQueue* s) {
    if (is_empty(s)) return s->head;

    Node* node = s->head;
    s->head = s->head->next;
    return node;
}

void initMQueue(MQueue* s) {
    s->head = s->tail = NULL;
}

bool is_empty(MQueue* s) {
    return (s->head == NULL);
}

void printMQueue(MQueue* s) {
    if (is_empty(s)) {
        printf("Empty queue.\n");
        return;
    }

    Node* current = s->head;

    while(current != NULL) {
        printf("%d\n", current->data);
        current = current->next;
    }
}

Node* makeNode(int value) {
    Node* node = malloc(sizeof(Node));
    node->data = value;
    node->next = NULL;
    return node;
}
