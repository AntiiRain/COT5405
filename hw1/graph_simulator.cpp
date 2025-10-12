//
// Created by lennard on 2025-10-12.
//

#include "graph_simulator.h"
#include "graph_simulator.h"

// n-cycle 图: 0-1, 1-2, ..., (n-1)-0
Graph create_n_cycle(int n) {
  Graph g;
  if (n < 3) return g; // 环至少需要3个顶点
  for (int i = 0; i < n; ++i) {
    g.add_edge(i, (i + 1) % n);
    g.add_edge((i + 1) % n, i); // 无向图
  }
  return g;
}

// 完全图: 每个节点都与其他所有节点相连
Graph create_complete_graph(int n) {
  Graph g;
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      g.add_edge(i, j);
      g.add_edge(j, i); // 无向图
    }
  }
  return g;
}

// 空图: 有 n 个节点, 但没有边
Graph create_empty_graph(int n) {
  Graph g;
  for (int i = 0; i < n; ++i) {
    g.add_edge(i, i); // 添加一个自环或空邻居列表以确保节点存在
  }
  return g;
}