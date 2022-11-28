#include <iostream>
#include <vector>

#include "src/quicksort.hpp"
#include "src/benchmark.hpp"

void print(const std::vector<int>& a) {
    for (int elem : a) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

int main() {
//    std::vector<int> a = {1, 5, 3, 4, 9, 22, 11, 2, 99, 0, 6, 7};
//    quicksort::sequential(a, 0, a.size());
//
//    std::cout << check_sorted_vector(a) << std::endl;
//    print(a);

    int repeat = 5;
//    benchmark("sequential quicksort", repeat, quicksort::sequential);
    benchmark("parallel quicksort", repeat, quicksort::parallel);

    return 0;
}