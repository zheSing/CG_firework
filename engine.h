#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "firework.h"
#include <vector>
using namespace std;

class Engine
{
private:
    vector<firework>* firework_list_ptr;
public:
    Engine(vector<firework>* ptr);
    ~Engine();
    // 产生一个烟花，随机初始化参数
    void create_firework_random(fireworktype type = fireworktype::mudan);
    // 根据烟花类型爆炸产生粒子
    void boom_firework(firework* fw, fireworktype type = fireworktype::mudan);
    // 引擎启动，根据物理定律更新烟花参数
    void engine_forward(firework* fw);
};

#endif