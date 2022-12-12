#include "src/benchmark.hpp"
#include "src/bfs.hpp"

int main() {
    int repeat = 5;
    int size = 500;

    benchmark("sequential", repeat, size, bfs::sequential);
    benchmark("parallel", repeat, size, bfs::parallel);

    return 0;
}