#include "Astar.h"
#include "robot.h"


void Init()                   // 初始化场景
{
    for (int i = 1; i <= n; i++) // 地图
        scanf("%s", ch[i] + 1);
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
    scanf("%d", &num);
    for (int i = 0; i < num; i++) // 货物
    {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        goods[i] = Goods(x, y, val, num);
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


int findClosestGoodsIndex(const Robot &robot, const Goods *goods, int num)
{
    int minDis = 0x3f3f3f3f;
    int index = -1;
    for (int i = 0; i < num; i++)
    {
        int dis = abs(robot.x - goods[i].x) + abs(robot.y - goods[i].y);
        if (dis < minDis)
        {
            minDis = dis;
            index = i;
        }
    }
    return index;
}

void getGoods(int i)
{
    if (robot[i].goods == 0)
    {

        // 实现 findClosestGoodsIndex 和 getDirection 函数
        int closestGoodsIndex = findClosestGoodsIndex(robot[i], goods, goods->num);
        if(closestGoodsIndex == -1) return; // 没有货物了
        Node goal(goods[closestGoodsIndex].x, goods[closestGoodsIndex].y);

        vector<Node> path = findPathForRobot(robot[i], goal, ch);
        if (!path.empty())
        {
            Node nextStep = path[1];
            printf("move %d %d\n", i, getDirection(robot[i], nextStep));
            printf("move %d %d\n", 0, 0);
        }
    }
}

 