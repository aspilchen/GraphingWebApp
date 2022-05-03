#include <stdlib.h>
#include <stdio.h>

#include "algos.h"
#include "edge_ops.h"

// Calcuates the index for the edge from i to j in a single array adjacency matrix
int calcIndex(int i, int j, int size) {
    return (i * size) + j;
}

struct Node findNewVertexFromHeap(struct BinaryTree* tree, int* visited) {
    struct Node node = tree->nodes[0];
    minHeapPop(tree);

    while(visited[node.vB] != 0 && tree->n > 0) {
        node = tree->nodes[0];
        minHeapPop(tree);
    }

    return node;
}

void minHeapCollect(struct BinaryTree* tree, const int* graph, const int from, const int nVert) {
    int index;
    struct Node node;
    for(int to = 0; to < nVert; to++) {
        index = calcIndex(from, to, nVert);
        if(graph[index] != 0) {
            setNode(&node, from, to, graph[index]);
            minHeapPush(tree, node);
        }
    }
}

void prim(const int* in, int* out, const int nVert) {
    // init
    struct BinaryTree tree;
    initTree(&tree, nVert*nVert);
    struct Node node;
    int* visited = (int*) calloc(nVert, sizeof(int)); // array of bools to tell if vertex i was visited
    int current = 0;
    int next = 0;
    int index = 0;
    int nVisited = 1;
    visited[current] = 1;

    // maint
    while(nVisited < nVert) {
        minHeapCollect(&tree, in, current, nVert);
        node = findNewVertexFromHeap(&tree, visited);

        out[calcIndex(node.vA, node.vB, nVert)] = node.weight;
        out[calcIndex(node.vB, node.vA, nVert)] = node.weight;

        current = node.vB;
        visited[current] = 1;
        nVisited++;
    }
    // end
    destroyTree(&tree);
    free(visited);
}

void bfs(const int* in, int* out, const int start, const int nVert) {
    int* visited = (int*) calloc(nVert, sizeof(int)); // array of bools to tell if vertex i was visited
    int current = start;
    struct Node node;
    struct Queue q;
    int indexA = 0;
    int indexB = 0;
    int nNew = 0;
    int weight = 0;

    initQueue(&q, nVert);

    visited[current] = 1;
    for(int i = 0; i < nVert; i++) {
        weight = in[calcIndex(current, i, nVert)];
        if(weight > 0) {
            setNode(&node, current, i, weight);
            enqueue(&q, node);
        }
    }

    while(q.size > 0) {
        // find next unvisited vertex
        while(q.size > 0 && visited[peekFront(&q).vB] != 0) {
            dequeue(&q);
        }
        // break if nothing remains
        if(q.size == 0) {
            break;
        }

        // move to new vertex
        node = peekFront(&q);
        current = node.vB;
        dequeue(&q);
        visited[current] = 1;

        // do both ways to get undirected or directed
        indexA = calcIndex(node.vA, node.vB, nVert);
        indexB = calcIndex(node.vB, node.vA, nVert);
        out[indexA] = in[indexA];
        out[indexB] = in[indexB];

        // collect new edges
        for(int i = 0; i < nVert; i++) {
            weight = in[calcIndex(current, i, nVert)];
            if(weight > 0) {
                setNode(&node, current, i, weight);
                enqueue(&q, node);
            }
        }
    }

    destroyQueue(&q);
    free(visited);
}

void dfs(const int* in, int* out, const int start, const int nVert) {
    int* visited = (int*) calloc(nVert, sizeof(int)); // array of bools to tell if vertex i was visited
    int current = start;
    struct Node node;
    struct Stack s;
    int indexA = 0;
    int indexB = 0;
    int nNew = 0;
    int weight = 0;

    initStack(&s, nVert);

    visited[current] = 1;
    for(int i = 0; i < nVert; i++) {
        weight = in[calcIndex(current, i, nVert)];
        if(weight > 0) {
            setNode(&node, current, i, weight);
            push(&s, node);
        }
    }

    while(s.size > 0) {
        // find next unvisited vertex
        while(s.size > 0 && visited[peekStack(&s).vB] != 0) {
            pop(&s);
        }
        // break if nothing remains
        if(s.size == 0) {
            break;
        }

        // move to new vertex
        node = peekStack(&s);
        current = node.vB;
        pop(&s);
        visited[current] = 1;

        // do both ways to get undirected or directed
        indexA = calcIndex(node.vA, node.vB, nVert);
        indexB = calcIndex(node.vB, node.vA, nVert);
        out[indexA] = in[indexA];
        out[indexB] = in[indexB];

        // collect new edges
        for(int i = 0; i < nVert; i++) {
            weight = in[calcIndex(current, i, nVert)];
            if(weight > 0) {
                setNode(&node, current, i, weight);
                push(&s, node);
            }
        }
    }

    destroyStack(&s);
    free(visited);
}
