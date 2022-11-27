#include <iostream>
#include <vector>

#include "src/quicksort.hpp"

void benchmark(std::string &name) {

}

void print(const std::vector<int>& a) {
    for (int elem : a) {
        std::cout << elem << " ";
    }
}

int main() {
    std::vector<int> a = {1, 5, 3, 4, 9};
    quicksort::sequential(a, 0, a.size());

    print(a);

    return 0;
}