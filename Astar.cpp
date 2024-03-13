#include "Logger.h"
#include "Astar.h"
#include "robot.h"

Node::Node(int x, int y, Node *parent) : x(x), y(y), g(std::numeric_limits<int>::max()), h(0), parent(parent) {}

int Node::getF() const
{
    return g + h;
}

bool Node::operator>(const Node &other) const
{
    return getF() > other.getF();
}

size_t NodeHash::operator()(const Node &node) const
{
    return std::hash<int>()(node.x) ^ std::hash<int>()(node.y);
}

bool NodeEqual::operator()(const Node &a, const Node &b) const
{
    return a.x == b.x && a.y == b.y;
}

int AStar::heuristic(const Node &a, const Node &b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

// 添加 isValid 函数
bool AStar::isValid(int x, int y, const char map[N][N])
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

vector<Node> AStar::getNeighbors(const Node &node, const char map[N][N])
{
    vector<Node> neighbors;
    // 定义四个方向：下、上、右、左
    vector<std::pair<int, int>> directions{{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    for (const auto &dir : directions)
    {
        int newX = node.x + dir.first;
        int newY = node.y + dir.second;

        if (isValid(newX, newY, map))
        {
            neighbors.emplace_back(newX, newY);
        }
    }

    return neighbors;
}

vector<Node> AStar::FindPath(const char map[][N], Node start, Node goal)
{
    priority_queue<Node, vector<Node>, greater<Node>> openSet;
    unordered_map<Node, Node *, NodeHash, NodeEqual> allNodes;

    start.g = 0;
    start.h = heuristic(start, goal);
    openSet.push(start);
    allNodes[start] = new Node(start.x, start.y); // 存储节点指针以便于更新和回溯

    while (!openSet.empty())
    {
        Node current = openSet.top();
        openSet.pop();

        if (current.x == goal.x && current.y == goal.y)
        {
            vector<Node> path;
            Node *pathNode = allNodes[current];

            while (pathNode != nullptr)
            {
                path.push_back(*pathNode);
                pathNode = pathNode->parent;
            }

            reverse(path.begin(), path.end());

            // 清理动态分配的节点
            for (auto &node : allNodes)
            {
                delete node.second;
            }

            return path;
        }

        for (auto &neighbor : getNeighbors(current, map))
        {
            int tentative_g = current.g + 1;

            if (!allNodes.count(neighbor) || tentative_g < allNodes[neighbor]->g)
            {
                neighbor.g = tentative_g;
                neighbor.h = heuristic(neighbor, goal);
                neighbor.parent = allNodes[current];

                if (!allNodes.count(neighbor))
                {
                    openSet.push(neighbor);
                    allNodes[neighbor] = new Node(neighbor.x, neighbor.y, allNodes[current]);
                }
            }
        }
    }

    // 清理动态分配的节点
    for (auto &node : allNodes)
    {
        delete node.second;
    }

    return {}; // 路径未找到
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
        Direction = 0;
    }

    return Direction;
}

vector<Node> findPathForRobot(Robot &robot, Node goal, const char map[N][N])
{

    Node start(robot.x, robot.y);
    vector<Node> path = AStar::FindPath(ch, start, goal);
    // LOGE("the start is %d %d", start.x, start.y);
    // LOGE("the goal is %d %d", goal.x, goal.y);
    // for (const auto point : path)
    // {
    //     LOGE("Path node: (%d, %d),fuhao:%c", point.x, point.y, ch[point.x][point.y]);
    // }

    return path;
}
