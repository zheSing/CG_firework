#include <iostream>
#include <fstream>
#include <string>

#include <cstdio>

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

#include <vector>

#include <chrono>
#include <iostream>
#include <cstdlib>

#include "firework.h"

vector<firework>* firework_list_ptr;

using namespace std;


struct firework* create_firework_random(fireworktype type = mudan)
{
	struct firework* fw = (struct firework*)malloc(sizeof(struct firework));
	if (fw == nullptr) {
		//error!
		return nullptr;
	}

	fw->already_boom = false;
	fw->life_before_boom = ((float)rand()) / RAND_MAX * 100;
	fw->life_after_boom = ((float)rand()) / RAND_MAX * 100;

	fw->position = glm::vec3(((float)rand()) / RAND_MAX * 100, ((float)rand()) / RAND_MAX * 100, ((float)rand()) / RAND_MAX * 100);
	fw->speed = glm::vec3(((float)rand()) / RAND_MAX * 100, ((float)rand()) / RAND_MAX * 100, ((float)rand()) / RAND_MAX * 100);

	fw->color = glm::vec4(((float)rand()) / RAND_MAX * 255, ((float)rand()) / RAND_MAX * 255, ((float)rand()) / RAND_MAX * 255, 0);

	fw->radius = ((float)rand()) / RAND_MAX * 10;

	fw->life_of_light = ((float)rand()) / RAND_MAX * 10;

	fw->light_intensity = ((float)rand()) / RAND_MAX * 10;

	fw->light_color = glm::vec3(((float)rand()) / RAND_MAX * 255, ((float)rand()) / RAND_MAX * 255, ((float)rand()) / RAND_MAX * 255);

	fw->boom_type = mudan;

	return fw;
}

void firework_engine(firework* fw)
{
	if (!fw->already_boom)
	{
		fw->life_before_boom--;
		fw->speed[2] -= 9.8;
		fw->position = fw->position + fw->speed;
		if (fw->life_before_boom == 0)
		{
			fw->already_boom = true;
			for (size_t i = 0; i < sizeof(fw->grain_list) / sizeof(fw->grain_list[0]); i++)
			{
				fw->grain_list[i] = (struct grain*)malloc(sizeof(struct grain));
				fw->grain_list[i]->position = fw->position;
				fw->grain_list[i]->speed = glm::vec3(((float)rand()) / RAND_MAX * 100, ((float)rand()) / RAND_MAX * 100, ((float)rand()) / RAND_MAX * 100);
				fw->grain_list[i]->color = glm::vec4(((float)rand()) / RAND_MAX * 255, ((float)rand()) / RAND_MAX * 255, ((float)rand()) / RAND_MAX * 255, 0);
				fw->grain_list[i]->radius = ((float)rand()) / RAND_MAX * 10;
				fw->grain_list[i]->shape = sphere;
			}
		}
	}
	else
	{
		fw->life_after_boom--;
		for (size_t i = 0; i < sizeof(fw->grain_list) / sizeof(fw->grain_list[0]); i++)
		{
			fw->grain_list[i]->speed[2] -= 9.8;
			fw->grain_list[i]->position = fw->grain_list[i]->position + fw->grain_list[i]->speed;
		}
	}

	if (fw->life_before_boom == 0 && fw->life_after_boom == 0)
	{
	}
}
