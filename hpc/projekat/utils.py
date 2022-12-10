def readFile(filePath):
    f = open(filePath, "r")
    ret = f.read()
    f.close()
    return ret

def check(file1, file2):
    print(readFile(file1) == readFile(file2))

def save(filePath, paths):
    from json import dumps
    f = open(filePath, "w")
    f.write(dumps(paths))
    f.close()


def bfs_find_shortest(graph: dict, start):
    from math import inf
    queue = [([start], 0)]
    paths = { n: ([start], inf if n != start else 0) for n in graph.keys() }
    while queue:
        (path, start_distance), *queue = queue
        tail = path[-1]
        for (node, distance) in graph[tail]:
            curr_distance = start_distance + distance
            if paths[node][1] > curr_distance:
                paths[node] = (path + [node], curr_distance)
                queue = queue + [(path + [node], curr_distance)]
    return paths

def dfs_find_shortest(graph: dict, start):
    from math import inf
    stack = [([start], 0)]
    paths = { n: ([start], inf if n != start else 0) for n in graph.keys() }
    while stack:
        *stack, (path, start_distance) = stack
        tail = path[-1]
        for (node, distance) in graph[tail]:
            curr_distance = start_distance + distance
            if paths[node][1] > curr_distance:
                paths[node] = (path + [node], curr_distance)
                stack = stack + [(path + [node], curr_distance)]
    return paths

def generateRandomGraph(node_numb, min_w, max_w) -> dict:
    from random import randrange
    nodes = list()
    for i in range(node_numb):
        node = ""
        for j in range(i // 26):
            node += chr(ord('A') + j % 26)
        nodes.append(node + chr(ord('A') + i % 26))
    graph = {}
    for ind, node in enumerate(nodes):
        graph[node] = []
        for neighbour in nodes:
            graph[node] += [(neighbour, min(max(ind, min_w),max_w))]
    return graph