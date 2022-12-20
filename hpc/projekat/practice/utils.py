def bfs_find_shortest(graph: dict[str|int, list[tuple[str|int, int]]], start : str) -> dict[str|int, tuple[list[str|int], int]]:
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

def dfs_find_shortest(graph: dict[str | int, list[tuple[str|int, int]]], start : str) -> dict[str|int, tuple[list[str|int], int]]:
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

def int_to_letters(value : int)->str:
    ret = ""
    for j in range(value // 26):
        ret += chr(ord('A') + j % 26)
    return ret + chr(ord('A') + value % 26)

def generate_random_fully_connected_graph_with_weght(vertex_numb : int, min_w : int, max_w : int, datatype : type) -> dict[str|int, list[tuple[str|int, int]]]:
    from random import randrange
    if datatype == str:
        return { int_to_letters(i): [(int_to_letters(j), randrange(min_w, max_w)) for j in range(vertex_numb)] for i in range(vertex_numb) }
    elif datatype == int:
        return { i : [(j, randrange(min_w, max_w)) for j in range(vertex_numb)] for i in range(vertex_numb) }
    return {}

def generate_random_fully_connected_graph_without_weght(vertex_numb : int, datatype:type) -> dict[str|int, list[str|int]]:
    if datatype == str:
        return { int_to_letters(i): [int_to_letters(j) for j in range(vertex_numb)] for i in range(vertex_numb) }
    elif datatype == int:
        return { i : [j for j in range(vertex_numb)] for i in range(vertex_numb) }
    return {}

def generate_graph_random_degree_with_weight(vertex_numb : int, min_w : int, max_w : int, degree : int, datatype:type) -> dict[str|int, list[tuple[str|int, int]]]:
    from random import randrange
    if datatype == str:
        return { int_to_letters(i): [(int_to_letters(randrange(vertex_numb)), randrange(min_w, max_w)) for _ in range(degree)] for i in range(vertex_numb) }
    elif datatype == int:
        return { i : [(randrange(vertex_numb), randrange(min_w, max_w)) for _ in range(degree)] for i in range(vertex_numb) }
    return {}

def generate_graph_random_degree_no_weight(vertex_numb : int, degree : int, datatype : type) -> dict[str|int, list[str|int]]:
    from random import randrange
    if datatype == str:
        return { int_to_letters(i): [int_to_letters(randrange(vertex_numb)) for _ in range(degree)] for i in range(vertex_numb) }
    elif datatype == int:
        return { i : [randrange(vertex_numb) for _ in range(degree)] for i in range(vertex_numb) }
    return {}

def generate_graph_random_degree_no_weight_matrix(vertex_numb : int, degree : int) -> list[list[int]]:
    from random import randrange
    return [[randrange(vertex_numb) for _ in range(degree)] for _ in range(vertex_numb)]