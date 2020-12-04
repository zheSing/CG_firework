#include "draw.h"

// 绑定基本图形缓存
Draw::Draw()
{
    glGenVertexArrays(POLYGON_NUM, VAO);
    glGenBuffers(POLYGON_NUM, VBO);
    glGenBuffers(POLYGON_NUM, EBO);
    for (int i = 0; i < POLYGON_NUM; i++)
    {
        glBindVertexArray(VAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, vertex_size[i] * sizeof(float), vertices[i], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size[i] * sizeof(int), indices[i], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
}

// 渲染单个图元
void Draw::draw_polygon(glm::vec3 position, float radius, polygon type, Shader& myshader)
{
    // 变换到中心，根据半径放缩比例
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(radius));
    myshader.setMat4("model", model);
    // 渲染
    glBindVertexArray(VAO[type]);
    glDrawElements(GL_TRIANGLES, index_size[type], GL_UNSIGNED_INT, 0);
}

// 绘图。根据烟花参数渲染图形
void Draw::draw_firework(firework* fw, Shader& myshader)
{
    if (fw->already_boom)
        draw_polygon(fw->position, fw->radius, fw->shape, myshader);
    else
        for (int i = 0; i < fw->num_of_grain; i++)
        {
            grain* grain_ptr = fw->grain_list[i];
            draw_polygon(grain_ptr->position, grain_ptr->radius, grain_ptr->shape, myshader);
        }
}