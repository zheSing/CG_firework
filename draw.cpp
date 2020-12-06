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
void Draw::draw_polygon(glm::vec3 position, float radius, glm::vec4 color, polygon type, Shader& myshader)
{
    // 变换到中心，根据半径放缩比例
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(radius));
    myshader.setMat4("model", model);
    myshader.setVec4("vertexColor", color);
    // 渲染
    glBindVertexArray(VAO[type]);
    glDrawElements(GL_TRIANGLES, indices[type]->size(), GL_UNSIGNED_INT, 0);
}

// 绘图。根据烟花参数渲染图形
void Draw::draw_firework(firework* fw, Shader& myshader)
{
    if (fw->already_boom)
        draw_polygon(fw->position, fw->radius, fw->color, fw->shape, myshader);
    else
        for (int i = 0; i < fw->num_of_grain; i++)
        {
            grain* grain_ptr = fw->grain_list + i;
            draw_polygon(grain_ptr->position, grain_ptr->radius, grain_ptr->color, grain_ptr->shape, myshader);
        }
}