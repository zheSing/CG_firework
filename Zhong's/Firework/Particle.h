#ifndef ___Particle___
#define ___Particle___

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Particle
{
public:
	Particle();
	~Particle();
	glm::vec3 getPosition();	//��ȡ����λ��
	glm::vec3 getVelocity();	//��ȡ�����ٶ�
	glm::vec4 getColor();	//��ȡ������ɫ
	GLfloat getRadius();	//��ȡ���Ӱ뾶
	void setPosition(glm::vec3);	//��������λ��
	void setVelocity(glm::vec3);	//���������ٶ�
	void setColor(glm::vec4);	//����������ɫ
	void setRadius(GLfloat);	//�������Ӱ뾶
	static const GLfloat GRAVITY;	//�������ٶ�
private:
	glm::vec3 position;		//����λ��
	glm::vec3 velocity;		//�����ٶ�
	glm::vec4 color;		//������ɫ
	GLfloat radius;			//���Ӱ뾶
};


#endif // !___Particle___
