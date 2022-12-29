#ifndef BST_BENCHMARK_HPP
#define BST_BENCHMARK_HPP

#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

#include "iostream"
#include "vector"
#include "chrono"
#include "random"
#include "atomic"

#include "BST.hpp"
#include "CBST.hpp"
#include "SeqBST.hpp"

const int THREADS_NUM = 4;
const long long DEADLINE = 5000;
std::vector<int> xs = {0, 10, 50};
std::atomic<int> ops{0};

bool check_sorted_vector(std::vector<uint32_t> vector) {
    bool is_sorted = true;
    for (int i = 0; i < vector.size() - 1; i++) {
        if (vector[i] > vector[i + 1]) {
            is_sorted = false;
            break;
        }
    }

    return is_sorted;
}

void check_bst(BST<uint32_t>* bst) {
    if (!check_sorted_vector(bst->inorder_traversal())) {
        throw std::runtime_error("bst check failed");
    }
}

uint32_t generate_key() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 100000);

    return dist(rng);
}

uint32_t generate_p() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 101);

    return dist(rng);
}

void benchmark(int x, BST<uint32_t>* bst) {
    auto start = std::chrono::high_resolution_clock::now();
    auto current_time = std::chrono::high_resolution_clock::now();

    while (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start).count() <= DEADLINE) {
        current_time = std::chrono::high_resolution_clock::now();
        uint32_t key = generate_key();
        uint32_t p = generate_p();

        if (p < x) {
            bst->insert(key);
        } else if (p >= x && p < 2 * x) {
            bst->remove(key);
        } else if (p >= 2 && p <= 100) {
            bst->contains(key);
        }

        ops++;
    }
}

void sequential_benchmark() {
    auto* bst = new SeqBST();

    for (int x : xs) {
        benchmark(x, bst);
        std::cout << "X: " << x << " OPS: " << ops.load() << std::endl;
        ops = 0;
    }

    check_bst(bst);
}

void concurrent_benchmark() {
    auto* bst = new CBST();
    int x = 0;
    boost::asio::thread_pool pool(THREADS_NUM);
    boost::asio::post(pool, [x, bst] () {
        benchmark(x, bst);
    });

    pool.join();
    std::cout << "X: " << x << " OPS: " << ops.load() << std::endl;
    ops = 0;

    check_bst(bst);
}

void seq_correctness_test() {
    auto* seq_bst = new SeqBST();
    std::cout << seq_bst->insert(1);
    std::cout << seq_bst->insert(2);
    std::cout << seq_bst->insert(3);
    std::cout << seq_bst->contains(3);
    std::cout << seq_bst->remove(1);
    std::cout << seq_bst->contains(1);

    check_bst(seq_bst);
}

void concurrent_correctness_test() {
    auto* cbst = new CBST();
    std::cout << cbst->insert(1);
    std::cout << cbst->insert(2);
    std::cout << cbst->insert(3);
    std::cout << cbst->contains(3);
    std::cout << cbst->remove(1);
    std::cout << cbst->contains(1);

    check_bst(cbst);
}

#endif //BST_BENCHMARK_HPP
