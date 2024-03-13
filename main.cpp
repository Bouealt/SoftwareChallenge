#include "Astar.h"


int money, boat_capacity, id; // 金钱, 船的容量, 帧序号
char ch[N][N];                // 地图
Robot robot[robot_num + 10];
Berth berth[berth_num + 10];
Boat boat[10];
vector<Goods> goodslist;



int main()
{
    // initLogger("myapp.log"); // 初始化日志系统，将日志输出到 myapp.log 文件
    Init();

    for (int zhen = 1; zhen <= 15000; zhen++)
    {

        // LOGE("current id is %d", id);
        int id = Input();
        // 为每个机器人寻找到最近的货物作为目标
        for (int i = 0; i < robot_num; i++)
        {
            
            // LOGE("current zhen is %d", zhen);
            // LOGE("current robot is %d", i);
            getDis(i); // 为机器人分配货物或分配泊位
            movetoDis(i); // 移动到目标
            // LOGE("\n\n\n");
        }
        puts("OK");
        fflush(stdout);
    }

    return 0;
}
