#include "firework.h"
#include <iostream>


#ifndef M_PI
#define M_PI    3.14159265358979323846264338327950288
#endif


static float g = -9.8f;

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

static void firework_boom(firework *fw)
{
	float explosion_speed = 9.0f;
	grain gn;
	for (int i = 0; i < fw->grain_cnt; i++) {
		gn.position = fw->position;
		gn.speed = explosion_speed * sample_vel();
		gn.color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		gn.radius = 15.0f;
		gn.shape = sphere_t;
		fw->grain_list.push_back(gn);
	}
}

firework *firework_new(fireworktype type)
{
	firework *fw = new firework;
	if (fw == nullptr)
	{
		cout << "memory error!" << endl;
		return nullptr;
	}

	int seed = 0;

	fw->already_boom = false;

	seed = 1 + (rand() % static_cast<int>(5));
	fw->life_before_boom = ((float)seed) * 1.6f;
	fw->life_after_boom = ((float)seed) * 1.5f;

	seed = 0 + (rand() % static_cast<int>(5));
	fw->position = initial_position[seed];

	fw->speed = glm::vec3(0.0f + noise(0.1f), 0.0f + noise(0.1f), 15.0f + noise(0.1f));

	fw->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);

	fw->radius = 15.0f;
	fw->shape = sphere_t;

	fw->life_of_light = fw->life_before_boom + fw->life_after_boom;

	fw->light_intensity = 1.0f;
	fw->light_color = glm::vec3(1.0f, 1.0f, 1.0f);

	fw->boom_type = mudan_t;

	fw->grain_list.reserve(30);
	fw->grain_cnt = 10 + (rand() % static_cast<int>(21));

	return fw;
}


void firework_update(firework *fw, float dt)
{
	if (!fw->already_boom)
	{
		fw->position += (fw->speed * dt);
		fw->speed.z += (g * dt);
		fw->life_before_boom -= dt;
		if (fw->life_before_boom <= 0) {
			firework_boom(fw);
			fw->already_boom = true;
		}
	}
	else
	{
		for (auto it = fw->grain_list.begin(); it != fw->grain_list.end(); ++it)
		{
			it->position += (it->speed * dt);
			it->speed.z += (g * dt);
		}
		fw->life_after_boom -= dt;
		if (fw->life_after_boom <= 0) {
			fw->grain_list.clear();
		}
	}
}
