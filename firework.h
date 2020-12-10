#ifndef __FIREWORK_H__
#define __FIREWORK_H__

#include "particle.h"

class Firework
{
public:
    Firework(); // Constructor declaration
    ~Firework();
    void initialise();
    void move(float);
    void explode(float);
    // GLboolean isLaunched();
    // void updateFramesUntilLaunch();

    GLboolean isExploded();
    GLint getPositionCnt();
    glm::vec3 getPosition();
    glm::vec3* getPositionArr();
    glm::vec3 getVelocity();
    glm::vec4 getColor();
    GLfloat getRadius();
    GLint getParticleNum();
    GLint getParticleAliveNum();
    Particle* getParticles();

    polygon getShape();
    fireworktype getType();

    GLfloat getLightLife();
    GLfloat getLightIntensity();
    glm::vec3 getLightColor();

private:
    GLfloat lastTime;
    GLboolean hasExploded;
    GLint position_cnt;                     //目前记录位置的数量，用于实现拖尾效果
    glm::vec3 position[POSITION_NUMBER];    //记录位置的数组，用于实现拖尾效果
    glm::vec3 velocity;
    glm::vec4 color;
    GLfloat radius;
    polygon shape;
    fireworktype type;
    static const GLfloat GRAVITY;
    static const GLfloat radiusScale;       //初始半径的缩放比例，用于适应世界范围
    static const GLfloat velocityScale;     //初始速度的缩放比例，用于适应世界范围
    static const GLfloat explodeScale;      //爆炸速度的缩放比例，用于适应世界范围
    static const GLint minParticleNum = 10;
    static const GLint maxParticleNum = 15;
    GLint particleNum;
    GLint particleAliveNum;
    GLfloat light_life;                     //光源寿命
    GLfloat light_intensity;                //光照强度
    glm::vec3 light_color;                  //光源颜色
    Particle particles[maxParticleNum * maxParticleNum];
    glm::vec3 getSpherePoint(GLfloat, GLfloat);
    void velocitySample(GLuint Longitude, GLuint Latitude, std::vector<glm::vec3>& vertexes);
    glm::vec3 velocitySampleRandom();
    glm::vec3 rgb2xyz(glm::vec3);
    glm::vec3 xyz2rgb(glm::vec3);
};

/*
修改
1. 拖尾效果
2. 粒子颜色受烟花颜色影响
3. 加入缩放比例适应世界范围
4. 粒子速度不会过分受烟花速度影响
*/

#endif // !__FIREWORK_H__