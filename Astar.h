#ifndef ASTAR_H
#define ASTAR_H
#include <bits/stdc++.h>
#include "robot.h"
using namespace std;

struct Node
{
    // int x, y;     // 节点的坐标
    // int g, h;     // g值：从起点到当前节点的代价；h值：从当前节点到终点的启发式估计代价
    // Node *parent; // 指向父节点的指针，用于重建路径
    

    // Node(int x, int y, Node* parent = nullptr) : x(x), y(y), parent(parent), g(0), h(0) {}

    int x, y;     // 节点的坐标
    int g, h;     // g值和h值

    Node() : x(0), y(0), g(0), h(0) {}
    Node(int x, int y) : x(x), y(y), g(0), h(0) {}
    
    int getF() const
    {
        return g + h;
    }

    bool operator==(const Node &o) const
    {
        return x == o.x && y == o.y;
    }

    bool operator<(const Node &o) const
    {
        return getF() > o.getF(); // 在优先队列中用于比较
    }
    bool operator>(const Node& other) const {
        return getF() < other.getF();
    }

};

struct NodeHash
{
    size_t operator()(const Node &node) const
    {
        return hash<int>()(node.x) ^ hash<int>()(node.y);
    }
};

bool isValid(int x, int y, const vector<vector<char>> &map);
vector<Node> aStar(const vector<vector<char>>& map, Node start, Node goal);
vector<Node> getNeighbors(const Node &node, const vector<vector<char>> &map);
int heuristic(const Node &a, const Node &b);
vector<Node> findPathForRobot(Robot &robot, Node goal, char ch[N][N]);
int getDirection(Robot &robot, Node nextStep);

#endif