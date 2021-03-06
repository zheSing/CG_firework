#include <glad/glad.h>
#include "firework.h"
#include "draw.h"
#include "camera.h"
#include "shader.h"
#include "blur.h"
#include "skybox.h"
#include "model.h"
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <time.h>
#include <irrklang/irrKlang.h>
using namespace std;

// 烟花列表，存储所有烟花
vector<Firework> firework_list;

// 摄像机参数
Camera camera(glm::vec3(0.0f, 130.0f, 110.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float NEAR = 0.1f;
float FAR = 400.0f;
bool firstMouse = true;

// 音频
irrklang::ISoundEngine* SoundEngine;

// 时间参数
float deltaTime = 0.1f;
float lastFrame = 0.0f;

// 烟花速度参数
float dt = 3.0f;

// 回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos);          // 鼠标移动
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);   // 鼠标缩放
void processInput(GLFWwindow* window);                                      // 按键

// 按键状态
bool PRESS[TYPE_NUM] = { 0 };                                               // 1~5的按键状态，当前是否被按下
bool MOUSEPRESS = false;                                                    // 鼠标左键状态
bool MOUSEABLE = false;                                                     // 鼠标状态

// 传递点光源给着色器
void set_point_light(Shader& blinnphongshader);

int main()
{
    // 设置随机数种子
    srand(unsigned int(time(NULL)));

    // 初始化glfw设置
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // 根据设备信息设置屏幕宽高
    GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(pMonitor);
    SCR_WIDTH = mode->width / 1.2;
    SCR_HEIGHT = mode->height / 1.2;

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FIREWORK", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
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

    // 加载着色器
    Shader ColorShader("shader/Color.vs", "shader/Color.fs");
    Shader BlurShader("shader/Result.vs", "shader/Blur.fs");
    Shader ResultShader("shader/Result.vs", "shader/Result.fs");
    Shader SkyboxShader("shader/Skybox.vs", "shader/Skybox.fs");
    Shader CastleShader("shader/Blinn_Phong.vs", "shader/Blinn_Phong.fs");

    // 开启深度测试
    glEnable(GL_DEPTH_TEST);

    // 辉光效果初始化
    Blur blur;

    // 加载烟花图元
    Draw draw;

    // 加载天空盒
    Skybox skybox;

    // 加载城堡模型
    Model castle("Castle/Castle OBJ2.obj");

    // 初始化音频设备
    SoundEngine = irrklang::createIrrKlangDevice();
    SoundEngine->play2D("sound/1.wav", GL_FALSE);
    SoundEngine->play2D("sound/2.wav", GL_FALSE);
    SoundEngine->stopAllSounds();

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 根据上一帧渲染时间渲染
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 接收输入
        processInput(window);

        // 背景颜色
        blur.bindFrameBuffer();

        // 设置烟花着色器
        ColorShader.use();
        // 视角变换、投影变换
        // 世界变换交给draw_firework函数
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, NEAR, FAR);
        ColorShader.setMat4("view", view);
        ColorShader.setMat4("projection", projection);
        // 渲染烟花，应用烟花引擎
        for (vector<Firework>::iterator firework_it = firework_list.begin(); firework_it != firework_list.end();)
        {
            draw.draw_firework(firework_it, ColorShader);
            firework_it->move(dt * deltaTime);
            // 判定是否爆炸及是否寿命结束
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

        // 设置城堡着色器
        CastleShader.use();
        // MVP变换
        CastleShader.setMat4("view", view);
        CastleShader.setMat4("projection", projection);
        glm::mat4 castleTransform = glm::mat4(1.0f);
        castleTransform = glm::scale(castleTransform, glm::vec3(5.0f));
        castleTransform = glm::rotate(castleTransform, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        CastleShader.setMat4("model", castleTransform);
        // 传递点光源给着色器
        set_point_light(CastleShader);
        // 渲染城堡模型
        castle.Draw(CastleShader);

        // 设置天空盒着色器
        SkyboxShader.use();
        SkyboxShader.setInt("skybox", 0);
        // 视角变换、投影变换
        SkyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
        SkyboxShader.setMat4("projection", projection);
        // 渲染天空盒
        skybox.Draw();

        // 后处理：辉光效果
        blur.blurTheFrame(BlurShader, ResultShader);

        // 显示渲染结果
        glfwSwapBuffers(window);

        // 检查时间，更新状态
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// 判断按键并执行相应动作
void processInput(GLFWwindow* window)
{
    // ESC 退出
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // 数字1开始 发射不同烟花类型
    for (int i = 0; i < TYPE_NUM; i++)
    {
        if (glfwGetKey(window, GLFW_KEY_1 + i) == GLFW_PRESS)
        {
            // 只有按键按下瞬间会发射烟花(松开->按下)
            if (!PRESS[i] && firework_list.size() < MAX_FIREWORK_NUMBER)
            {
                fireworktype type = fireworktype(i);
                Firework newfirework(type);
                firework_list.push_back(newfirework);
                SoundEngine->play2D("sound/1.wav", GL_FALSE);
            }
            PRESS[i] = true;
        }
        if (glfwGetKey(window, GLFW_KEY_1 + i) == GLFW_RELEASE)
            PRESS[i] = false;
    }
    // E Q 调整烟花速度
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        dt = (dt + 0.01f) > 4.0f ? 4.0f : (dt + 0.01f);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        dt = (dt - 0.01f) < 2.0f ? 2.0f : (dt - 0.01f);
    // 鼠标点击
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && MOUSEPRESS == false)
    {
        if (MOUSEABLE)
        {
            glfwSetCursorPosCallback(window, mouse_callback);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            MOUSEABLE = false;
        }
        else
        {
            firstMouse = true;
            glfwSetCursorPosCallback(window, NULL);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            MOUSEABLE = true;
        }
        MOUSEPRESS = true;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
        MOUSEPRESS = false;
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
    string color_string = "].Color";
    string pos_string = "].Position";
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