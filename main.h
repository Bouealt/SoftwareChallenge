#ifndef MAIN_H
#define MAIN_H
#include <bits/stdc++.h>
#include "robot.h"
#include "Logger.h"

using namespace std;

const int n = 200;
const int robot_num = 10;
const int berth_num = 10;
const int N = 210;


extern int money, boat_capacity, id;
extern char ch[N][N];
extern Robot robot[robot_num + 10];
extern Berth berth[berth_num + 10];
extern Boat boat[10];
extern vector<Goods> goodslist;



#endif // MAIN_H
