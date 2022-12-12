#ifndef BFS_GRAPH_HPP
#define BFS_GRAPH_HPP

#include "vector"

namespace node {
    int x(int size, int vertex) {
        return vertex == 0 ? 0 : vertex % size;
    }

    int y(int size, int vertex) {
        return vertex == 0 ? 0 : (vertex / size) % size;
    }

    int z(int size, int vertex) {
        return vertex == 0 ? 0 : (vertex / size) / size;
    }

    int compute_vertex(int size, int x, int y, int z) {
        return (size * z + y) * size + x;
    }
}

class Graph {
public:
    int total{};

    virtual std::vector<int> neighbors(int vertex) {
        return std::vector<int>(0);
    }
};

class Graph3D : public Graph {
private:
    std::vector<int> do_neighbors(int x, int y, int z) {
        std::vector<int> neighbors(0);
        if (x < border) {
            neighbors.push_back(node::compute_vertex(size, x + 1, y, z));
        }

        if (y < border) {
            neighbors.push_back(node::compute_vertex(size, x, y + 1, z));
        }

        if (z < border) {
            neighbors.push_back(node::compute_vertex(size, x, y, z + 1));
        }

        return neighbors;
    }

public:
    int size;
    int border;

    Graph3D(int size) {
        assert(size != 0 && size != 1);
        this->size = size;
        this->border = size - 1;
        this->total = size * size * size;
    }

    std::vector<int> neighbors(int vertex) override {
        int x = node::x(size, vertex);
        int y = node::y(size, vertex);
        int z = node::z(size, vertex);

        return do_neighbors(x, y, z);
    }
};

#endif //BFS_GRAPH_HPP
