#include "../edge_ops.h"
#include <assert.h>
#include <stdio.h>

void dumpTree(struct BinaryTree* tree) {
    printf("Tree dump\n");
    for(int i = 0; i < tree->n; i++) {
        printf("%d ",tree->nodes[i].weight);
    }
    printf("\n");
}

void testIndexing() {
    assert(getParentIndex(0) == 0);
    assert(getParentIndex(1) == 0);
    assert(getParentIndex(2) == 0);
    assert(getParentIndex(3) == 1);
    assert(getParentIndex(4) == 1);
    assert(getParentIndex(5) == 2);
    assert(getParentIndex(6) == 2);

    assert(getLeftChildIndex(0) == 1);
    assert(getRightChildIndex(0) == 2);
    assert(getLeftChildIndex(1) == 3);
    assert(getRightChildIndex(1) == 4);
    assert(getLeftChildIndex(2) == 5);
    assert(getRightChildIndex(2) == 6);
}

void testIsNode() {
    int nTests = 10;
    struct BinaryTree tree;
    initTree(&tree, nTests);
    struct Node node;

    setNode(&node, 0, 0, 1);
    minHeapPush(&tree, node);
    assert(isNode(&tree, 0));

    minHeapPush(&tree, node);
    assert(isNode(&tree, 1));
    minHeapPush(&tree, node);
    assert(!isNode(&tree, 0));
    assert(isNode(&tree, 2));
    assert(isNode(&tree, 1));
    destroyTree(&tree);
}

void testMinHeapPush1() {
    int nTests = 10;
    struct BinaryTree tree;
    initTree(&tree, nTests);
    struct Node node;

    for(int i = 0; i < nTests; i++) {
        assert(tree.n == i);
        setNode(&node, 0, 0, i);
        minHeapPush(&tree, node);
    }

    //dumpTree(&tree);

    for(int i = 0; i < nTests; i++) {
        assert(tree.nodes[i].weight == i);
    }
    destroyTree(&tree);
}

void testMinHeapPush2() {
    int nTests = 6;
    struct BinaryTree tree;
    initTree(&tree, 10);
    struct Node node;

    for(int i = nTests; i > 0; i--) {
        setNode(&node, 0, 0, i);
        minHeapPush(&tree, node);
        //dumpTree(&tree);
    }
    setNode(&node, 0, 0, 20);
    minHeapPush(&tree, node);
    //dumpTree(&tree);

    assert(tree.nodes[0].weight == 1);
    assert(tree.nodes[1].weight == 3);
    assert(tree.nodes[2].weight == 2);
    assert(tree.nodes[3].weight == 6);
    assert(tree.nodes[4].weight == 4);
    assert(tree.nodes[5].weight == 5);
    assert(tree.nodes[6].weight == 20);
    destroyTree(&tree);
}

void testMinHeapPop1() {
    int nTests = 10;
    struct BinaryTree tree;
    initTree(&tree, nTests);
    struct Node node;

    for(int i = 0; i < nTests; i++) {
        setNode(&node, 0, 0, i);
        minHeapPush(&tree, node);
    }

    for(int i = 0; i < nTests; i++) {
        //dumpTree(&tree);
        assert(tree.nodes[0].weight == i);
        assert(tree.n == nTests-i);
        minHeapPop(&tree);
    }
    assert(tree.n == 0);
    destroyTree(&tree);
}

void testQueueInit() {
    struct Queue q;
    const int n = 10;
    initQueue(&q, n);
    assert(q.size == 0);
    assert(q.capacity == n);
    assert(q.front == 0);
    assert(q.back == 0);
}

void testQueueInOut() {
    struct Queue q;
    struct Node node;
    const int n = 5;
    initQueue(&q, n);

    for(int i = 0; i < n; i++) {
        setNode(&node, i, i+1, i+1);
        enqueue(&q, node);
    }

    for(int i = 0; i < n; i++) {
        assert(q.nodes[i].vA == i);
        assert(q.nodes[i].vB == i+1);
        assert(q.nodes[i].weight == i+1);
    }
}

int main() {
    testIndexing();
    testIsNode();
    testMinHeapPush1();
    testMinHeapPush2();
    testMinHeapPop1();
    testQueueInit();
    testQueueInOut();
}