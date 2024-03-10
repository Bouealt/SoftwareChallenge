// #include <bits/stdc++.h>


// using namespace std;

// const int n = 200;
// const int robot_num = 10;
// const int berth_num = 10;
// const int N = 210;

// struct Robot
// {
//     int x, y, goods; // 坐标, 是否携带货物（0,1）
//     int status;      // 状态（0,1 恢复，正常）
//     int mbx, mby;
//     Robot() {}
//     Robot(int startX, int startY)
//     {
//         x = startX;
//         y = startY;
//     }
// } robot[robot_num + 10];

// struct Berth
// {
//     int x;
//     int y;
//     int transport_time;
//     int loading_speed;
//     Berth() {}
//     Berth(int x, int y, int transport_time, int loading_speed)
//     {
//         this->x = x;
//         this->y = y;
//         this->transport_time = transport_time;
//         this->loading_speed = loading_speed;
//     }
// } berth[berth_num + 10];

// struct Boat
// {
//     int num, pos, status; // 货物数量, 泊位(-1表示虚拟点), 状态（0运输，1正常运行（正常状态或运输完成状态），2停泊）
// } boat[10];
// /**********************************/
// struct Goods
// {
//     int x, y, value, num; // 坐标，价值，货物数量
//     Goods() {}
//     Goods(int x, int y, int value, int num) : x(x), y(y), value(value), num(num) {}
// } goods[50];

// struct Node
// {
//     int x, y;     // 节点的坐标
//     int g, h;     // g值：从起点到当前节点的代价；h值：从当前节点到终点的启发式估计代价
//     Node *parent; // 指向父节点的指针，用于重建路径
    

//     Node(int x, int y, Node *parent = nullptr) : x(x), y(y), parent(parent), g(0), h(0) {}

//     int getF() const
//     {
//         return g + h;
//     }

//     bool operator==(const Node &o) const
//     {
//         return x == o.x && y == o.y;
//     }

//     bool operator<(const Node &o) const
//     {
//         return getF() > o.getF(); // 在优先队列中用于比较
//     }
// };

// struct NodeHash
// {
//     size_t operator()(const Node &node) const
//     {
//         return hash<int>()(node.x) ^ hash<int>()(node.y);
//     }
// };

// /**********************************/
// int money, boat_capacity, id; // 金钱, 船的容量, 帧序号
// char ch[N][N];                // 地图
// int gds[N][N];                // 货物
// void Init()                   // 初始化场景
// {
//     for (int i = 1; i <= n; i++) // 地图
//         scanf("%s", ch[i] + 1);
//     for (int i = 0; i < berth_num; i++) // 港口
//     {
//         int id;
//         scanf("%d", &id);
//         scanf("%d%d%d%d", &berth[id].x, &berth[id].y, &berth[id].transport_time, &berth[id].loading_speed);
//     }
//     scanf("%d", &boat_capacity); // 船的容量
//     char okk[100];
//     scanf("%s", okk);
//     printf("OK\n");
//     fflush(stdout);
// }

// int Input()
// {
//     scanf("%d%d", &id, &money); // 帧序号, 金钱
//     int num;
//     scanf("%d", &num);
//     for (int i = 0; i < num; i++) // 货物
//     {
//         int x, y, val;
//         scanf("%d%d%d", &x, &y, &val);
//         goods[i] = Goods(x, y, val, num);
//     }
//     for (int i = 0; i < robot_num; i++) // 机器人
//     {
//         int sts;
//         scanf("%d%d%d%d", &robot[i].goods, &robot[i].x, &robot[i].y, &sts);
//     }
//     for (int i = 0; i < 5; i++) // 船
//         scanf("%d%d\n", &boat[i].status, &boat[i].pos);
//     char okk[100];
//     scanf("%s", okk);
//     return id;
// }

// /***************************************/
// // 添加 A* 算法实现
// vector<Node> aStar(const vector<vector<char>>& map, Node start, Node goal) {
//     priority_queue<Node, vector<Node>, greater<>> openSet;  // 优先队列，根据 f 值排序
//     unordered_map<Node, Node, NodeHash> cameFrom;  // 用于重建路径

//     unordered_map<Node, int, NodeHash> gScore;  // 保存从起点到当前节点的代价
//     gScore[start] = 0;

//     unordered_map<Node, int, NodeHash> fScore;  // 保存从起点经过当前节点到达终点的估计代价
//     fScore[start] = heuristic(start, goal);

//     openSet.push(start);

//     while (!openSet.empty()) {
//         Node current = openSet.top();
//         openSet.pop();

//         // 如果到达目标节点，回溯并返回路径
//         if (current == goal) {
//             vector<Node> path;
//             while (cameFrom.find(current) != cameFrom.end()) {
//                 path.push_back(current);
//                 current = cameFrom[current];
//             }
//             reverse(path.begin(), path.end());
//             return path;
//         }

//         for (Node& neighbor : getNeighbors(current, map)) {
//             // 计算经过当前节点到达邻居节点的 g 值
//             int tentative_gScore = gScore[current] + 1;  // 假设每步代价为 1

//             if (tentative_gScore < gScore[neighbor]) {
//                 // 这条路径到邻居节点更短
//                 cameFrom[neighbor] = current;
//                 gScore[neighbor] = tentative_gScore;
//                 fScore[neighbor] = gScore[neighbor] + heuristic(neighbor, goal);

//                 if (find_in_priority_queue(openSet, neighbor) == false) {
//                     openSet.push(neighbor);
//                 }
//             }
//         }
//     }

//     return {};  // 如果无法到达目标节点，返回空路径
// }


// // 添加 isValid 函数
// bool isValid(int x, int y, const vector<vector<char>> &map)
// {
//     // 此处应包含 isValid 函数的实现
//     // 检查坐标是否超出地图边界
//     if (x < 0 || x >= map.size() || y < 0 || y >= map[0].size()) {
//         return false;
//     }

//     // 检查节点是否为可通行的
//     char cell = map[x][y];
//     return cell == '.' || cell == 'B' || cell == 'A';  // 初始位置，空地和泊位是可通行的
// }

// // 添加 getNeighbors 函数
// vector<Node> getNeighbors(const Node &node, const vector<vector<char>> &map)
// {
//     // 此处应包含 getNeighbors 函数的实现
// }
// int findClosestGoodsIndex(const Robot &robot, const Goods *goods, int num)
// {
//     int minDis = 0x3f3f3f3f;
//     int index = -1;
//     for (int i = 0; i < num; i++)
//     {
//         int dis = abs(robot.x - goods[i].x) + abs(robot.y - goods[i].y);
//         if (dis < minDis)
//         {
//             minDis = dis;
//             index = i;
//         }
//     }
//     return index;
// }


// // 添加 heuristic 函数
// int heuristic(const Node &a, const Node &b)
// {
//     // 此处应包含 heuristic 函数的实现
//     return abs(a.x - b.x) + abs(a.y - b.y);
// }

// void getGoods(int i)
// {
//     if (robot[i].goods == 0)
//     {

//         // 实现 findClosestGoodsIndex 和 getDirection 函数
//         int closestGoodsIndex = findClosestGoodsIndex(robot[i], goods, goods->num);
//         if(closestGoodsIndex == -1) return; // 没有货物了
//         Node goal(goods[closestGoodsIndex].x, goods[closestGoodsIndex].y);

//         vector<Node> path = findPathForRobot(robot[i], goal, ch);
//         if (!path.empty())
//         {
//             Node nextStep = path[1];
//             printf("move %d %d\n", i, getDirection(robot[i], nextStep));
//         }
//     }
// }

// /***********************************/

// int main()
// {
//     Init();
//     for (int zhen = 1; zhen <= 15000; zhen++)
//     {
//         int id = Input();
//         // 为每个机器人寻找到最近的货物作为目标
//         for (int i = 0; i < robot_num; i++)
//         {
//             getGoods(i);
//         }
//         puts("OK");
//         fflush(stdout);
//     }

//     return 0;
// }
