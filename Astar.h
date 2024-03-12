#ifndef ASTAR_H
#define ASTAR_H
#include <bits/stdc++.h>
#include "robot.h"
#include "Logger.h"
using namespace std;




struct NodeHash
{
    size_t operator()(const Node &node) const
    {
        return hash<int>()(node.x) ^ hash<int>()(node.y);
    }
};

bool isValid(int x, int y, const char map[N][N]);
vector<Node> aStar(const vector<vector<char>>& map, Node start, Node goal);
vector<Node> getNeighbors(const Node &node, const char map[N][N]);
int heuristic(const Node &a, const Node &b);
vector<Node> findPathForRobot(Robot &robot, Node goal, const char map[N][N]);
int getDirection(Robot &robot, Node nextStep);

#endif