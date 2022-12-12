#ifndef BFS_BFS_HPP
#define BFS_BFS_HPP

#include <atomic>
#include <parlay/primitives.h>
#include <parlay/sequence.h>

#include "vector"
#include "queue"
#include "graph.hpp"

namespace bfs {
    std::vector<int> sequential(int s, Graph &graph) {
        std::queue<int> q;
        q.push(s);
        std::vector<bool> used(graph.total);
        std::vector<int> d(graph.total);
        used[s] = true;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            std::vector<int> neighbors = graph.neighbors(v);
            for (int to : neighbors) {
                if (!used[to]) {
                    used[to] = true;
                    q.push(to);
                    d[to] = d[v] + 1;
                }
            }
        }

        return d;
    }

    std::vector<int> parallel(int s, Graph &graph) {
        std::vector<int> d(graph.total);
        using nested_seq = parlay::sequence<parlay::sequence<int>>;
        auto visited = parlay::tabulate<std::atomic<bool>>(graph.total, [&](long i) {
            return (i == s);
        });

        parlay::sequence<int> frontier(1, s);
        nested_seq frontiers;
        int depth = 0;
        while (!frontier.empty()) {
            frontiers.push_back(frontier);
            parlay::for_each(frontier,[&](int node) { d[node] = depth; });

            auto out = parlay::flatten(map(frontier, [&](int u) { return graph.neighbors(u); }));

            frontier = parlay::filter(out, [&](auto &&v) {
                bool expected = false;
                return (!visited[v]) && visited[v].compare_exchange_strong(expected, true);
            });

            depth++;
        }

        return d;
    }
}

#endif //BFS_BFS_HPP
