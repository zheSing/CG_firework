#include "draw.h"
#include "generate_vertices.h"

void init_vertices()
{
    generate_sphere_vertices(sphere_vertices, sphere_indices);
    generate_cube_vertices(cube_vertices, cube_indices);
}

// 绑定基本图形缓存
Draw::Draw()
{
    init_vertices();
    glGenVertexArrays(POLYGON_NUM, VAO);
    glGenBuffers(POLYGON_NUM, VBO);
    glGenBuffers(POLYGON_NUM, EBO);
    for (int i = 0; i < POLYGON_NUM; i++)
    {
        glBindVertexArray(VAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, vertices[i]->size() * sizeof(float), &(*vertices[i])[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices[i]->size() * sizeof(int), &(*indices[i])[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
}

// 渲染单个图元
void Draw::draw_polygon(glm::vec3* position, int pos_cnt, float radius, glm::vec4 color, polygon type, Shader& myshader)
{
    for (int i = 0; i < pos_cnt; i++)
    {
        // 变换到中心，根据半径放缩比例
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position[POSITION_NUMBER - 1 - i]);
        model = glm::scale(model, glm::vec3(radius / (i + 1)));
        myshader.setMat4("model", model);
        glm::vec4 fadecolor = color;
        fadecolor.w /= (i + 1);
        myshader.setVec4("vertexColor", fadecolor);
        // 渲染
        glBindVertexArray(VAO[type]);
        glDrawElements(GL_TRIANGLES, indices[type]->size(), GL_UNSIGNED_INT, 0);
    }
}

// 绘图。根据烟花参数渲染图形
void Draw::draw_firework(Firework* fw, Shader& myshader)
{
    if (!fw->isExploded())
        draw_polygon(fw->getPosition(), fw->getPositionCnt(), fw->getRadius(), fw->getColor(), fw->getShape(), myshader);
    else
        for (int i = 0; i < fw->getParticleNum(); i++)
        {
            Particle* grain_ptr = fw->getParticles() + i;
            draw_polygon(grain_ptr->getPosition(), grain_ptr->getPositionCnt(), grain_ptr->getRadius(), grain_ptr->getColor(), grain_ptr->getShape(), myshader);
        }
}