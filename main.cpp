#include <bits/stdc++.h>
#include "Astar.h"
#include "robot.h"

using namespace std;

int main()
{
    Init();
    for (int zhen = 1; zhen <= 15000; zhen++)
    {

        int id = Input();
        // 为每个机器人寻找到最近的货物作为目标
        for (int i = 0; i < robot_num; i++)
        {
            LOGE("current id is %d", id);
            LOGE("current zhen is %d", zhen);
            LOGE("current robot is %d", i);
            getDis(i);    // 为机器人分配货物或分配泊位
            movetoDis(i); // 移动到目标
            // LOGE("\n\n\n");
        }

        puts("OK");
        fflush(stdout);
    }

    return 0;
}
