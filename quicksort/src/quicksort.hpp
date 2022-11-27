#ifndef ADV_MULTIPROC_QUICKSORT_HPP
#define ADV_MULTIPROC_QUICKSORT_HPP

#include <iostream>
#include <vector>

#include <parlay/parallel.h>
#include <parlay/primitives.h>
#include <parlay/sequence.h>

const int BLOCK = 100;

namespace quicksort {
    int partition(std::vector<int> &a, int l, int r);

    int rand_partition(std::vector<int> &a, int l, int r);

    void sequential(std::vector<int> &a, int l, int r);

    void parallel(std::vector<int> &a, int l, int r);
}

#endif //ADV_MULTIPROC_QUICKSORT_HPP
