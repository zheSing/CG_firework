#ifndef ___Firework___
#define ___Firework___

#include "Particle.h"
#include <vector>

class Firework
{
public:
	Firework(); // Constructor declaration
	~Firework();
	void initialise();
	void move(float);
	void explode(float);
	GLboolean isLaunched();	//�жϻ���Ƿ���
	void updateFramesUntilLaunch();	//����framesUntilLaunch
	GLboolean isExploded();	//�жϻ���Ƿ�ը
	glm::vec3 getPosition();//��ȡ���λ��
	glm::vec3 getVelocity();//��ȡ����ٶ�
	glm::vec4 getColor();	//��ȡ�����ɫ
	GLfloat getRadius();	//��ȡ����뾶
	GLint getParticleNum();	//��ȡ�������� = particleNum * particleNum
	Particle* getParticles();	//��ȡ�����б�
private:
	GLint framesUntilLaunch;//��������ӳ�֡��
	GLboolean hasExploded;	//����Ƿ�ը
	glm::vec3 position;		//���λ��
	glm::vec3 velocity;		//����ٶ�
	glm::vec4 color;		//�����ɫ
	GLfloat radius;			//����뾶
	static const GLfloat GRAVITY;	//�������ٶ�
	static const GLint minParticleNum = 10;	//�����ը��������С����������ƽ����
	static const GLint maxParticleNum = 20;	//�����ը�������������������ƽ����
	GLint particleNum;		//�����ը����������������ƽ����
	GLint particleAliveNum;		//�����ը�������������
	Particle particles[maxParticleNum * maxParticleNum];	//�����ը�����������б�
	glm::vec3 getSpherePoint(GLfloat, GLfloat);	//��������������
	void velocitySample(GLuint Longitude, GLuint Latitude, std::vector<glm::vec3>& vertexes);	//�����ٶȲ���
	glm::vec3 velocitySampleRandom();	//����ٶȲ���
	glm::vec3 rgb2xyz(glm::vec3);	//RGBתXYZ
	glm::vec3 xyz2rgb(glm::vec3);	//XYZתRGB
};

#endif // !___Firework___
