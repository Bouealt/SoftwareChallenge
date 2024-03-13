#ifndef ROBOT_H
#define ROBOT_H
#include <bits/stdc++.h>
using namespace std;

class Node {
public:
    int x, y; // 节点的坐标
    int g, h; // g值和h值
    Node *parent; // 父节点

    Node(int x = 0, int y = 0, Node *parent = nullptr);

    int getF() const;
    bool operator>(const Node& other) const;
};


struct Robot
{
    int x, y, goods;      // 坐标, 是否携带货物（0,1）
    bool flag = false;    // 是否分配货物
    bool isStick = false; // 机器人是否被困住
    int status;           // 状态（0,1 恢复，正常）
    int mbx = 0;
    int mby = 0;         // 目标坐标

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
    // 机器人下一步是否会发生碰撞
    bool isNextStepSafe()
    {

    }

};

struct Berth
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
};

struct Boat
{
    int num, pos, status; // 货物数量, 泊位(-1表示虚拟点), 状态（0运输，1正常运行（正常状态或运输完成状态），2停泊）
};

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
#endif