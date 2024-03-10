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
static int gds[N][N];                // 货物

static struct Robot
{
    int x, y, goods; // 坐标, 是否携带货物（0,1）
    int status;      // 状态（0,1 恢复，正常）
    int mbx, mby;
    Robot() {}
    Robot(int startX, int startY)
    {
        x = startX;
        y = startY;
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

static struct Goods
{
    int x, y, value, num; // 坐标，价值，货物数量
    Goods() {}
    Goods(int x, int y, int value, int num) : x(x), y(y), value(value), num(num) {}
} goods[50];

void Init();
int Input();
int findClosestGoodsIndex(const Robot &robot, const Goods *goods, int num);
void getGoods(int i);
#endif