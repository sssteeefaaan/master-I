#include <stdio.h>
#include <string>
#include <map>
#include <tuple>
#include <deque>
#include <omp.h>
#include <ctime>
using namespace std;

map<string, tuple<deque<string>, uint>> bfs_find_shortest(map<string, deque<tuple<string, uint>>>& Graph, string start)
{
    map<string, tuple<deque<string>, uint>> paths;

    for(auto item : Graph)
    {
        if(item.first != start)
        {
            auto path = deque<string>(Graph.size());
            path.push_back(start);
            auto info = make_tuple(path, 0);
            paths.insert_or_assign(item.first, info);
        }
    }

    deque<tuple<deque<string>, uint>> queue;
    while(!queue.empty())
    {
        auto next = queue.front();
        queue.pop_front();
        auto path = get<0>(next);
        auto start_distance = get<1>(next);
        auto tail = path.back();
        for(auto item : Graph[tail])
        {
            auto node = get<0>(item);
            auto distance = get<1>(item);
            auto curr_distance = start_distance + distance;
            if(get<1>(paths[node]) > curr_distance)
            {
                path.push_back(node);
                auto newEntry = make_tuple(path, curr_distance);
                paths[node] = newEntry;
                queue.push_back(newEntry);
            }
        }
    }

    return paths;
}

map<string, deque<tuple<string, uint>>> generate_random_graph(uint node_numb, uint min_w, uint max_w)
{
    map<string, deque<tuple<string, uint>>> graph;
    deque<string> nodes(node_numb);
    for(uint i = 0; i < node_numb; i++)
    {
        string node = "";
        for(uint j = 0; j < i / 26; j++)
            node += (char)((int)'A' + j % 26);
        nodes.push_back(node + (char)((int)'A' + i % 26));
    }

    for(uint i = 0; i < node_numb; i++)
    {
        graph[nodes[i]] = deque<tuple<string, uint>>(node_numb);
        for(auto n : nodes)
            graph[nodes[i]].push_back(make_tuple(n, min(max(i, min_w), min_w)));
    }

    return graph;
}

void save(string file, map<string, map<string, tuple<deque<string>, uint>>>& paths)
{
    FILE* f = fopen(file.c_str(), "wb");
    fwrite(&paths, sizeof(paths), 1, f);
    fclose(f);
}

void parallel_traversal(map<string, deque<tuple<string, uint>>>& graph)
{
    auto paths = map<string, map<string, tuple<deque<string>, uint>>>();
    auto keys = deque<string>(graph.size());
    for(std::map<string, deque<tuple<string, uint>>>::iterator iter = graph.begin(); iter != graph.end(); iter++)
        keys.push_back(iter->first);

    auto dt = clock();
    int proc = omp_get_num_procs();
    #pragma omp parallel num_threads(proc)
    {
        auto my_paths = map<string, map<string, tuple<deque<string>, uint>>>();
        #pragma omp for schedule(dynamic)
        for(int i = 0; i < keys.size(); i++)
        {
            auto res = bfs_find_shortest(graph, keys.at(i));
            my_paths.insert_or_assign(keys.at(i), res);
        }
        #pragma omp critical (join)
            paths.merge(my_paths);
    }

    dt = clock() - dt;
    printf("[Parallel]: Time taken %f\n", double(dt));

    save("./parallel", paths);
}

void serial_traversal(map<string, deque<tuple<string, uint>>>& graph)
{
    auto paths = map<string, map<string, tuple<deque<string>, uint>>>();

    auto dt = clock();
    for(auto item : graph)
        paths.insert_or_assign(item.first, bfs_find_shortest(graph, item.first));

    dt = clock() - dt;
    printf("[Serial]: Time taken %f\n", double(dt));

    save("./serial", paths);
}

int main(int argc, char** argv)
{
    int node_numb = 1000, min_w = 1, max_w = 10;
    switch(argc)
    {
        case(4): node_numb=atoi(argv[3]);
        case(3): node_numb=atoi(argv[2]);
        case(2): node_numb=atoi(argv[1]);
    }

    auto graph = generate_random_graph(node_numb, min_w, max_w);

    serial_traversal(graph);
    parallel_traversal(graph);

    return 0;
}