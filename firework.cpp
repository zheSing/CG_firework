#include "firework.h"
#include "generate_vertices.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <glm-master/glm/glm.hpp>
#include <glm-master/glm/gtc/matrix_transform.hpp>

const GLfloat Firework::GRAVITY = -0.25f;

Firework::Firework()
{
    initialise();
}

Firework::~Firework()
{
}

void Firework::initialise()
{
    // framesUntilLaunch = ((int)rand() % 2000);

    hasExploded = false;

    position[POSITION_NUMBER - 1] = glm::vec3(
        -0.1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.2))),//��ʼλ��x����������[-0.1,0.1]
        0.0f,//��ʼλ��y����
        -0.99f	//��ʼλ��z����
    );

    velocity = glm::vec3(
        -0.26 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.52))),//��ʼ�ٶ�x����������[-0.26,0.26]
        0.78 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.2))),	//��ʼ�ٶ�z����������[0.78,0.98]
        -0.26 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.52)))//��ʼ�ٶ�y����������[-0.26,0.26]
    );

    glm::vec3 xyz(
        0.4273033440113067627f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.639719843864440918f - 0.4273033440113067627f))),
        0.218611598014831543f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.7708090543746948242f - 0.218611598014831543f))),
        0.0975274890661239624f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.8590958118438720703f - 0.0975274890661239624f)))
    );

    color = glm::vec4(
        xyz2rgb(xyz),
        1.0f	//��ʼ��͸��
    );

    radius = 0.006f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.001f)));//��ʼ�뾶������[0.004,0.005]
    particleNum = 0;
    particleAliveNum = 0;

    std::cout << "Initialised a firework." << std::endl;
}

glm::vec3 Firework::getSpherePoint(GLfloat u, GLfloat v)
{
    GLfloat pi = glm::pi<GLfloat>();
    GLfloat z = std::cos(pi * u);
    GLfloat x = std::sin(pi * u) * std::cos(2 * pi * v);
    GLfloat y = std::sin(pi * u) * std::sin(2 * pi * v);
    return glm::vec3(x, y, z);
}


void Firework::velocitySample(GLuint Longitude, GLuint Latitude, std::vector<glm::vec3>& vertexes)
{
    // Longitude�������зָ���
    // Latitude��γ���зָ���
    GLfloat lon_step = 1.0f / Longitude;
    GLfloat lat_step = 1.0f / Latitude;
    for (size_t lat = 0; lat < Latitude; lat++) {  // γ��u
        for (size_t lon = 0; lon < Longitude; lon++) { // ����v
            glm::vec3 point = getSpherePoint(lat * lat_step, lon * lon_step);
            vertexes.push_back(point);
        }
    }
}

glm::vec3 Firework::velocitySampleRandom()
{
    return glm::normalize(glm::vec3(
        -0.1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.2))),
        -0.1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.2))),
        -0.1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.2)))
    ));
}

glm::vec3 Firework::rgb2xyz(glm::vec3 rgb)
{
    rgb.r > 0.04045f ? rgb.r = pow(((rgb.r + 0.055f) / 1.055f), 2.4f) : rgb.r /= 12.92f;
    rgb.g > 0.04045f ? rgb.g = pow(((rgb.g + 0.055f) / 1.055f), 2.4f) : rgb.g /= 12.92f;
    rgb.b > 0.04045f ? rgb.b = pow(((rgb.b + 0.055f) / 1.055f), 2.4f) : rgb.b /= 12.92f;

    glm::mat3 A(glm::vec3(0.412456439089692, 0.212672851405623, 0.019333895582329),
        glm::vec3(0.357576077643909, 0.715152155287818, 0.119192025881303),
        glm::vec3(0.180437483266399, 0.072174993306560, 0.950304078536368));

    return A * rgb;
}

glm::vec3 Firework::xyz2rgb(glm::vec3 xyz)
{
    glm::mat3 A(glm::vec3(3.240454162114104, -0.969266030505187, 0.055643430959115),
        glm::vec3(-1.537138512797716, 1.876010845446694, -0.204025913516754),
        glm::vec3(-0.498531409556016, 0.041556017530350, 1.057225188223179));

    glm::vec3 rgb = A * xyz;

    rgb.r > 0.0031308f ? rgb.r = 1.055f * pow(rgb.r, (1.0f / 2.4f)) - 0.055f : rgb.r *= 12.92f;
    rgb.g > 0.0031308f ? rgb.g = 1.055f * pow(rgb.g, (1.0f / 2.4f)) - 0.055f : rgb.g *= 12.92f;
    rgb.b > 0.0031308f ? rgb.b = 1.055f * pow(rgb.b, (1.0f / 2.4f)) - 0.055f : rgb.b *= 12.92f;

    return rgb;
}

void Firework::move(float dt)
{
    // if (framesUntilLaunch > 0)
    // {
    //     framesUntilLaunch--;
    //     return;
    // }

    if (this->isExploded())
    {

        this->explode(dt);
    }

    else
    {
        position_cnt = position_cnt + 1 < POSITION_NUMBER ? position_cnt + 1 : POSITION_NUMBER;
        for (int i = 0; i < POSITION_NUMBER - 1; i++)
        {
            this->position[i] = this->position[i + 1];
        }
        position[POSITION_NUMBER - 1] += (velocity * dt);

        velocity.y += (Firework::GRAVITY * dt);

        if (velocity.y + (Firework::GRAVITY * dt) <= 0.0f)
        {
            velocity.y = 0;
            hasExploded = true;//z����߶ȴﵽ����ը
            color.a = 0.0f;//�����ը������ȫ͸��
            particleNum = minParticleNum + (rand() % static_cast<int>(maxParticleNum - minParticleNum + 1));

            particleAliveNum = particleNum * particleNum;

            std::vector<glm::vec3> velSample;
            velocitySample(particleNum, particleNum, velSample);

            GLfloat explosion_speed = 0.10f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.02)));//�ٶȴ�С�˻�����������[0.08,0.10]

            for (int i = 0; i < particleNum * particleNum; i++)
            {
                particles[i].setPosition(this->getPosition());
                particles[i].setVelocity(velSample[i] * explosion_speed + velocity);//�����ٶȲ���, ʹ������ٶȲ�����ע�͵�����䲢ȡ��ע����һ�����
                //particles[i].setVelocity(velocitySampleRandom() * explosion_speed + velocity);//����ٶȲ���

                glm::vec3 xyz(
                    0.4273033440113067627f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.639719843864440918f - 0.4273033440113067627f))),
                    0.218611598014831543f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.7708090543746948242f - 0.218611598014831543f))),
                    0.0975274890661239624f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.8590958118438720703f - 0.0975274890661239624f)))
                );

                particles[i].setColor(glm::vec4(
                    xyz2rgb(xyz),
                    1.0f	//��ʼ��͸��
                ));

                particles[i].setRadius(
                    0.002f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.001)))//��ʼ�뾶������[0.002,0.003]
                );
            }

            std::cout << "Boom!" << std::endl;
        }
    }
}

void Firework::explode(float dt)
{
    for (int i = 0; i < particleNum * particleNum; i++)
    {
        if (particles[i].getColor().a > 0.0f)
        {
            particles[i].setPosition(particles[i].getPosition() + (particles[i].getVelocity() * dt));
            particles[i].setVelocity(particles[i].getVelocity() + glm::vec3(0.0f, Particle::GRAVITY * dt, 0.0f));

            float alphaFactor = 0.000005f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.8)));
            particles[i].setColor(particles[i].getColor() - glm::vec4(0.0f, 0.0f, 0.0f, alphaFactor * dt));
            if (particles[i].getColor().a <= 0.0f)
            {
                particleAliveNum--;
            }
        }
    }

    // if (particleAliveNum == 0)
    // {
    //     initialise();
    // }
}

// GLboolean Firework::isLaunched()
// {
//     return framesUntilLaunch <= 0;
// }

// void Firework::updateFramesUntilLaunch()
// {
//     framesUntilLaunch--;
// }

GLboolean Firework::isExploded()
{
    return hasExploded;
}

GLint Firework::getPositionCnt()
{
    return position_cnt;
}

glm::vec3 Firework::getPosition()
{
    return position[POSITION_NUMBER - 1];
}

glm::vec3* Firework::getPositionArr()
{
    return position;
}

glm::vec3 Firework::getVelocity()
{
    return velocity;
}

glm::vec4 Firework::getColor()
{
    return color;
}

GLfloat Firework::getRadius()
{
    return radius;
}


GLint Firework::getParticleNum()
{
    return particleNum * particleNum;
}


Particle* Firework::getParticles()
{
    return particles;
}

polygon Firework::getShape()
{
    return shape;
}

fireworktype Firework::getType()
{
    return type;
}

GLfloat Firework::getLightLife()
{
    return light_life;
}

GLfloat Firework::getLightIntensity()
{
    return light_intensity;
}


glm::vec3 Firework::getLightColor()
{
    return light_color;
}