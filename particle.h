#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <GLFW/glfw3.h>
#include <glm-master/glm/glm.hpp>

#define POLYGON_NUM 2
#define MAX_GRAIN_PER_FIREWORK 30
#define MAX_FIREWORK_NUMBER 15
#define POSITION_NUMBER 4

enum polygon { sphere_t, cube_t };
enum fireworktype { mudan_t };

class Particle
{
public:
    Particle();
    ~Particle();
    GLint getPositionCnt();
    glm::vec3* getPosition();
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
    GLint position_cnt;
    glm::vec3 position[POSITION_NUMBER];
    glm::vec3 velocity;
    glm::vec4 color;
    GLfloat radius;
    polygon shape;
};

#endif