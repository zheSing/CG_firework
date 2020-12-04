#include "firework.h"
#include "shader.h"
#include <vector>
#include <glad/glad.h>
#include <glm-master/glm/glm.hpp>
#include <glm-master/glm/gtc/matrix_transform.hpp>
#include <iostream>
using namespace std;

// enum polygon { sphere, cube };

int vertex_size[POLYGON_NUM] = { 100, 0 };
int index_size[POLYGON_NUM] = { 100, 0 };

float sphere_vertices[] = { 0.0f, 0.0f, 0.0f };
int sphere_indices[] = { 0, 1, 2 };

float cube_vertices[] = { 0.0f, 0.0f, 0.0f };
int cube_indices[] = { 0, 1, 2 };

float* vertices[POLYGON_NUM] = { sphere_vertices, cube_vertices };
int* indices[POLYGON_NUM] = { sphere_indices, cube_indices };

class Draw
{
private:
    GLuint VAO[POLYGON_NUM], VBO[POLYGON_NUM], EBO[POLYGON_NUM];
public:
    Draw();
    ~Draw();
    void draw_polygon(glm::vec3 position, float radius, polygon type, Shader& myshader);
    void draw_firework(firework* fw, Shader& myshader);
};
