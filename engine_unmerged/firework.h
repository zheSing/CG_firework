#ifndef __FIREWORK_H__
#define __FIREWORK_H__


#include <glm/glm.hpp>
#include <vector>

using namespace std;


const unsigned int MAX_GRAIN_PER_FIREWORK = 30;

enum polygon { sphere_t, cube_t };
enum fireworktype { mudan_t };

struct grain
{
	glm::vec3 position;//位置
	glm::vec3 speed;//速度
	glm::vec4 color;//颜色

	float radius;//半径
	polygon shape;//形状
};


struct firework
{
	bool already_boom;//是否已经爆炸
	float life_before_boom;//爆炸前寿命
	float life_after_boom;//爆炸后寿命

	glm::vec3 position;//位置
	glm::vec3 speed;//方向

	glm::vec4 color;//颜色
	float radius;//半径
	polygon shape;//形状

	float life_of_light;//光源寿命
	float light_intensity;//光照强度
	glm::vec3 light_color;//光源颜色

	fireworktype boom_type;//爆炸方式


	vector<grain> grain_list;//粒子
	int grain_cnt;

	

};

firework *firework_new(fireworktype type);
void firework_update(firework *fw, float dt);

#endif // !__FIREWORK_H__
