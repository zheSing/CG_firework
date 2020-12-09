#ifndef ___Particle___
#define ___Particle___

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Particle
{
public:
	Particle();
	~Particle();
	glm::vec3 getPosition();	//获取粒子位置
	glm::vec3 getVelocity();	//获取粒子速度
	glm::vec4 getColor();	//获取粒子颜色
	GLfloat getRadius();	//获取粒子半径
	void setPosition(glm::vec3);	//设置粒子位置
	void setVelocity(glm::vec3);	//设置粒子速度
	void setColor(glm::vec4);	//设置粒子颜色
	void setRadius(GLfloat);	//设置粒子半径
	static const GLfloat GRAVITY;	//重力加速度
private:
	glm::vec3 position;		//粒子位置
	glm::vec3 velocity;		//粒子速度
	glm::vec4 color;		//粒子颜色
	GLfloat radius;			//粒子半径
};


#endif // !___Particle___
