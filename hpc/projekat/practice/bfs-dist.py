from sys import argv
from time import time
from math import inf

from mpi4py import MPI

from utils import save, generate_random_fully_connected_graph_with_weght, bfs_find_shortest

default = {
    "node_numb": 100,
    "min_w": 1,
    "max_w": 10
}

def dictReduce(dict1, dict2, datatype):
    for (k, v) in dict2.items():
        dict1[k] = v
    return dict1

def sequential(graph):
    start = time()
    paths = dict()
    for node in graph.keys():
        paths |= { node: bfs_find_shortest(graph, node) }
    end = time()
    print("[Sequential]: Elapsed time", end - start)

def distributed(graph):
    com = MPI.COMM_WORLD
    rank = com.Get_rank()
    size = com.Get_size()
    keys = list()
    if rank == 0:
        keys = list(graph.keys())
        start = time()
    else:
        graph = {}

    graph = com.bcast(graph, root=0)
    my_work = com.scatter([keys[i * len(graph) // size : (i + 1) * len(graph) // size] for i in range(size)], root = 0)

    my_paths = dict()
    for node in my_work:
        my_paths |= { node: bfs_find_shortest(graph, node) }
    paths = com.gather(my_paths, root = 0)

    if rank == 0:
        total = {}
        for p in paths:
            total |= p
        paths = total
        end = time()
        print("[Distributed]: Elapsed time", end - start)

    return paths

def main():
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    graph = {}
    if rank == 0:
        kwargs = {
            t.split("=")[0][1:]:t.split("=")[1] for t in argv[1:]
        }
        for param in default.keys():
            if not kwargs.get(param, None):
                kwargs[param] = default[param]
        graph = generate_random_fully_connected_graph_with_weght(**kwargs)
    dpaths = distributed(graph)
    if rank == 0:
        spaths = sequential(graph)
        print(spaths == dpaths)

if __name__ == "__main__":
    main()