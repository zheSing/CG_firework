#include "engine.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static glm::vec3 initial_position[5] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(-5.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, -5.0f, 0.0f),
    glm::vec3(-5.0f, 5.0f, 0.0f),
    glm::vec3(5.0f, -5.0f, 0.0f)
};

// generate random float in [-0.5, +0.5] * scale
static float noise(float scale) {
    return scale * static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5;
}

static 	glm::vec3 sample_vel() {
    float theta = 2 * M_PI * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);  // 0 - 2PI
    float phi = M_PI * static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // 0 - PI
    float noise = 1 - (0.1f * static_cast <float> (rand()) / static_cast <float> (RAND_MAX)); // 0.9 - 1
    return noise * glm::vec3(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi));
}

Engine::Engine(vector<firework>* ptr, float delta, float g)
{
    this->g = g;
    this->delta = delta;
    this->firework_list_ptr = ptr;
}

// 产生一个烟花，随机初始化参数
void Engine::create_firework_random(fireworktype type)
{
    firework fw;

    int seed = 0;

    fw.already_boom = false;

    seed = 1 + (rand() % static_cast<int>(5));
    fw.life_before_boom = ((float)seed) * 10;
    fw.life_after_boom = ((float)seed) * 5;

    seed = 0 + (rand() % static_cast<int>(5));
    fw.position = initial_position[seed];

    fw.speed = glm::vec3(0.0f + noise(0.1f), 10.0f + noise(0.1f), 0.0f + noise(0.1f));

    fw.color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

    fw.radius = 2.0f;
    fw.shape = sphere_t;

    fw.life_of_light = fw.life_before_boom + fw.life_after_boom;

    fw.light_intensity = 1.0f;
    fw.light_color = glm::vec3(1.0f, 1.0f, 1.0f);

    fw.boom_type = mudan_t;

    //fw.grain_list.reserve(30);
    fw.grain_cnt = 10 + (rand() % static_cast<int>(21));

    this->firework_list_ptr->push_back(fw);
}

// 根据烟花类型爆炸产生粒子
void Engine::boom_firework(vector<firework>::iterator fw, fireworktype type)
{
    float explosion_speed = 4.5f;
    grain gn;
    for (int i = 0; i < fw->grain_cnt; i++) {
        gn.position = fw->position;
        gn.speed = explosion_speed * sample_vel();
        gn.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        gn.radius = 2.0f;
        gn.shape = sphere_t;
        fw->grain_list[i] = gn;
    }
}

// 引擎启动，根据物理定律更新烟花参数
void Engine::forward()
{
    float dt = this->delta;
    for (auto firework_iter = this->firework_list_ptr->begin(); firework_iter != this->firework_list_ptr->end();)
    {
        if (!firework_iter->already_boom)
        {
            firework_iter->position += (firework_iter->speed * dt);
            firework_iter->speed.y += (g * dt);
            firework_iter->life_before_boom -= dt;
            if (firework_iter->life_before_boom <= 0)
            {
                this->boom_firework(firework_iter);
                firework_iter->already_boom = true;
            }
            firework_iter++;
        }
        else
        {
            for (int i = 0; i < firework_iter->grain_cnt; i++)
            {
                grain* grain_iter = firework_iter->grain_list + i;
                grain_iter->position += (grain_iter->speed * dt);
                grain_iter->speed.y += (g * dt);
            }
            firework_iter->life_after_boom -= dt;
            firework_iter->life_of_light -= dt;
            if (firework_iter->life_after_boom <= 0)
            {
                firework_iter = this->firework_list_ptr->erase(firework_iter);
                //cout << 1 << endl;
            }
            else
                firework_iter++;
        }
    }
}