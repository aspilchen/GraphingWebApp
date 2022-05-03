import algos

def makeEdgeDict(vA, vB, weight):
    return {
        'vA': vA,
        'vB': vB,
        'weight': weight
    }


def calcIndex(i, offset, size):
    return (i * size) + offset

# Done in one big function because swig gets angry passing c arrays around
# takes a json representation of the graph, converts it to a swig C array,
# runs the c algorithm, converts the c array back to a dictionary
def process(graph, algorithm):
    nVert = len(graph['vertices'])
    size = nVert * nVert # size of 1d adjacency matrix
    cInGraph = algos.new_intArr(size) # swig array passed as input to C function
    cOutGraph = algos.new_intArr(size) # swig array passed as output to C function
    index = 0

    # dictionary graph to be returned and converted to json
    outGraph = {
        'vertices': [],
        'dEdges': [],
        'uEdges': []
    }

    for i in graph['uEdges']:
        index = calcIndex(int(i['vA']), int(i['vB']), nVert)
        algos.intArr_setitem(cInGraph, index, int(i['weight']))
        index = calcIndex(int(i['vB']), int(i['vA']), nVert)
        algos.intArr_setitem(cInGraph, index, int(i['weight']))

    for i in graph['dEdges']:
        index = calcIndex(int(i['vA']), int(i['vB']), nVert)
        algos.intArr_setitem(cInGraph, index, int(i['weight']))

    # run desired c function algorithm
    # this is super ugly, but I couldnt think of a better alternative since
    # the number of args can be different
    if algorithm == 'prim':
        algos.prim(cInGraph, cOutGraph, nVert)
    elif algorithm == 'bfs':
        algos.bfs(cInGraph, cOutGraph, 0, nVert)
    elif algorithm == 'dfs':
        algos.dfs(cInGraph, cOutGraph, 0, nVert)

    # add vertices to output dictionary
    for i in range(nVert):
        outGraph['vertices'].append(i)

     # add edges to output dictionary
    for i in range(nVert):
        for j in range(nVert):
            eIJ = calcIndex(i, j, nVert) # check from vertex i to vertex j
            eJI = calcIndex(j, i, nVert) # check from vertex i to vertex j 
            wA = algos.intArr_getitem(cOutGraph, eIJ)
            wB = algos.intArr_getitem(cOutGraph, eJI)
            if wA != 0 and wB !=0 and i < j: # if edge exists from i to j and from j to i, i<j to avoid duplicates
                outGraph['uEdges'].append(makeEdgeDict(i, j, wA))
            elif wA != 0 and wB == 0: # if edge exists from i to j only
                outGraph['dEdges'].append(makeEdgeDict(i, j, wA))

    algos.delete_intArr(cInGraph)
    algos.delete_intArr(cOutGraph)
    return outGraph