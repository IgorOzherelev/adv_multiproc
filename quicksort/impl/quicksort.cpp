#include "../src/quicksort.hpp"

int quicksort::partition(std::vector<int> &a, int l, int r) {
    auto pivot = a[r - 1];
    int i = l;
    for (int j = l; j < r - 1; j++) {
        if (a[j] <= pivot) {
            std::swap(a[j], a[i]);
            i++;
        }
    }

    std::swap(a[i], a[r - 1]);
    return i;
}

int quicksort::rand_partition(std::vector<int> &a, int l, int r) {
    auto pivot_idx = std::rand() % (r - l) + l;
    std::swap(a[pivot_idx], a[r - 1]);

    return partition(a, l, r);
}

void quicksort::sequential(std::vector<int> &a, int l, int r) {
    if (r - l > 1) {
        int i = rand_partition(a, l, r);
        sequential(a, l, i);
        sequential(a, i + 1, r);
    }
}

void quicksort::parallel(std::vector<int> &a, int l, int r) {
    if (r - l < BLOCK) {
        sequential(a, l, r);
        return;
    }

    int i = rand_partition(a, l, r);
    parlay::par_do(
            [&]() { parallel(a, l, i); },
            [&]() { parallel(a, i + 1, r); }
    );
}
