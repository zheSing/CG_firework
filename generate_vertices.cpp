
// // 生成基本图形的顶点
// void generate_polygon_vertices(glm::vec3 position, float radius, polygon type, vector<float>& polygon_vertices, vector<int>& polygon_indices)
// {
//     polygon_indices.clear();
//     polygon_vertices.clear();
//     if (sphere == type)
//     {
//         const int xmax = 10, ymax = 10;
//         for (int x = 0; x < xmax; x++)
//             for (int y = 0; y < ymax; y++)
//             {
//                 float xseg = (float)x / (float)xmax;
//                 float yseg = (float)y / (float)ymax;
//                 float xPos = std::cos(xseg * 2.0f * PI) * std::sin(yseg * PI);
//                 float yPos = std::cos(yseg * PI);
//                 float zPos = std::sin(xseg * 2.0f * PI) * std::sin(yseg * PI);
//                 polygon_vertices.push_back(xPos);
//                 polygon_vertices.push_back(yPos);
//                 polygon_vertices.push_back(zPos);
//             }
//         for (int i = 0;i < xmax;i++)
//             for (int j = 0;j < ymax;j++)
//             {
//                 polygon_indices.push_back(i * (ymax + 1) + j);
//                 polygon_indices.push_back((i + 1) * (ymax + 1) + j);
//                 polygon_indices.push_back((i + 1) * (ymax + 1) + j + 1);
//                 polygon_indices.push_back(i * (ymax + 1) + j);
//                 polygon_indices.push_back((i + 1) * (ymax + 1) + j + 1);
//                 polygon_indices.push_back(i * (ymax + 1) + j + 1);
//             }
//     }
//     else if (cube == type)
//     {
//     }
// }