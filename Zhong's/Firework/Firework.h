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
	GLboolean isLaunched();	//判断火箭是否发射
	void updateFramesUntilLaunch();	//更新framesUntilLaunch
	GLboolean isExploded();	//判断火箭是否爆炸
	glm::vec3 getPosition();//获取火箭位置
	glm::vec3 getVelocity();//获取火箭速度
	glm::vec4 getColor();	//获取火箭颜色
	GLfloat getRadius();	//获取火箭半径
	GLint getParticleNum();	//获取粒子总数 = particleNum * particleNum
	Particle* getParticles();	//获取粒子列表
private:
	GLint framesUntilLaunch;//火箭发射延迟帧数
	GLboolean hasExploded;	//火箭是否爆炸
	glm::vec3 position;		//火箭位置
	glm::vec3 velocity;		//火箭速度
	glm::vec4 color;		//火箭颜色
	GLfloat radius;			//火箭半径
	static const GLfloat GRAVITY;	//重力加速度
	static const GLint minParticleNum = 10;	//火箭爆炸产生的最小粒子数量的平方根
	static const GLint maxParticleNum = 20;	//火箭爆炸产生的最大粒子数量的平方根
	GLint particleNum;		//火箭爆炸产生的粒子数量的平方根
	GLint particleAliveNum;		//火箭爆炸后存活的粒子数量
	Particle particles[maxParticleNum*maxParticleNum];	//火箭爆炸产生的粒子列表
	glm::vec3 getSpherePoint(GLfloat, GLfloat);	//计算球面点的坐标
	void velocitySample(GLuint Longitude, GLuint Latitude, std::vector<glm::vec3> &vertexes);	//均匀速度采样
	glm::vec3 velocitySampleRandom();	//随机速度采样
	glm::vec3 rgb2xyz(glm::vec3);	//RGB转XYZ
	glm::vec3 xyz2rgb(glm::vec3);	//XYZ转RGB
};

#endif // !___Firework___
