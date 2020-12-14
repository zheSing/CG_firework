#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
using namespace std;

#define POLYGON_NUM 2
#define MAX_FIREWORK_NUMBER 15
#define POSITION_NUMBER 6
#define TYPE_NUM 4

enum polygon { sphere_t, cube_t };
enum fireworktype { mudan_t, diamond_t, heart_t, bullfinch_t };

class Particle
{
public:
    Particle();
    ~Particle();
    GLint getPositionCnt();
    glm::vec3 getPosition();
    glm::vec3* getPositionArr();
    glm::vec3 getVelocity();
    glm::vec4 getColor();
    GLfloat getRadius();
    polygon getShape();
    void setPositionCnt(GLint);
    void setPosition(glm::vec3);
    void setVelocity(glm::vec3);
    void setColor(glm::vec4);
    void setRadius(GLfloat);
    void setShape(polygon);
    static const GLfloat GRAVITY;
private:
    GLint position_cnt;                     //目前记录位置的数量，用于实现拖尾效果
    glm::vec3 position[POSITION_NUMBER];    //记录位置的数组，用于实现拖尾效果
    glm::vec3 velocity;
    glm::vec4 color;
    GLfloat radius;
    polygon shape;
};

#endif
