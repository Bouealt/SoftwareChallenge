#include "Astar.h"

vector<Node> aStar(const vector<vector<char>>& map, Node start, Node goal) {
    priority_queue<Node, vector<Node>, greater<Node>> openSet;  // 优先队列，根据 f 值排序
    unordered_map<Node, Node, NodeHash> cameFrom;  // 用于重建路径

    unordered_map<Node, int, NodeHash> gScore;  // 保存从起点到当前节点的代价
    gScore[start] = 0;

    unordered_map<Node, int, NodeHash> fScore;  // 保存从起点经过当前节点到达终点的估计代价
    fScore[start] = heuristic(start, goal);

    openSet.push(start);

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        // 如果到达目标节点，回溯并返回路径
        if (current == goal) {
            vector<Node> path;
            while (cameFrom.find(current) != cameFrom.end()) {
                path.push_back(current);
                current = cameFrom[current];
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for (Node& neighbor : getNeighbors(current, map)) {
            // 计算经过当前节点到达邻居节点的 g 值
            int tentative_gScore = gScore[current] + 1;  // 假设每步代价为 1

            if (tentative_gScore < gScore[neighbor]) {
                // 这条路径到邻居节点更短
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_gScore;
                fScore[neighbor] = gScore[neighbor] + heuristic(neighbor, goal);
                bool flag = 0;
                priority_queue<Node, vector<Node>, greater<Node>> tempQueue = openSet;
                while (!tempQueue.empty()) {
                    Node topNode = tempQueue.top();
                    if(neighbor.x == topNode.x && neighbor.y == topNode.y)
                    {
                        flag = 1;
                    }
                    tempQueue.pop();
                }
                if(flag == 0)
                {
                    openSet.push(neighbor);
                }

            }
        }
    }

    return {};  // 如果无法到达目标节点，返回空路径
}

// 添加 isValid 函数
bool isValid(int x, int y, const vector<vector<char>> &map)
{
    // 此处应包含 isValid 函数的实现
    // 检查坐标是否超出地图边界
    if (x < 0 || x >= map.size() || y < 0 || y >= map[0].size()) {
        return false;
    }

    // 检查节点是否为可通行的
    char cell = map[x][y];
    return cell == '.' || cell == 'B' || cell == 'A';  // 初始位置，空地和泊位是可通行的
}

// 添加 getNeighbors 函数
// vector<Node> getNeighbors(const Node &node, const vector<vector<char>> &map)
// {
//     // 此处应包含 getNeighbors 函数的实现
//        vector<Node> neighbors;

//     // 向上
//     if (isValid(node.x - 1, node.y, map)) {
//         neighbors.emplace_back(node.x - 1, node.y, &node);
//     }
//     // 向下
//     if (isValid(node.x + 1, node.y, map)) {
//         neighbors.emplace_back(node.x + 1, node.y, &node);
//     }
//     // 向左
//     if (isValid(node.x, node.y - 1, map)) {
//         neighbors.emplace_back(node.x, node.y - 1, &node);
//     }
//     // 向右
//     if (isValid(node.x, node.y + 1, map)) {
//         neighbors.emplace_back(node.x, node.y + 1, &node);
//     }

//     return neighbors;
// }
vector<Node> getNeighbors(const Node &node, const vector<vector<char>> &map) {
    vector<Node> neighbors;

    // 向上
    if (isValid(node.x - 1, node.y, map)) {
        neighbors.emplace_back(node.x - 1, node.y);
    }
    // 向下
    if (isValid(node.x + 1, node.y, map)) {
        neighbors.emplace_back(node.x + 1, node.y);
    }
    // 向左
    if (isValid(node.x, node.y - 1, map)) {
        neighbors.emplace_back(node.x, node.y - 1);
    }
    // 向右
    if (isValid(node.x, node.y + 1, map)) {
        neighbors.emplace_back(node.x, node.y + 1);
    }

    return neighbors;
}


// 添加 heuristic 函数
int heuristic(const Node &a, const Node &b)
{
    // 此处应包含 heuristic 函数的实现
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// vector<Node> findPathForRobot(Robot &robot, Node goal, char ch[N][N])
// {
//     // vector<vector<char>> map;
//     vector<vector<char>> map(N, vector<char>(N)); // 预分配大小
//     for (int i = 0; i < N; ++i) {
//         std::copy(ch[i], ch[i] + N, map[i].begin());
//     }
//     Node start(robot.x, robot.y);
//     for (int i = 0; i < N; ++i) {
//         for (int j = 0; j < N; ++j) {
//             map[i][j] = ch[i][j];
//         }
//     }
//     vector<Node> path = aStar(map, start, goal);
//     return path;
// }
vector<Node> findPathForRobot(Robot &robot, Node goal, char ch[N][N]) {
    vector<vector<char>> map(N, vector<char>(N)); // 预分配大小
    for (int i = 0; i < N; ++i) {
        std::copy(ch[i], ch[i] + N, map[i].begin());
    }
    Node start(robot.x, robot.y);
    vector<Node> path = aStar(map, start, goal);
    return path;
}


int getDirection(Robot &robot, Node nextStep)
{
    int Direction;
    if (robot.x == nextStep.x && robot.y == nextStep.y - 1) {                   //下一步向右
        Direction = 0;
    } else if (robot.x == nextStep.x && robot.y == nextStep.y + 1) {            //下一步向左
        Direction = 1;
    } else if (robot.x == nextStep.x - 1 && robot.y == nextStep.y) {            //下一步向下
        Direction = 3;
    } else if (robot.x == nextStep.x + 1 && robot.y == nextStep.y) {            //下一步向上
        Direction = 2;
    } else {
        Direction = 1;
    }

    return Direction;
}