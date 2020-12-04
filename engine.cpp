#include "engine.h"

Engine::Engine(vector<firework>* ptr)
{
    firework_list_ptr = ptr;
}

// 产生一个烟花，随机初始化参数
void Engine::create_firework_random(fireworktype type = mudan)
{

}

// 根据烟花类型爆炸产生粒子
void Engine::boom_firework(firework* fw, fireworktype type = mudan)
{

}

// 引擎启动，根据物理定律更新烟花参数
void Engine::engine_forward(firework* fw)
{

}