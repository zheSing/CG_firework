#ifndef __DRAW_H__
#define __DRAW_H__

#include "firework.h"
#include "shader.h"
#include <vector>
#include <glad/glad.h>
#include <glm-master/glm/glm.hpp>
#include <glm-master/glm/gtc/matrix_transform.hpp>
#include <iostream>
using namespace std;


void init_vertices();

class Draw
{
private:
    GLuint VAO[POLYGON_NUM], VBO[POLYGON_NUM], EBO[POLYGON_NUM];
public:
    Draw();
    ~Draw();
    void draw_polygon(glm::vec3* position, int pos_cnt, float radius, glm::vec4 color, polygon type, Shader& myshader);
    void draw_firework(Firework* fw, Shader& myshader);
};

#endif