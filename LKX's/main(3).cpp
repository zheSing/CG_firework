#include <glad/glad.h>
#include "firework.h"
#include "draw.h"
#include "camera.h"
#include "shader.h"
#include "shading.h"
#include <vector>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <glm-master/glm/glm.hpp>
#include <glm-master/glm/gtc/matrix_transform.hpp>
#include <glm-master/glm/gtc/type_ptr.hpp>
#include <iostream>
using namespace std;


// 烟花列表
vector<Firework> firework_list;

// 摄像机
Camera camera(glm::vec3(0.0f, 150.0f, 225.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float NEAR = 0.1f;
float FAR = 300.0f;
bool firstMouse = true;

// 时间
float deltaTime = 0.0f;
float lastFrame = 0.0f;
const float dt = 0.1f;

// 回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
bool ENTER_PRESS = false;

// 传递点光源函数
void set_point_light(Shader& blinnphongshader);

int main()
{
    // 初始化glfw设置
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FIREWORK", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // 捕获鼠标
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // 加载OpenGL函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //帧缓冲
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    //颜色缓冲区
    GLuint texColorBuffer[2];
    glGenTextures(2, texColorBuffer);
    init_framebuffer(framebuffer, texColorBuffer);

    // Blur
    GLuint BlurFBO[2];
    GLuint BlurColorbuffers[2];
    glGenFramebuffers(2, BlurFBO);
    glGenTextures(2, BlurColorbuffers);
    init_Blur(BlurFBO, BlurColorbuffers);
    //矩形顶点
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    init_rectangle(VAO, VBO, EBO);

    Shader ColorShader("Color.vs", "Color.fs");
    Shader BlinnPhongShader("Blinn_Phong.vs", "Blinn_Phong.fs");
    Shader BlurShader("Result.vs", "Blur.fs");
    Shader ResultShader("Result.vs", "Result.fs");

    // 开启深度测试
    glEnable(GL_DEPTH_TEST);
    // 绑定基本图元
    Draw draw;

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 更新时间
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 接收输入
        processInput(window);

        // 背景颜色
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // 设置普通shader
        ColorShader.use();
        // ColorShader.setVec3("viewPos", camera.Position);

        // 视角变换、投影变换
        // 世界变换交给draw_firework函数
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, NEAR, FAR);
        glm::mat4 view = camera.GetViewMatrix();
        ColorShader.setMat4("projection", projection);
        ColorShader.setMat4("view", view);

        // 渲染烟花系统
        // 应用烟花引擎
        for (vector<Firework>::iterator firework_it = firework_list.begin(); firework_it != firework_list.end();)
        {
            draw.draw_firework(firework_it, ColorShader);
            firework_it->move(dt);
            //判定是否爆炸及是否生存期到
            if (firework_it->isExploded() && firework_it->getParticleAliveNum() <= 0)
            {
                firework_it = firework_list.erase(firework_it);
                std::cout << "Delete a firework." << std::endl;
            }
            else
            {
                firework_it++;
            }
        }
        BlurShading(BlurFBO,BlurColorbuffers,texColorBuffer[1],VAO,BlurShader);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ResultShading(texColorBuffer[0], BlurColorbuffers[0], VAO, ResultShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// 判断按键并执行相应动作
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        if (!ENTER_PRESS)
        {
            Firework newfirework;
            firework_list.push_back(newfirework);
        }
        ENTER_PRESS = true;
    }
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
        ENTER_PRESS = false;
}

// 窗口回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// 鼠标移动回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// 放大缩小回调函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
// 传递点光源给着色器
void set_point_light(Shader& blinnphongshader)
{
    int count = 0;
    string struct_string = "light_list[";
    string color_string = "].color";
    string pos_string = "].pos";
    string intensity_string = "].intensity";
    for (int i = 0; i < firework_list.size(); i++)
    {
        if (firework_list[i].isExploded() && firework_list[i].getLightLife() > 0)
        {
            Firework* ptr = &firework_list[i];
            blinnphongshader.setVec3(struct_string + to_string(count) + color_string, ptr->getLightColor());
            blinnphongshader.setVec3(struct_string + to_string(count) + pos_string, ptr->getPosition());
            blinnphongshader.setFloat(struct_string + to_string(count) + intensity_string, ptr->getLightIntensity());
            count++;
        }
    }
    blinnphongshader.setInt("num_lights", count);
}