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
	glm::vec3 position;//λ��
	glm::vec3 speed;//�ٶ�
	glm::vec4 color;//��ɫ

	float radius;//�뾶
	polygon shape;//��״
};


struct firework
{
	bool already_boom;//�Ƿ��Ѿ���ը
	float life_before_boom;//��ըǰ����
	float life_after_boom;//��ը������

	glm::vec3 position;//λ��
	glm::vec3 speed;//����

	glm::vec4 color;//��ɫ
	float radius;//�뾶
	polygon shape;//��״

	float life_of_light;//��Դ����
	float light_intensity;//����ǿ��
	glm::vec3 light_color;//��Դ��ɫ

	fireworktype boom_type;//��ը��ʽ


	vector<grain> grain_list;//����
	int grain_cnt;

	

};

firework *firework_new(fireworktype type);
void firework_update(firework *fw, float dt);

#endif // !__FIREWORK_H__
