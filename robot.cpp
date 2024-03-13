#include "Astar.h"
#include "robot.h"
#include "Logger.h"

void Init() // 初始化场景
{
    for (int i = 0; i < n; i++) // 地图
        scanf("%s", ch[i]);

    // 读取数据，跳过索引0，从索引1开始，符合原始代码的处理方式
    // for (int i = 1; i <= n; i++)
    // {
    //     // 因为vector不能直接使用scanf读取整行
    //     for (int j = 1; j <= n; j++)
    //     {
    //         cin >> ch[i][j];
    //     }
    // }
    for (int i = 0; i < berth_num; i++) // 港口
    {
        int id;
        scanf("%d", &id);
        scanf("%d%d%d%d", &berth[id].x, &berth[id].y, &berth[id].transport_time, &berth[id].loading_speed);
    }
    scanf("%d", &boat_capacity); // 船的容量
    char okk[100];
    scanf("%s", okk);
    printf("OK\n");
    fflush(stdout);
}

int Input()
{
    scanf("%d%d", &id, &money); // 帧序号, 金钱
    int num;
    Goods goods;
    scanf("%d", &num);
    for (int i = 0; i < num; i++) // 货物
    {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        goods = Goods(x, y, val);
        goodslist.push_back(goods);
    }
    for (int i = 0; i < robot_num; i++) // 机器人
    {
        int sts;
        scanf("%d%d%d%d", &robot[i].goods, &robot[i].x, &robot[i].y, &sts);
    }
    for (int i = 0; i < 5; i++) // 船
        scanf("%d%d\n", &boat[i].status, &boat[i].pos);
    char okk[100];
    scanf("%s", okk);
    return id;
}

int findClosestGoods(Robot &robot)
{
    int minDis = 0x3f3f3f3f;
    int index = -1;

    for (int i = 0; i < goodslist.size(); i++)
    {
        int dis = abs(robot.x - goodslist[i].x) + abs(robot.y - goodslist[i].y);
        if (dis < minDis)
        {
            minDis = dis;
            index = i;
        }
    }
    if (index != -1) // 找到离当前机器人最近的货物
    {
        robot.flag = true;              // 标记机器人已分配货物
        robot.mbx = goodslist[index].x; // 机器人目标坐标为物品坐标
        robot.mby = goodslist[index].y;
        goodslist.erase(goodslist.begin() + index); // 删除已分配的货物
    }
    else
    {
        return index; // 未找到货物
    }
}
int findClosestBerth(Robot &robot)
{
    int minDis = 0x3f3f3f3f;
    int index = -1;

    for (int i = 0; i < berth_num; i++)
    {
        int dis = abs(robot.x - berth[i].x) + abs(robot.y - berth[i].y);
        if (dis < minDis)
        {
            minDis = dis;
            index = i;
        }
    }
    if (index != -1) // 找到离当前机器人最近的泊位
    {
        robot.mbx = berth[index].x; // 机器人目标坐标为泊位坐标
        robot.mby = berth[index].y;
    }
    else
    {
        return index; // 未找到泊位
    }
}

void getDis(int i)
{
    if (robot[i].goods == 0 && !goodslist.empty()) // 机器人没有携带货物，且货物数量不为0，分配货物
    {
        // LOGI("this is the robot:%d", i);
        if (!robot[i].flag) // 机器人没有分配货物
        {
            // 实现 findClosestGoodsIndex 和 getDirection 函数
            int ret = findClosestGoods(robot[i]);
            if (ret == -1)
            {
                LOGE(" ------------************ 未找到货物 ************------------");
                return;
            }
            // LOGI("the robot %d  dis position is %d %d\n",i,robot[i].mbx, robot[i].mby);
        }
    }
    // if (robot[i].goods) // 机器人携带货物，去卸货
    // {
    //     int ret = findClosestBerth(robot[i]);
    //     if (ret == -1)
    //     {
    //         LOGE(" ------------************ 未找到泊位 ************------------");
    //         return;
    //     }
    //     // LOGI("the robot %d  dis position is %d %d\n",i,robot[i].mbx, robot[i].mby);
    // }
    else
        return;
}

void movetoDis(int i) // 往目标方向移动
{
    // LOGI("the robot position %d   %d ", robot[i].x, robot[i].y);
    // LOGI("the robot dis position is %d %d", robot[i].mbx, robot[i].mby);
    Node goal(robot[i].mbx, robot[i].mby);

    // LOGI(" MOVE ");
    // LOGI("the robot position is %d %d\n", robot[i].x, robot[i].y);

    // LOGI("the goal position is %d %d\n", goal->x, goal->y);
    if ((goal.x == 0 && goal.y == 0) || (robot[i].x == goal.x && robot[i].y == goal.y)) // 机器人到达目标位置
    {
        return;
    }
    // LOGI("current robot is %d", i);
    // LOGI("robot path is %d", robot[i].path.empty());
    else if (robot[i].path.empty() && robot[i].status == 0) // 机器人没有路径或者发生碰撞，重新寻路
    {
        robot[i].path.clear(); // 清空路径
        robot[i].path = findPathForRobot(robot[i], goal, ch);
     
        if (robot[i].path.empty())
        {
            return;
        }
        robot[i].path.erase(robot[i].path.begin()); // 移除已经使用的节点
    }
    else
    {
        Node nextStep = robot[i].path.front();
        robot[i].path.erase(robot[i].path.begin()); // 移除已经使用的节点
        printf("move %d %d\n", i, getDirection(robot[i], nextStep));
    }
}