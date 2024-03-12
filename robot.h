#ifndef ROBOT_H
#define ROBOT_H
#include <bits/stdc++.h>
#include "Astar.h"
using namespace std;

const int n = 200;
const int robot_num = 10;
const int berth_num = 10;
const int N = 210;

static int money, boat_capacity, id; // 金钱, 船的容量, 帧序号
static char ch[N][N];                // 地图
// static const vector<vector<char>> ch;   // 地图

struct Node
{
   
    int x, y;     // 节点的坐标
    int g, h;     // g值和h值

    Node() : x(0), y(0), g(INT_MAX), h(INT_MAX) {}
    Node(int x, int y) : x(x), y(y), g(INT_MAX), h(INT_MAX) {}
    
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

static struct Robot
{
    int x, y, goods; // 坐标, 是否携带货物（0,1）
    bool flag = false; // 是否分配货物
    bool isStick = false; // 机器人是否被困住
    int status;      // 状态（0,1 恢复，正常）
    int mbx, mby;   // 目标坐标
    vector<Node> path; // 缓存路径
    Robot() {}
    Robot(int startX, int startY)
    {
        x = startX;
        y = startY;
    }

    // 获取并移除下一个路径节点
    Node getNextPathNode() {
        if (!path.empty()) {
            Node nextNode = path.front();
            path.erase(path.begin()); // 移除已经使用的节点
            return nextNode;
        }
        // 如果路径为空，返回当前位置
        return Node(x, y);
    }

    // 检查路径是否为空
    bool isPathEmpty() {
        return path.empty();
    }
    // 清除路径
    void clearPath() {
        path.clear();
    }
    // 机器人下一步是否会发生碰撞
    bool isNextStepSafe()
    {

    }

} robot[robot_num + 10];

static struct Berth
{         
    int x;
    int y;
    int transport_time;
    int loading_speed;
    Berth() {}
    Berth(int x, int y, int transport_time, int loading_speed)
    {
        this->x = x;
        this->y = y;
        this->transport_time = transport_time;
        this->loading_speed = loading_speed;
    }
} berth[berth_num + 10];

static struct Boat
{
    int num, pos, status; // 货物数量, 泊位(-1表示虚拟点), 状态（0运输，1正常运行（正常状态或运输完成状态），2停泊）
} boat[10];

struct Goods
{
    int x, y, value; // 坐标，价值
    Goods() {}
    Goods(int x, int y, int value) : x(x), y(y), value(value) {}
};

void Init();
int Input();
int findClosestGoods(Robot &robot);
int findClosestBerth(Robot &robot);
void getDis(int i);
void movetoDis(int i);
static vector<Goods> goodslist;          // 货物列表
#endif