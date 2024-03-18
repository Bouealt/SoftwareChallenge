#include "Astar.h"
#include "robot.h"
#include "Logger.h"
int go = 0;
void Init() // 初始化场景
{
    for (int i = 0; i < n; i++) // 地图
        scanf("%s", ch[i]);
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
        goodslist[goods] = id; // 添加货物
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
    int timeout = 939;
    Goods closestGoods;     // 用来存储最近的货物
    vector<Goods> toRemove; // 用来存储需要移除的货物
    bool found = false;
    // int k = 15; // 权重
    for (const auto &pair : goodslist)
    {
        const Goods &goods = pair.first;
        int dis = abs(robot.x - goods.x) + abs(robot.y - goods.y) ;
        if (id - pair.second >= timeout)
        {                              // 如果货物超时
            toRemove.push_back(goods); // 标记为待移除
            continue;
        }
        if (dis >= 25) //--------------------------拾取范围
            continue;  // 拾取范围
        if (dis < minDis)
        {
            minDis = dis;
            closestGoods = goods;
            found = true;
        }
    }
    // 移除所有标记为超时的货物
    for (const Goods &goods : toRemove)
    {
        goodslist.erase(goods);
    }
    if (found)
    {
        robot.mbx = closestGoods.x; // 机器人目标坐标为物品坐标
        robot.mby = closestGoods.y;
        goodslist.erase(closestGoods); // 删除已分配的货物
        return 0;                      // 返回找到的货物的ID
    }

    return -1; // 未找到货物
}

int findClosestBerth(Robot &robot)
{
    int minDis = 0x3f3f3f3f;
    int index = -1;
    int k = 6; // 权重
    for (int i = 0; i < berth_num; i++)
    {
        int dis = abs(robot.x - berth[i].x) + abs(robot.y - berth[i].y) - berth[i].loading_speed * k;
        if (dis < minDis)
        {
            minDis = dis;
            index = i;
        }
    }
    if (index != -1 && !berth[index].isAllocated) // 找到离当前机器人最近的泊位，且该泊位未分配
    // if (index != -1) // 找到离当前机器人最近的泊位，且该泊位未分配
    {
        LOGE(" berth %d num is %d", index, berth[index].num);
        robot.mbx = berth[index].x; // 机器人目标坐标为泊位坐标
        robot.mby = berth[index].y;
        robot.berth = index;
        berth[index].isAllocated = true; // 标记泊位已分配
        if (boatnum < 5)
            printf("ship %d %d\n", boatnum++, index);
        // robot.isStick = false; // 标记机器人被困
    }
    else
    {
        // robot.isStick = true; // 标记机器人未被困
        return index; // 未找到泊位
    }
}
// 寻找巡逻路线
void findLoop(int i)
{
    if (!robot[i].to_Berth_path.empty())
        return;                           // 机器人已经找到巡逻路线
    if (findClosestBerth(robot[i]) == -1) // 绑定最近的泊位
    {
        // LOGE(" ------------************ 未找到泊位 ************------------");
        return;
    };

    Node goal(robot[i].mbx, robot[i].mby);
    robot[i].to_Berth_path = findPathForRobot(robot[i], goal, ch);
    robot[i].isLoop = true;       // 标记机器人已找到巡逻路线
    robot[i].ispullGoods = false; // 非送货
    robot[i].findGoods = false;   // 非拿货
    if (robot[i].to_Berth_path.empty())
        robot[i].isStick = true; // 标记机器人被困
    else
        robot[i].isStick = false; // 标记机器人未被困
    return;
}
// 寻找最近货物
void findGoods(int i)
{

    if (robot[i].goods || robot[i].findGoods || ch[robot[i].x][robot[i].y] == 'B')
        return;                           // 机器人携带货物或已经分配货物，不需要再次分配货物
    if (findClosestGoods(robot[i]) == -1) // 绑定最近的货物
    {
        // LOGE(" ------------************ 未找到货物 ************------------");
        return;
    };
    Node goal(robot[i].mbx, robot[i].mby);
    Node start(robot[i].x, robot[i].y);
    robot[i].to_Goods_path = findPathForRobot(robot[i], goal, ch); // 寻找货物的路径
    if (!robot[i].to_Goods_path.empty())
        robot[i].to_Goods_path.pop_front(); // 将第一个弹出
    else if(robot[i].to_Goods_path.size() > 80 ||  robot[i].to_Goods_path.empty())
    {
        robot[i].findGoods = false;   // 标记机器人已标记货物
        robot[i].isLoop = true;       // 标记机器人未找到巡逻路线
        robot[i].ispullGoods = false; // 非送货
        // robot[i].to_Berth_Path_reverse_stack.pop(); // 将第一个弹出
        return;
    }
    Node start1(robot[i].x, robot[i].y);
    robot[i].to_Goods_path_reverse.push(start); // 压栈，用于返回时的目标
    robot[i].findGoods = true;                  // 标记机器人已标记货物
    robot[i].isLoop = false;                    // 标记机器人未找到巡逻路线
    robot[i].ispullGoods = false;               // 非送货
    return;
}
void copyRobotPath_B(int i)
{ // 备份路径

    if (robot[i].to_Berth_path_copy.empty() && !robot[i].to_Berth_path.empty()) // 备份路径为空，且路径不为空
    {
        robot[i].to_Berth_path_copy = robot[i].to_Berth_path; // 备份路径
        robot[i].to_Berth_path_copy.pop_front();
        robot[i].toBerth = true; // 标记机器人已经在去港口的路上
        // for (const auto &node : robot[i].to_Berth_path_copy)
        // {
        //     LOGE("GO TO Berth:robot:%d(%d, %d),%C ", i, node.x, node.y, ch[node.x][node.y]);
        // }
    }
    else
    {
        return;
    }
}
void reversePath_B(int i)
{
    if (robot[i].to_Berth_Path_reverse.empty() && !robot[i].to_Berth_path.empty())
    {
        robot[i].to_Berth_Path_reverse = robot[i].to_Berth_path;
        reverse(robot[i].to_Berth_Path_reverse.begin(), robot[i].to_Berth_Path_reverse.end());
        if (robot[i].to_Berth_Path_reverse_stack.empty())
        {
            Node start(robot[i].x, robot[i].y);
            robot[i].to_Berth_Path_reverse_stack.push(start);
        }
        robot[i].to_Berth_Path_reverse.pop_front(); // 移除top节点-----
    }
    else
    {
        return;
    }
}
void Loop(int i)
{
    // LOGE(" LOOOP");
    // LOGE(" loop the robot is %d %d ", robot[i].x, robot[i].y);
    if (robot[i].toBerth) // 去港口的路上
    {
        // LOGE(" toBerth ");
        if (ch[robot[i].x][robot[i].y] == 'A')
        {
            copyRobotPath_B(i); // 起点获得返回路径
            // LOGE("*******************************");
        }
        if (!robot[i].to_Berth_path_copy.empty()) // 巡逻路线
        {
            Node nextStep = robot[i].to_Berth_path_copy.front();
            robot[i].to_Berth_path_copy.pop_front(); // 移除已经使用的节点
            printf("move %d %d\n", i, getDirection(robot[i], nextStep));
            // LOGE("PATH toberth loop :(%d,%d)", nextStep.x, nextStep.y);
            return;
        }
        else if (robot[i].x == berth[robot[i].berth].x && robot[i].y == berth[robot[i].berth].y && robot[i].goods)
        {                           // 已经到了港口
            printf("pull %d\n", i); // 将货物放到港口
            // berthMap[robot[i].berth] = ++berth[robot[i].berth].num; // 港口货物数量+1
            robot[i].toBerth = false; // 已经到了港口 *****
            robot[i].goods = 0;
            return;
        }
        else
        {
            // LOGE(" the robot is %d %d ", robot[i].x, robot[i].y);
            // LOGE(" the goal berth %d %d ", berth[robot[i].berth].x, berth[robot[i].berth].y);
            // 到港口，但没有货物
            if (robot[i].x == berth[robot[i].berth].x && robot[i].y == berth[robot[i].berth].y)
            {
                robot[i].toBerth = false; // 已经到了港口 *****
                // LOGE(" the robot is %d %d ", robot[i].x, robot[i].y);
            }
            return;
        }
    }
    else if (!robot[i].toBerth) // 返回起点的路上
    {
        // LOGE(" toStart   ");
        if (!robot[i].toBerth && ch[robot[i].x][robot[i].y] == 'B' && !robot[i].goods)
        if (robot[i].x == berth[robot[i].berth].x && robot[i].y == berth[robot[i].berth].y)
        {
            // LOGE("*****************************");
            reversePath_B(i);
        }
        // 起点获得返回路径
        if (!robot[i].to_Berth_Path_reverse.empty() && !robot[i].goods) // 巡逻路线
        {
            Node nextStep = robot[i].to_Berth_Path_reverse.front();
            robot[i].to_Berth_Path_reverse_stack.push(nextStep); // 压栈，用于返回 ***********
            robot[i].to_Berth_Path_reverse.pop_front();          // 移除已经使用的节点
            printf("move %d %d\n", i, getDirection(robot[i], nextStep));
            return;
        }
        else if (ch[robot[i].x][robot[i].y] == 'A' && !robot[i].goods)
        { // 已经到了起点
            while (!robot[i].to_Berth_Path_reverse_stack.empty())
            { // 清空返回路径栈
                robot[i].to_Berth_Path_reverse_stack.pop();
            }
            robot[i].toBerth = true; // 已经到了港口 *****
            return;
        }
        else
        {
            // LOGE(" to response error");
            return;
        }
    }
    else
    {
        // LOGE("Loop error");
    }
}
void getGoods(int i)
{
    // LOGE(" getGoods");
    // LOGE(" goods the robot is %d %d ", robot[i].x, robot[i].y);
    if (robot[i].toBerth) // 去港口的路上
    {
        if (!robot[i].to_Goods_path.empty() && !robot[i].goods)
        {
            Node nextStep = robot[i].to_Goods_path.front();
            robot[i].to_Goods_path_reverse.push(nextStep); // 压栈，用于返回
            robot[i].to_Goods_path.pop_front();            // 移除已经使用的节点
            printf("move %d %d\n", i, getDirection(robot[i], nextStep));
            // LOGE("PATH TO GOODS(toberth):(%d,%d)", nextStep.x, nextStep.y);
            return;
        }
        else if (robot[i].x == robot[i].mbx && robot[i].y == robot[i].mby && !robot[i].goods) // 到达货物
        {
            robot[i].to_Goods_path_reverse.pop(); // 出栈
            printf("get %d\n", i);                // 拿货
            // LOGE("GET GOOD");
            robot[i].goods = 1;                   // 标记机器人已经拿到货物
            return;
        }
        else if (!robot[i].to_Goods_path_reverse.empty())
        { // 返回巡逻路线
            // LOGE("GOODS IS OR NOT %d" , robot[i].goods);
            // LOGE("**************************");
            Node nextStep = robot[i].to_Goods_path_reverse.top();
            robot[i].to_Goods_path_reverse.pop(); // 出栈
            printf("move %d %d\n", i, getDirection(robot[i], nextStep));
            // LOGE("GOODS  TO PATH(toberth):(%d,%d)", nextStep.x, nextStep.y);
            return;
        }
        else
        {
            robot[i].isLoop = true;     // 标记机器人已经回到巡逻路线
            robot[i].findGoods = false; // 标记机器人拿完货物
            robot[i].ispullGoods = false;
        }
    }
    else if (!robot[i].toBerth) // 返回起点的路上
    {

        if (!robot[i].to_Goods_path.empty() && !robot[i].goods)
        { // 路径不为空，去货物路上
            Node nextStep = robot[i].to_Goods_path.front();
            robot[i].to_Goods_path_reverse.push(nextStep);                // 压栈，用于返回
            robot[i].to_Goods_path.erase(robot[i].to_Goods_path.begin()); // 移除已经使用的节点
            printf("move %d %d\n", i, getDirection(robot[i], nextStep));
            // LOGE("PATH TO GOODS(torespon):(%d,%d)", nextStep.x, nextStep.y);
            return;
        }
        else if (robot[i].x == robot[i].mbx && robot[i].y == robot[i].mby && !robot[i].goods) // 到达货物
        {
            robot[i].to_Goods_path_reverse.pop(); // 出栈
            printf("get %d\n", i);                // 拿货
            robot[i].goods = 1;                   // 标记机器人已经拿到货物
            return;
        }
        else if (!robot[i].to_Goods_path_reverse.empty() && robot[i].goods)
        { // 返回巡逻路线
            Node nextStep = robot[i].to_Goods_path_reverse.top();
            robot[i].to_Goods_path_reverse.pop(); // 出栈
            // robot[i].to_Berth_Path_reverse_stack.pop(); // 出栈
            printf("move %d %d\n", i, getDirection(robot[i], nextStep));
            // LOGE("good to PATH(torespon):(%d,%d)", nextStep.x, nextStep.y);
            return;
        }
        else
        {
            // LOGE("GET GOOD OVER");
            robot[i].to_Berth_Path_reverse_stack.pop(); // 将第一个弹出
            robot[i].ispullGoods = true;                // 去放货物
            robot[i].findGoods = false;                 // 标记机器人拿完货物
            robot[i].isLoop = false;                    // 标记机器人已经回到巡逻路线
        }
    }
    else
    {
        // LOGE("getGoods error");
        return;
    }
}
void putGoods(int i)
{
    // LOGE(" putGoods");
    // LOGE(" put the robot is %d %d ", robot[i].x, robot[i].y);
    if (robot[i].goods && (robot[i].x != berth[robot[i].berth].x || robot[i].y != berth[robot[i].berth].y)) // 已拿到，去放
    {
        Node nextStep = robot[i].to_Berth_Path_reverse_stack.top();
        robot[i].to_Berth_Path_reverse_stack.pop(); // 出栈
        printf("move %d %d\n", i, getDirection(robot[i], nextStep));
        // LOGE("putting good to :(%d,%d)", nextStep.x, nextStep.y);
        // LOGE("%c ", ch[robot[i].x][robot[i].y]);
        // LOGE(" %d ", robot[i].toBerth);
        // LOGE(" %d ", robot[i].goods);
        return;
    }
    else if (robot[i].x == berth[robot[i].berth].x && robot[i].y == berth[robot[i].berth].y && robot[i].goods)
    { // 已经到了港口
        // LOGE(" PULL111111111111111111111111");
        printf("pull %d\n", i); // 将货物放到港口
        // berthMap[robot[i].berth] = ++berth[robot[i].berth].num; // 港口货物数量+1
        robot[i].goods = 0;
        robot[i].to_Berth_Path_reverse.clear();
        while (!robot[i].to_Berth_Path_reverse_stack.empty())
        { // 清空返回路径栈
            Node s = robot[i].to_Berth_Path_reverse_stack.top();
            robot[i].to_Berth_Path_reverse_stack.pop();
        }
        robot[i].isLoop = true; // 标记机器人已经回到巡逻路线
        robot[i].toBerth = false;
        robot[i].ispullGoods = false;
        robot[i].findGoods = false;
        return;
    }
    else
    {
        return;
    }
}

void moveRobot(int i) // 往目标方向移动
{
    findGoods(i); // 搜索货物
    if (robot[i].isLoop)
        Loop(i); // 巡逻
    else if (robot[i].findGoods)
        getGoods(i); // 找货
    else if (robot[i].ispullGoods)
        putGoods(i); // 返回时放货
    else
    {
        // LOGE("move error");
    }
}


// void Robot_unloading(int i)
// {
//     // LOGE("Robot_unloading");
//     int maxKey = -1;
//     int maxValue = numeric_limits<int>::min();

//     // if (berthMap.empty())
//     //     return;
//     // for (auto &pair : berthMap)
//     // {
//     //     if (pair.second > maxValue && !berth[pair.first].flag)
//     //     {
//     //         maxKey = pair.first;
//     //         maxValue = pair.second;
//     //     }
//     // }
//     if ((boat[i].status == 1 || boat[i].status == 2) && boat[i].num != boat_capacity && maxKey != -1)
//     {
//         printf("ship %d %d\n", i, maxKey);
//         berth[maxKey].flag = true;
//     }

//     for (int j = 0; j < 5; j++)
//     {
//         if (boat[j].num >= boat_capacity && boat[j].status == 2)
//         {
//             printf("go %d\n", j);
//         }
//     }
// }