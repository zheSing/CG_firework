#ifndef __FIREWORK_H__
#define __FIREWORK_H__

#include "particle.h"

// struct grain
// {
//     int position_cnt;
//     glm::vec3 position[POSITION_NUMBER];//位置
//     glm::vec3 speed;//速度
//     glm::vec4 color;//颜色

//     float radius;//半径
//     polygon shape;//形状
// };


// struct firework
// {
//     bool already_boom;//是否已经爆炸
//     int life_before_boom;//爆炸前寿命
//     int life_after_boom;//爆炸后寿命

//     int position_cnt;
//     glm::vec3 position[POSITION_NUMBER];//位置
//     glm::vec3 speed;//方向

//     glm::vec4 color;//颜色
//     float radius;//半径
//     polygon shape;//形状

//     int life_of_light;//光源寿命
//     float light_intensity;//光照强度
//     glm::vec3 light_color;//光源颜色

//     fireworktype boom_type;//爆炸方式

//     int grain_cnt; //粒子数量

//     grain grain_list[MAX_GRAIN_PER_FIREWORK];//粒子

// };

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
    Particle* getParticles();

    polygon getShape();
    fireworktype getType();

    GLfloat getLightLife();      //光源寿命
    GLfloat getLightIntensity();    //光照强度
    glm::vec3 getLightColor();      //光源颜色

private:
    GLboolean hasExploded;
    GLint position_cnt;
    glm::vec3 position[POSITION_NUMBER];
    glm::vec3 velocity;
    glm::vec4 color;
    GLfloat radius;
    polygon shape;
    fireworktype type;
    static const GLfloat GRAVITY;
    static const GLint minParticleNum = 10;
    static const GLint maxParticleNum = 20;
    GLint particleNum;
    GLint particleAliveNum;
    GLfloat light_life;      //光源寿命
    GLfloat light_intensity;    //光照强度
    glm::vec3 light_color;      //光源颜色
    Particle particles[maxParticleNum * maxParticleNum];
    glm::vec3 getSpherePoint(GLfloat, GLfloat);
    void velocitySample(GLuint Longitude, GLuint Latitude, std::vector<glm::vec3>& vertexes);
    glm::vec3 velocitySampleRandom();
    glm::vec3 rgb2xyz(glm::vec3);
    glm::vec3 xyz2rgb(glm::vec3);
};

#endif // !__FIREWORK_H__