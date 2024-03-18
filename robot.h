#ifndef ROBOT_H
#define ROBOT_H
#include <bits/stdc++.h>
using namespace std;

struct Vec2
{
    int x, y;
    Vec2() {}
    Vec2(int x, int y) : x(x), y(y) {}
};

class Node
{
public:
    int x, y;     // 节点的坐标
    int g, h;     // g值和h值
    Node *parent; // 父节点

    Node(int x = 0, int y = 0, Node *parent = nullptr);

    int getF() const;
    bool operator>(const Node &other) const;
};

struct Goods
{
    int x, y, value; // 坐标，价值
    Goods() {}
    Goods(int x, int y, int value) : x(x), y(y), value(value) {}
};
struct GoodsHash
{
    size_t operator()(const Goods &goods) const
    {
        return std::hash<int>()(goods.x) ^ (std::hash<int>()(goods.y) << 1) ^ (std::hash<int>()(goods.value) << 2);
    }
};
struct GoodsEqual
{
    bool operator()(const Goods &lhs, const Goods &rhs) const
    {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.value == rhs.value;
    }
};

struct Berth
{
    int x;
    int y;
    int transport_time;
    int loading_speed;
    bool flag = false;        // 港口是否有船
    bool isAllocated = false; // 是否被分配

    int num = 0; // 港口货物数量
    Berth() {}
    Berth(int x, int y, int transport_time, int loading_speed)
    {
        this->x = x;
        this->y = y;
        this->transport_time = transport_time;
        this->loading_speed = loading_speed;
    }
};

struct Boat
{
    int num, pos, status; // 货物数量, 泊位(-1表示虚拟点), 状态（0运输，1正常运行（正常状态或运输完成状态），2停泊）
    int berth;            // 绑定的港口
    int zhen;             // 指令帧数
    bool isSale = false;          // 是否出售
};

struct Robot
{
    int x, y, goods; // 坐标, 是否携带货物（0,1）

    bool findGoods = false;   // 判断是否是拿货状态
    bool isLoop = false;      // 判断是否是巡逻状态
    bool ispullGoods = false; // 判断是否是放货状态
    bool isStick = false;     // 判断是否被困状态

    bool toBerth = true; // 是否前往泊位
    int status;          // 状态（0,1 恢复，正常）
    int mbx = 0;
    int mby = 0; // 货物目标坐标
    int berth;
    int mavalue = 0; // 目标评判价值
    int mbb;         // 目标泊位

    list<Node> to_Berth_path;                // 缓存路径_泊位
    list<Node> to_Berth_path_copy;           // 缓存路径_泊位备份
    list<Node> to_Berth_Path_reverse;        // 缓存反向泊位路径
    list<Node> to_Goods_path;                // 缓存路径_货物
    stack<Node> to_Berth_Path_reverse_stack; // 缓存反向泊位路径备份
    stack<Node> to_Goods_path_reverse;       // 缓存货物路径反向

    Robot() {}
    Robot(int startX, int startY)
    {
        x = startX;
        y = startY;
    }
};
void Init();
int Input();
int findClosestGoods(Robot &robot);
int findClosestBerth(Robot &robot);
void findLoop(int i);
void findGoods(int i);
void copyRobotPath_B(int i);
void reversePath_B(int i);
void copyReversePath_B(int i);
void Loop(int i);
void getGoods(int i);
void putGoods(int i);
void moveRobot(int i);

// bool avoid_collision(int i, Node nextStep);

void Robot_unloading(int i);
#endif