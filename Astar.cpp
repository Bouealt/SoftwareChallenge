#include "Astar.h"
#include "Logger.h"

vector<Node> aStar(const char map[N][N], Node start, Node goal)
{
    priority_queue<Node, vector<Node>, greater<Node>> openSet; // 优先队列，根据 f 值排序
    unordered_set<Node, NodeHash> openSetTracker;              // 用于跟踪 openSet 中的节点
    unordered_set<Node, NodeHash> closedSet;                    // 用于跟踪已经评估过的节点
    unordered_map<Node, Node, NodeHash> cameFrom;              // 用于重建路径

    start.g = 0;
    start.h = heuristic(start, goal);

    LOGE("this is the astar start : %***********************");
    openSet.push(start);
    openSetTracker.insert(start); // 在跟踪器中添加起点

    while (!openSet.empty())
    {
        Node current = openSet.top();
        openSet.pop();
        openSetTracker.erase(current); // 从跟踪器中移除当前节点
        // LOGI("\n\n");
        // LOGI("Current node: (%d, %d), g: %d, h: %d", current.x, current.y, current.g, current.h);
        // LOGI("Goal node: (%d, %d)", goal.x, goal.y);

        // 如果到达目标节点，回溯并返回路径
        if (current.x == goal.x && current.y == goal.y)
        {
            vector<Node> path;
            while (cameFrom.find(current) != cameFrom.end())
            {
                path.push_back(current);
                // LOGI("Path node: (%d, %d)", current.x, current.y);
                current = cameFrom[current];
            }
            reverse(path.begin(), path.end());
            // LOGI("\n\n");
            // LOGI("****************Goal reached, path reconstructed.***********");
            // LOGI("\n\n");

            return path;
        }
        closedSet.insert(current);  // 将当前节点添加到已评估集合中

        for (Node neighbor : getNeighbors(current, map))
        {
            if (closedSet.find(neighbor) != closedSet.end()) {
                continue;  // 如果邻居在 closedSet 中，跳过此邻居
            }

            // 计算经过当前节点到达邻居节点的 g 值
            int tentative_gScore = current.g + 1;
            // LOGI("Neighbor node: (%d, %d), g: %d, h: %d, tentative_gScore: %d", neighbor.x, neighbor.y, neighbor.g, neighbor.h, tentative_gScore);
            // LOGI("openSet size is %d", openSet.size());
            if (tentative_gScore < neighbor.g)
            {
                // 如果邻居在 openSet 中，更新
                if (openSetTracker.find(neighbor) != openSetTracker.end())
                {
                    // 这条路径到邻居节点更短
                    cameFrom[neighbor] = current;
                    // LOGI("Updating cameFrom for node: (%d, %d)", neighbor.x, neighbor.y);
                    neighbor.g = tentative_gScore;
                    neighbor.h = heuristic(neighbor, goal);
                }

                // 如果邻居不在 openSet 中，添加它
                else if (openSetTracker.find(neighbor) == openSetTracker.end())
                {
                    openSet.push(neighbor);
                    openSetTracker.insert(neighbor); // 在跟踪器中添加邻居节点
                    // LOGI("Adding node to openSet and openSetTracker: (%d, %d)", neighbor.x, neighbor.y);
                }
            }
        }
        // LOGE("the loop count is %d",count);
        // LOGI("\n\n");
    }
    // LOGI("\n\n");
    // LOGI("find path failed***************");
    // LOGI("\n\n");

    return {}; // 如果无法到达目标节点，返回空路径
}

// 添加 isValid 函数
bool isValid(int x, int y, const char map[N][N])
{
    // 此处应包含 isValid 函数的实现
    // 检查坐标是否超出地图边界
    if (x < 0 || x >= n || y < 0 || y >= n)
    {
        return false;
    }

    // 检查节点是否为可通行的
    char cell = map[x][y];
    return cell == '.' || cell == 'B' || cell == 'A'; // 初始位置，空地和泊位是可通行的
}

vector<Node> getNeighbors(const Node &node, const char map[N][N])
{
    vector<Node> neighbors;

    // 向上
    if (isValid(node.x - 1, node.y, map))
    {
        // LOGI(" UP   *******************************************");
        neighbors.emplace_back(node.x - 1, node.y);
    }
    // 向下
    if (isValid(node.x + 1, node.y, map))
    {
        // LOGI(" DOWN   *******************************************");
        neighbors.emplace_back(node.x + 1, node.y);
    }
    // 向左
    if (isValid(node.x, node.y - 1, map))
    {
        // LOGI(" LEFT   *******************************************");
        neighbors.emplace_back(node.x, node.y - 1);
    }
    // 向右
    if (isValid(node.x, node.y + 1, map))
    {
        // LOGI(" RIGNT   *******************************************");
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

vector<Node> findPathForRobot(Robot &robot, Node goal, const char map[N][N])
{
    Node start(robot.x, robot.y);
    vector<Node> path = aStar(map, start, goal);
    return path;
}

int getDirection(Robot &robot, Node nextStep)
{
    int Direction;
    if (robot.x == nextStep.x && robot.y == nextStep.y - 1)
    { // 下一步向右
        Direction = 0;
    }
    else if (robot.x == nextStep.x && robot.y == nextStep.y + 1)
    { // 下一步向左
        Direction = 1;
    }
    else if (robot.x == nextStep.x - 1 && robot.y == nextStep.y)
    { // 下一步向下
        Direction = 3;
    }
    else if (robot.x == nextStep.x + 1 && robot.y == nextStep.y)
    { // 下一步向上
        Direction = 2;
    }
    else
    {
        Direction = 1;
    }

    return Direction;
}