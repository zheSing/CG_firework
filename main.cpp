#include "firework.h"
#include "draw.h"
#include "camera.h"
#include "shader.h"
#include "engine.h"
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm-master/glm/glm.hpp>
#include <glm-master/glm/gtc/matrix_transform.hpp>
#include <glm-master/glm/gtc/type_ptr.hpp>
#include <iostream>
using namespace std;

// 窗口大小
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 烟花列表
vector<firework> firework_list;

// 摄像机
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float NEAR = 0.1f;
float FAR = 100.0f;
bool firstMouse = true;

// 时间
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, Engine& engine);


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

    // 开启深度测试
    glEnable(GL_DEPTH_TEST);

    Shader ColorShader("Color.vs", "Color.fs");
    Shader BlinnPhongShader("Blinn_Phong.vs", "Blinn_Phong.fs");

    // 绑定基本图元
    Draw draw;

    // 引擎
    Engine engine(&firework_list);

    //TODO：加载固定模型

    // 渲染循环
    while (true)
    {
        // 更新时间
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 接收输入
        processInput(window, engine);

        // 设置普通shaer
        ColorShader.use();
        ColorShader.setVec3("viewPos", camera.Position);

        // 视角变换、投影变换
        // 世界变换交给draw_firework函数
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, NEAR, FAR);
        glm::mat4 view = camera.GetViewMatrix();
        ColorShader.setMat4("projection", projection);
        ColorShader.setMat4("view", view);

        // 渲染烟花系统
        for (int i = 0; i < firework_list.size(); i++)
        {
            draw.draw_firework(&firework_list[i], ColorShader);
            engine.engine_forward(&firework_list[i]);
        }

        //TODO：渲染固定模型
    }
    return 0;
}



// 判断按键并执行相应动作
void processInput(GLFWwindow* window, Engine& engine)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    // TODO：如果某按键按下，产生烟花
        // engine.create_firework_random();
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