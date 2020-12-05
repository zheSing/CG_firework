#include "firework.h"
#include "shader.h"
#include <vector>
#include <glad/glad.h>
#include <glm-master/glm/glm.hpp>
#include <glm-master/glm/gtc/matrix_transform.hpp>
#include <iostream>
using namespace std;

// enum polygon { sphere, cube };

vector<float> sphere_vertices;
vector<int> sphere_indices;

vector<float> cube_vertices;
vector<int> cube_indices;

vector<float>* vertices[] = { &sphere_vertices, &cube_vertices };
vector<int>* indices[] = { &sphere_indices, &cube_indices };

void init_vertices();

class Draw
{
private:
    GLuint VAO[POLYGON_NUM], VBO[POLYGON_NUM], EBO[POLYGON_NUM];
public:
    Draw();
    ~Draw();
    void draw_polygon(glm::vec3 position, float radius, glm::vec4 color, polygon type, Shader& myshader);
    void draw_firework(firework* fw, Shader& myshader);
};
