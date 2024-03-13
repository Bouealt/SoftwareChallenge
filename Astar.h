#pragma once
#ifndef ASTAR_H
#define ASTAR_H
/*
A*算法对象类
*/
#include <bits/stdc++.h>
#include "main.h"

struct NodeHash {
    size_t operator()(const Node &node) const;
};

struct NodeEqual {
    bool operator()(const Node &a, const Node &b) const;
};

class AStar {
public:
    static vector<Node> FindPath(const char map[][N], Node start, Node goal);
private:
    static int heuristic(const Node &a, const Node &b);
    static bool isValid(int x, int y, const char map[N][N]);
    static vector<Node> getNeighbors(const Node &node, const char map[N][N]);
};

int getDirection(Robot &robot,  Node nextStep);
vector<Node> findPathForRobot(Robot &robot, Node goal, const char map[N][N]);

#endif