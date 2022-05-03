import algorithms
import json

graph = json.loads('{"vertices":[{"label":0},{"label":1},{"label":2}],"dEdges":[],"uEdges":[{"vA":0,"vB":1,"weight":"1"},{"vA":1,"vB":2,"weight":"1"}]}')
algorithm = "prim"

g1 = algorithms.process(graph, algorithm)
print(g1)