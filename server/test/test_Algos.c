#include <stdio.h>
#include <assert.h>
#include "../algos.c"

void testFindNewVert() {
    int nVert = 5;
    struct BinaryTree tree;
    struct Node node;

    initTree(&tree, nVert);
    int visited[] = {1, 0, 0, 0, 0};

    setNode(&node, 0, 1, 1);
    minHeapPush(&tree, node);
    setNode(&node, 0, 2, 1);
    minHeapPush(&tree, node);
    setNode(&node, 1, 3, 1);
    minHeapPush(&tree, node);
    setNode(&node, 2, 3, 1);
    minHeapPush(&tree, node);
    setNode(&node, 2, 4, 3);
    minHeapPush(&tree, node);


    for(int i = 0; i < nVert-1; i++) {
        node = findNewVertexFromHeap(&tree, visited);
        visited[node.vB]++;
    }

    // all nodes are visited once
    for(int i = 0; i < nVert; i++) {
        //printf("%d ", visited[i]);
        assert(visited[i] == 1);
    }
    //printf("\n");
    destroyTree(&tree);
}

void testCollect() {
    int nVert = 5;
    int nEdges = 10;
    struct BinaryTree tree;
    struct Node node;
    initTree(&tree, nVert);
    int checked[] = {0, 0, 0, 0, 0};
    int graph[] = {0, 1, 1, 0, 0, // v0->{v1, v2}
                    1, 0, 0, 1, 0, // v1->{v0, v3}
                    1, 0, 0, 1, 3, // v2->{v0, v3, v4}
                    0, 1, 1, 0, 0, // v3->{v1, v2}
                    0, 0, 3, 0, 0}; // v4->v2

    for(int i = 0; i < nVert; i++) {
        minHeapCollect(&tree, graph, i, nVert);
    }
    assert(tree.n == nEdges);
    while(tree.n > 0) {
        checked[tree.nodes[0].vA]++;
        checked[tree.nodes[0].vB]++;
        minHeapPop(&tree);
    }

//    for(int i = 0; i < nVert; i++) {
//        printf("%d ", checked[i]);
//    }
//    printf("\n");

    assert(checked[0] == 4);
    assert(checked[1] == 4);
    assert(checked[2] == 6);
    assert(checked[3] == 4);
    assert(checked[4] == 2);
    destroyTree(&tree);
}

void testPrim1() {
    const int nVert = 3;
    int outGraph[nVert*nVert];
    int inGraph[] = {0,1,0,  // v0->v1
                     1,0,1,  // v1->{v0.v2}
                     0,1,0}; // v2->v1


    for(int i = 0; i < nVert*nVert; i++) {
        outGraph[i] = 0;
    }

    prim(inGraph, outGraph, nVert);

    assert(outGraph[calcIndex(0, 1, nVert)] == 1);
    assert(outGraph[calcIndex(1, 0, nVert)] == 1);
    assert(outGraph[calcIndex(1, 2, nVert)] == 1);
    assert(outGraph[calcIndex(2, 1, nVert)] == 1);

    assert(outGraph[calcIndex(0, 2, nVert)] == 0);
    assert(outGraph[calcIndex(2, 0, nVert)] == 0);
}

void testPrim2() {
    const int nVert = 5;
    int outGraph[nVert*nVert];
    int inGraph[] = {0, 2, 1, 0, 0, // v0->{v1, v2}
                    2, 0, 0, 4, 0, // v1->{v0, v3}
                    1, 0, 0, 4, 3, // v2->{v0, v3, v4}
                    0, 4, 4, 0, 0, // v3->{v1, v2}
                    0, 0, 3, 0, 0}; // v4->v2


    for(int i = 0; i < nVert*nVert; i++) {
        outGraph[i] = 0;
    }

    prim(inGraph, outGraph, nVert);

    assert(outGraph[calcIndex(0, 1, nVert)] == 2);
    assert(outGraph[calcIndex(1, 0, nVert)] == 2);
    assert(outGraph[calcIndex(0, 2, nVert)] == 1);
    assert(outGraph[calcIndex(2, 0, nVert)] == 1);

    int pA = outGraph[calcIndex(1, 3, nVert)] == 4;
    pA = pA && outGraph[calcIndex(3, 1, nVert)] == 4;
    pA = pA && outGraph[calcIndex(2, 3, nVert)] != 4;
    pA = pA && outGraph[calcIndex(3, 2, nVert)] != 4;

    int pB = outGraph[calcIndex(1, 3, nVert)] != 4;
    pB = pB && outGraph[calcIndex(3, 1, nVert)] != 4;
    pB = pB && outGraph[calcIndex(2, 3, nVert)] == 4;
    pB = pB && outGraph[calcIndex(3, 2, nVert)] == 4;

    assert(pA || pB);
}

void testBFS1() {
    const int nVert = 5;
    int size = nVert*nVert;
    int outGraph[nVert*nVert];
    int inGraph[] = {0, 2, 1, 0, 0, // v0->{v1, v2}
                    2, 0, 0, 4, 0, // v1->{v0, v3}
                    1, 0, 0, 4, 3, // v2->{v0, v3, v4}
                    0, 4, 4, 0, 0, // v3->{v1, v2}
                    0, 0, 3, 0, 0}; // v4->v2


    for(int i = 0; i < nVert*nVert; i++) {
        outGraph[i] = 0;
    }

    bfs(inGraph, outGraph, 0, nVert);

    for(int i = 0; i < nVert; i++) {
        for(int j = 0; j < nVert; j++) {
            printf("%d ", outGraph[calcIndex(i, j, nVert)]);
        }
        printf("\n");
    }
}

void testBFS2() {
    const int nVert = 4;
    int size = nVert*nVert;
    int outGraph[nVert*nVert];
    int inGraph[] = {0, 1, 0, 0, // v0->v1
                    0, 0, 1, 0,  // v1->v2
                    0, 0, 0, 1,  // v2->v3
                    0, 0, 0, 0};  // 


    for(int i = 0; i < nVert*nVert; i++) {
        outGraph[i] = 0;
    }

    bfs(inGraph, outGraph, 0, nVert);

    for(int i = 0; i < nVert; i++) {
        for(int j = 0; j < nVert; j++) {
            printf("%d ", outGraph[calcIndex(i, j, nVert)]);
        }
        printf("\n");
    }
}

int main() {
    testFindNewVert();
    testCollect();
    testPrim1();
    testPrim2();
    //testBFS1();
    testBFS2();
    return 0;
}