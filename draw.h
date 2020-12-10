#ifndef __DRAW_H__
#define __DRAW_H__

#include <glad/glad.h>
#include "firework.h"
#include "shader.h"
#include <vector>
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
    void draw_polygon(glm::vec3* position, GLint pos_cnt, GLfloat radius, glm::vec4 color, polygon type, Shader& myshader);
    void draw_firework(vector<Firework>::iterator fw, Shader& myshader);
};

#endif