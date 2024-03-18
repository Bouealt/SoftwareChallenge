#include "Astar.h"
#include "main.h"

int money, boat_capacity, id, boatnum = 0; // 金钱, 船的容量, 帧序号
char ch[N][N];                             // 地图

Robot robot[robot_num + 10];
Berth berth[berth_num + 10];
Boat boat[10];
unordered_map<Goods, int, GoodsHash, GoodsEqual> goodslist;
// unordered_map<int, int> berthMap;

// unordered_map<Vec2, int, NodeHash, NodeEqual> currentMap;
// unordered_map<Vec2, int, NodeHash, NodeEqual> nextMap;

// 定义一个基于值的比较函数，用于降序排列

void robotLogic() // 机器人逻辑
{
    for (int i = 0; i < robot_num; i++)
    {
        // LOGE("berth %d num is %d", i, berth[i].num);
        if (robot[i].isStick)
            continue;
        findLoop(i);  // 寻找巡逻路线
        moveRobot(i); // 移动到目标
    }
    // findLoop(7);  // 寻找巡逻路线
    // moveRobot(7); // 移动到目标
    // findLoop(8);  // 寻找巡逻路线
    // moveRobot(8); // 移动到目标
}
void shipLogic() // 船逻辑
{
    if (id > 13500)
    {
        for (int i = 0; i < 5; i++)
        { // 送货
            if ((berth[boat[i].pos].transport_time + id) >= 14500 && !boat[i].isSale)
            {
                printf("go %d\n", i);
                boat[i].isSale = true;
            }
        }
    }
    // if (id == 7000)
    // {
    //     printf("ship %d %d\n", 3, 9);
    //     return;
    // }
    if (id % 2000 == 0)
    {
        for (int i = 0; i < 10; i++)
        {
            berth[i].flag = false; // 初始化
        }
        for (int i = 0; i < 5; i++)
        {
            Robot_unloading(i);
        }
    }
    else
        return;
}
int main()
{
    // initLogger("myapp.log"); // 初始化日志系统，将日志输出到 myapp.log 文件
    Init();
    // LOGE(" THE BOAT CAPACITY IS %d", boat_capacity);
    // for (int i = 0; i < 10; i++)
    // {
    //     LOGE("berth %d transport_time %d loading_speed %d", i, berth[i].transport_time, berth[i].loading_speed);
    // }
    // for (int i = 0; i < 5; i++)
    // {
    //     if (i == 1 || i == 2)
    //         continue;
    //     printf("ship %d %d\n", i, i);
    // }
    // printf("ship %d %d\n", 1, 5);
    // printf("ship %d %d\n", 2, 8);
    for (int zhen = 1; zhen <= 15000; zhen++)
    {

        // LOGE("current id is %d", id);
        id = Input();
        robotLogic(); // 机器人逻辑
        shipLogic();  // 船逻辑

        // currentMap.clear();
        // unordered_map.clear();
        puts("OK");
        fflush(stdout);
    }

    return 0;
}
