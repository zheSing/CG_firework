#ifndef __DRAW_H__
#define __DRAW_H__

#include <glad/glad.h>
#include "firework.h"
#include "shader.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
using namespace std;

// 生成粒子形状的顶点
void init_vertices();

class Draw
{
private:
    // 粒子形状顶点的缓存
    GLuint VAO[POLYGON_NUM], VBO[POLYGON_NUM], EBO[POLYGON_NUM];
public:
    // 绑定顶点到缓存中
    Draw();
    // 释放缓存
    ~Draw();
    // 绘制基本形状
    void draw_polygon(glm::vec3* position, GLint pos_cnt, GLfloat radius, glm::vec4 color, polygon type, Shader& myshader);
    // 绘制未爆炸的烟花，或已经爆炸的烟花的粒子
    void draw_firework(vector<Firework>::iterator fw, Shader& myshader);
};

#endif