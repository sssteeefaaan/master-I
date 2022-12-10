from sys import argv
from time import time

from utils import save, generateRandomGraph, bfs_find_shortest

default = {
    "node_numb": 1000,
    "min_w": 1,
    "max_w": 10,
    "filePath": "./sequential.json"
}

def main(kwargs):
    graph = generateRandomGraph(
        node_numb = kwargs["node_numb"],
        min_w = kwargs["min_w"],
        max_w = kwargs["max_w"])
    start = time()
    paths = dict()
    for node in graph.keys():
        paths |= { node: bfs_find_shortest(graph, node) }
    end = time()
    print("Elapsed time:", end - start)
    save(kwargs["filePath"], paths)

if __name__ == "__main__":
    kwargs = {
        t.split("=")[0][1:]:t.split("=")[1] for t in argv[1:]
    }
    for param in default.keys():
        if not kwargs.get(param, None):
            kwargs[param] = default[param]
    main(kwargs)
