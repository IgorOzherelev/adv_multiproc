#ifndef ADV_MULTIPROC_BENCHMARK_HPP
#define ADV_MULTIPROC_BENCHMARK_HPP

#include <functional>
#include "iostream"
#include "vector"
#include "chrono"

const int NUMBER_OF_ELEMENTS = 100000000;

std::vector<int> generate_random_vector(int num) {
    std::vector<int> data = std::vector<int>(num, 0);

    for (int i = 0; i < num; i++) {
        data[i] = std::rand();
    }

    return data;
}

bool check_sorted_vector(std::vector<int>& vector) {
    bool is_sorted = true;
    for (int i = 0; i < vector.size() - 1; i++) {
        if (vector[i] > vector[i + 1]) {
            is_sorted = false;
            break;
        }
    }

    return is_sorted;
}

long long do_benchmark(const std::function<void(std::vector<int>&, int, int)>& quicksort) {
    std::vector<int> vector = generate_random_vector(NUMBER_OF_ELEMENTS);

    auto start = std::chrono::high_resolution_clock::now();
    quicksort(vector, 0, vector.size());
    auto end = std::chrono::high_resolution_clock::now();
    if (!check_sorted_vector(vector)) {
        throw std::runtime_error("Quicksort, shame on you!!");
    }
    std::destroy(vector.begin(), vector.end());

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

void benchmark(
        const std::string& name,
        std::size_t repeat,
        const std::function<void(std::vector<int>&, int, int)>& quicksort
) {
    long long time = 0;

    std::cout << "started " << name << std::endl;
    for (int i = 0; i < repeat; i++) {
        long long repeat_time = do_benchmark(quicksort);
        time += repeat_time;
        std::cout << "REPEAT[" << i << "]" << "TIME[" << repeat_time << " in ms]" << std::endl;
    }
    std::cout << "AVERAGE[" << time / repeat << " in ms]" << std::endl;
}

#endif //ADV_MULTIPROC_BENCHMARK_HPP
