#include <vector>
#include <glm-master/glm/glm.hpp>
#include <iostream>
#include <fstream>
#define PI 3.14159265358979323846
using namespace std;

// 生成球的顶点
void generate_sphere_vertices(vector<float>& polygon_vertices, vector<int>& polygon_indices)
{
    polygon_indices.clear();
    polygon_vertices.clear();

    glm::vec3 position = glm::vec3(0.0f);
    float radius = 1.0f;

    const int xmax = 10, ymax = 10, maximum = 100;
    for (int x = 0; x < xmax; x++)
        for (int y = 0; y < ymax; y++)
        {
            float xseg = (float)x / (float)xmax;
            float yseg = (float)y / (float)ymax;
            float xPos = std::cos(xseg * 2.0f * PI) * std::sin(yseg * PI);
            float yPos = std::cos(yseg * PI);
            float zPos = std::sin(xseg * 2.0f * PI) * std::sin(yseg * PI);
            sphere_vertices.push_back(xPos);
            sphere_vertices.push_back(yPos);
            sphere_vertices.push_back(zPos);
        }
    for (int i = 0;i < xmax;i++)
        for (int j = 0;j < ymax;j++)
        {
            sphere_indices.push_back((i * ymax + j) % maximum);
            sphere_indices.push_back(((i + 1) * ymax + j) % maximum);
            sphere_indices.push_back(((i + 1) * ymax + j + 1) % maximum);
            sphere_indices.push_back((i * ymax + j) % maximum);
            sphere_indices.push_back(((i + 1) * ymax + j + 1) % maximum);
            sphere_indices.push_back((i * ymax + j + 1) % maximum);
        }
}

void generate_cube_vertices(vector<float>& polygon_vertices, vector<int>& polygon_indices)
{

}