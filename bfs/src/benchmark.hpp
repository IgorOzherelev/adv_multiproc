#ifndef BFS_BENCHMARK_HPP
#define BFS_BENCHMARK_HPP

#include <functional>
#include "graph.hpp"
#include "iostream"
#include "chrono"
#include "string"

void check_depths(std::vector<int> depths, int size) {
    for (int i = 0; i < depths.size(); i++) {
        assert(depths[i] == node::x(size, i) + node::y(size, i) + node::z(size, i));
    }
}

long long do_benchmark(int size, const std::function<std::vector<int>(int, Graph&)>& bfs) {
    auto start = std::chrono::high_resolution_clock::now();
    auto g = Graph3D(size);
    check_depths(bfs(0, g), size);
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

void benchmark(const std::string &name, std::size_t repeat, int size, const std::function<std::vector<int>(int, Graph&)>& bfs) {
    std::cout << "started " << name << std::endl;

    long long time = 0;
    for (std::size_t i = 0; i < repeat; i++) {
        long long repeat_time = do_benchmark(size, bfs);
        time += repeat_time;
        std::cout << "REPEAT[" << i << "]" << "TIME[" << repeat_time << " in ms]" << std::endl;
    }
    std::cout << "AVERAGE[" << time / repeat << " in ms]" << std::endl;
}

#endif //BFS_BENCHMARK_HPP
