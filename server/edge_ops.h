#include <stdlib.h>
#include <stdio.h>

struct Node {
    int vA;
    int vB;
    int weight;
};

void setNode(struct Node* node, int vA, int vB, int weight) {
    node->vA = vA;
    node->vB = vB;
    node->weight = weight;
}

struct Stack{
    struct Node* nodes;
    int size;
    int capacity;
};

void initStack(struct Stack* s, const int size) {
    s->nodes = (struct Node*) calloc(size, sizeof(struct Node));
    s->size = 0;
    s->capacity = size;
}

void destroyStack(struct Stack* s) {
    free(s->nodes);
    s->size = 0;
    s->capacity = 0;
}

void push(struct Stack* s, struct Node n) {
    if(s->size < s->capacity) {
        s->nodes[s->size] = n;
        s->size++;
    }
}

void pop(struct Stack* s) {
    if(s->size > 0) {
        s->size--;
    }
}

struct Node peekStack(struct Stack* s) {
    return s->nodes[s->size-1];
}

struct Queue {
    struct Node* nodes;
    int size;
    int front;
    int back;
    int capacity;
};


void initQueue(struct Queue* q, const int size) {
    q->nodes = (struct Node*) calloc(size, sizeof(struct Node));
    q->size = 0;
    q->front = 0;
    q->back = 0;
    q->capacity = size;
}

void destroyQueue(struct Queue* q) {
    free(q->nodes);
    q->size = 0;
    q->capacity = 0;
}

void enqueue(struct Queue* q, struct Node n) {
    if(q->size < q->capacity) {
        q->nodes[q->back] = n;
        q->back = (q->back + 1) % q->capacity;
        q->size++;
    }
}

void dequeue(struct Queue* q) {
    if(q->size > 0) {
        q->front = (q->front + 1) % q->capacity;
        q->size--;
    }
}

struct Node peekFront(struct Queue* q) {
    return q->nodes[q->front];
}

struct Node peekBack(struct Queue* q) {
    return q->nodes[q->back -1];
}

struct BinaryTree {
    struct Node* nodes;
    int n;
};

void initTree(struct BinaryTree* tree, int n) {
    tree->nodes = (struct Node*) calloc(n, sizeof(struct Node));
    tree->n = 0;
}

void destroyTree(struct BinaryTree* tree) {
    free(tree->nodes);
}

int getLeftChildIndex(int i) {
    return (2*i)+1;
}

int getRightChildIndex(int i) {
    return (2*i)+2;
}

int isNode(struct BinaryTree* tree, int i) {
    return i >= (tree->n / 2) && i < tree->n;
}

int getParentIndex(int i) {
    int p = 0;
    if(i == 0) {
        p = 0;
    }
    else if(i%2 == 0) {
        p = (i/2) - 1;
    }
    else {
        p = (i-1) / 2;
    }
    return p;
}

void minHeapPush(struct BinaryTree* tree, struct Node v) {
    int i = getParentIndex(tree->n);
    int j = tree->n;

    while(v.weight < tree->nodes[i].weight) {
        tree->nodes[j] = tree->nodes[i];
        j = i;
        if(i == 0) break;
        i = getParentIndex(i);
    }

    tree->nodes[j] = v;
    tree->n++;
}

void minHeapPop(struct BinaryTree* tree) {
    int left = getLeftChildIndex(0);
    int right = getRightChildIndex(0);
    int last = tree->n-1;
    int current = 0;

    while(!isNode(tree, current)) {
        if(tree->nodes[last].weight > tree->nodes[left].weight ||
          tree->nodes[last].weight > tree->nodes[right].weight) {
            
            if(tree->n <= right || tree->nodes[left].weight <= tree->nodes[right].weight) {
                tree->nodes[current] = tree->nodes[left];
                current = left;
            }
            else if(tree->nodes[right].weight < tree->nodes[left].weight) {
                tree->nodes[current] = tree->nodes[right];
                current = right;
            }
            left = getLeftChildIndex(current);
            right = getRightChildIndex(current);
        }
        else {
            break;
        }
    }
    tree->nodes[current] = tree->nodes[last];
    tree->n--;
}