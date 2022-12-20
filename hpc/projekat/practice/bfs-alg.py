from mpi4py import MPI
from utils import generate_graph_random_degree_no_weight_matrix
from time import time
from math import inf

def bfs_sequnetial(graph : dict[str|int, list[str|int]], s : str|int) -> dict[str|int, int]:
    d = {}
    for v in graph.keys():
        d[v] = -1
    d[s] = 0
    level = 1
    ns = []
    fs = [s]
    while fs:
        for u in fs:
            for v in graph[u]:
                if d[v] == -1:
                    ns += [v]
                    d[v] = level
        fs = ns
        ns = []
        level += 1
    return d

def bfs_sequnetial_matrix(graph : list[list[int]], s : int) -> list[int]:
    d = [inf] * len(graph)
    d[s] = 0
    level = 1
    ns = []
    fs = [s]
    while fs:
        for u in fs:
            for v in graph[u]:
                if d[v] == inf:
                    ns += [v]
                    d[v] = level
        fs = ns
        ns = []
        level += 1
    return d

def infinity():
    index = 0
    while True:
        yield index
        index += 1

def dict_reduce(dict1 : dict, dict2: dict, datatype):
    for (k, v) in dict2.items():
        if v != -1:
            dict1[k] = v
    return dict1

def list_reduce(list1 : list, list2 : list, datatype):
    for (ind, x) in enumerate(list2):
        if list1[ind] == -1:
            list1[ind] = x
    return list1

def one_dimentional_distributed_memory_BFS(graph : dict[str|int, list[str|int]], s : str|int) -> dict[str|int, int]:
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()

    graph = comm.bcast(graph, root = 0)

    keys = list(graph.keys())
    L_v_s = { v: -1 if v != s else 0 for v in keys }
    work_load = len(keys) // size + 1
    owned = [set(keys[i * work_load : min(len(keys), (i + 1) * work_load)]) for i in range(size)]

    for level in infinity():
        F = [v for v in owned[rank] if L_v_s[v] == level]
        if not comm.allreduce(len(F) > 0, op=MPI.BOR):
            break
        N = [neighbour for vertex in F for neighbour in graph[vertex]]
        myNq = set()
        for i in range(size):
            Nq = set([v for v in N if v in owned[i]])
            myNq |= comm.sendrecv(sendobj=Nq, dest=i, source=i, sendtag=level, recvtag=level)
        for v in myNq:
            if L_v_s[v] == -1:
                L_v_s[v] = level + 1

    L_v_s = comm.reduce(L_v_s, MPI.Op.Create(dict_reduce, commute=True), root=0)

    return L_v_s

def list_min(a, b, datatype):
    return [min(x, y) for (x, y) in zip(a, b)]

def list_set_join(a, b, datatype):
    return [x|y for (x, y) in zip(a, b)]

def one_dimentional_distributed_memory_BFS_matrix(graph : list[list[int]], s : int) -> list[int]:
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()

    graph = comm.bcast(graph, root = 0)
    L_v_s = [inf if v != s else 0 for v in range(len(graph))]
    work_load = len(graph) // size + 1
    owned = [set(range(i * work_load, min(len(graph), (i + 1) * work_load))) for i in range(size)]

    for level in infinity():
        F = [v for v in owned[rank] if L_v_s[v] == level]
        if not comm.allreduce(len(F) > 0, op=MPI.BOR):
            break
        N = [neighbour for vertex in F for neighbour in graph[vertex]]
        Nq = list()
        for i in range(size):
            Nq += [set([v for v in N if v in owned[i]])]
        globalNq = comm.allreduce(Nq, op=MPI.Op.Create(list_set_join, commute=True))
        for v in globalNq[rank]:
            if L_v_s[v] == inf:
                L_v_s[v] = level + 1
    L_v_s = comm.reduce(L_v_s, op=MPI.Op.Create(list_min, commute=True), root=0)

    return L_v_s

def main():
    start_node = 0
    node_numb = 10000
    degree = 100
    graph = list()

    rank = MPI.COMM_WORLD.Get_rank()
    if rank == 0:
        graph = generate_graph_random_degree_no_weight_matrix(node_numb, degree)
        dt = time()
    pd = one_dimentional_distributed_memory_BFS_matrix(graph, start_node)
    if rank == 0:
        dt = time() - dt
        print("[Distributed]: Time taken", dt)
        dt = time()
        sd = bfs_sequnetial_matrix(graph, start_node)
        dt = time() - dt
        print("[Sequential]: Time taken", dt)
        print(pd == sd)

if __name__ == "__main__":
    main()
