from sys import argv
from time import time

from mpi4py import MPI

from utils import save, generateRandomGraph, bfs_find_shortest

default = {
    "node_numb": 1000,
    "min_w": 1,
    "max_w": 10,
    "filePath": "./parallel.json"
}

def main(kwargs):
    com = MPI.COMM_WORLD
    rank = com.Get_rank()
    size = com.Get_size()
    graph = dict()
    keys = list()
    if rank == 0:
        graph = generateRandomGraph(
                node_numb = kwargs["node_numb"],
                min_w = kwargs["min_w"],
                max_w = kwargs["max_w"])
        keys = list(graph.keys())
        start = time()

    graph = com.bcast(graph, root=0)
    my_work = com.scatter([keys[i * len(graph) // size : (i + 1) * len(graph) // size] for i in range(size)], root = 0)

    my_paths = dict()
    for node in my_work:
        my_paths |= { node: bfs_find_shortest(graph, node) }
    paths = com.gather(my_paths, root = 0)

    if rank == 0:
        total = dict()
        for i in paths:
            total |= i
        end = time()
        print("Elapsed time:", end - start)
        save(kwargs["filePath"], total)

if __name__ == "__main__":
    kwargs = {
        t.split("=")[0][1:]:t.split("=")[1] for t in argv[1:]
    }
    for param in default.keys():
        if not kwargs.get(param, None):
            kwargs[param] = default[param]
    main(kwargs)