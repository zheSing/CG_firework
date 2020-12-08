#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "firework.h"
#include <vector>
using namespace std;

class Engine
{
private:
    float g;
    float delta;
    vector<firework>* firework_list_ptr;
public:
    Engine(vector<firework>* ptr, float delta = 0.3f, float g = -0.2f);
    ~Engine();
    // 产生一个烟花，随机初始化参数
    void create_firework_random(fireworktype type = fireworktype::mudan_t);
    // 根据烟花类型爆炸产生粒子
    static void boom_firework(vector<firework>::iterator fw, fireworktype type = fireworktype::mudan_t);
    // 引擎启动，根据物理定律更新烟花参数
    void forward();
};

#endif