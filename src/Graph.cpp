//
// Created by tobias on 24.10.19.
//

#include "Graph.h"

namespace decomposition_tree {

    Graph::Graph(const node_ n) {
        this->n = n;
        m = 0;
        edges = std::vector<std::vector<Edge *>>();
        for (node_ i = 0; i < n; i++) {
            edges.emplace_back(std::vector<Edge *>());
        }
    }

    Graph::~Graph() {
        for (node_ i = 0; i < n; i++) {
            for (auto e: edges[i]) {
                delete (e);
            }
        }
    }

    void Graph::addNode(const decomposition_tree::node_ node) {
        if (node >= n) {
            n = node + 1;
            while (edges.size() < n) {
                edges.emplace_back(std::vector<Edge *>());
            }
        }
    }

    void Graph::addEdge(const decomposition_tree::Edge &e) {
        if (e.getStart() >= n || e.getEnd() >= n) {
            addNode(e.getStart());
            addNode(e.getEnd());
            auto e1 = new decomposition_tree::Edge(e.getStart(), e.getEnd(), e.getWeight());
            auto e2 = new decomposition_tree::Edge(e.getEnd(), e.getStart(), e.getWeight());
            e1->setReverse(e2);
            e2->setReverse(e1);
            edges[e.getStart()].emplace_back(e1);
            edges[e.getEnd()].emplace_back(e2);
            m += 2;
            return;
        }
        for (auto eTmp: edges[e.getStart()]) {
            if (e.getEnd() == eTmp->getEnd()) {
                eTmp->increase_weight(e.getWeight());
                eTmp->getReverse()->increase_weight(e.getWeight());
                return;
            }
        }

        auto e1 = new decomposition_tree::Edge(e.getStart(), e.getEnd(), e.getWeight());
        auto e2 = new decomposition_tree::Edge(e.getEnd(), e.getStart(), e.getWeight());
        e1->setReverse(e2);
        e2->setReverse(e1);
        edges[e.getStart()].emplace_back(e1);
        edges[e.getEnd()].emplace_back(e2);
        m += 2;
    }
}

