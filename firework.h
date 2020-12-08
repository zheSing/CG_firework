#ifndef __FIREWORK_H__
#define __FIREWORK_H__

#include<glm-master/glm/glm.hpp>
#define POLYGON_NUM 2
#define MAX_GRAIN_PER_FIREWORK 30
#define MAX_FIREWORK_NUMBER 15
#define POSITION_NUMBER 4

enum polygon { sphere_t, cube_t };
enum fireworktype { mudan_t };

struct grain
{
    int position_cnt;
    glm::vec3 position[POSITION_NUMBER];//位置
    glm::vec3 speed;//速度
    glm::vec4 color;//颜色

    float radius;//半径
    polygon shape;//形状
};


struct firework
{
    bool already_boom;//是否已经爆炸
    int life_before_boom;//爆炸前寿命
    int life_after_boom;//爆炸后寿命

    int position_cnt;
    glm::vec3 position[POSITION_NUMBER];//位置
    glm::vec3 speed;//方向

    glm::vec4 color;//颜色
    float radius;//半径
    polygon shape;//形状

    int life_of_light;//光源寿命
    float light_intensity;//光照强度
    glm::vec3 light_color;//光源颜色

    fireworktype boom_type;//爆炸方式

    int grain_cnt; //粒子数量

    grain grain_list[MAX_GRAIN_PER_FIREWORK];//粒子

};

#endif // !__FIREWORK_H__