#include "Firework.h"
#include <iostream>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


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
	framesUntilLaunch = ((int)rand() % 2000);

	hasExploded = false;

	position = glm::vec3(
		-0.1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.2))),//初始位置x坐标限制在[-0.1,0.1]
		0.0f,//初始位置y坐标
		-0.99f	//初始位置z坐标
	);

	velocity = glm::vec3(
		-0.26 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.52))),//初始速度x分量限制在[-0.26,0.26]
		-0.26 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.52))),//初始速度y分量限制在[-0.26,0.26]
		0.78 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.2)))	//初始速度z分量限制在[0.78,0.98]
	);

	glm::vec3 xyz(
		0.4273033440113067627f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.639719843864440918f - 0.4273033440113067627f))),
		0.218611598014831543f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.7708090543746948242f - 0.218611598014831543f))),
		0.0975274890661239624f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.8590958118438720703f - 0.0975274890661239624f)))
	);

	color = glm::vec4(
		xyz2rgb(xyz),
		1.0f	//初始不透明
	);

	radius = 0.006f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.001f)));//初始半径限制在[0.004,0.005]
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


void Firework::velocitySample(GLuint Longitude, GLuint Latitude, std::vector<glm::vec3> &vertexes)
{
	// Longitude：经线切分个数
	// Latitude：纬线切分个数
	GLfloat lon_step = 1.0f / Longitude;
	GLfloat lat_step = 1.0f / Latitude;
	for (size_t lat = 0; lat < Latitude; lat++) {  // 纬线u
		for (size_t lon = 0; lon < Longitude; lon++) { // 经线v
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
	if (framesUntilLaunch > 0)
	{
		framesUntilLaunch--;
		return;
	}

	position += (velocity * dt);
	velocity.z += (Firework::GRAVITY * dt);

	if (velocity.z <= 0.0f)
	{
		velocity.z = 0;
		hasExploded = true;//z方向高度达到最大后爆炸
		color.a = 0.0f;//火箭爆炸后变成完全透明
		particleNum = minParticleNum + (rand() % static_cast<int>(maxParticleNum - minParticleNum + 1));

		particleAliveNum = particleNum * particleNum;

		std::vector<glm::vec3> velSample;
		velocitySample(particleNum, particleNum, velSample);

		GLfloat explosion_speed = 0.10f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.02)));//速度大小乘积因子限制在[0.08,0.10]

		for (int i = 0; i < particleNum * particleNum; i++)
		{
			particles[i].setPosition(position);
			particles[i].setVelocity(velSample[i] * explosion_speed + velocity);//均匀速度采样, 使用随机速度采样请注释掉本语句并取消注释下一条语句
			//particles[i].setVelocity(velocitySampleRandom() * explosion_speed + velocity);//随机速度采样

			glm::vec3 xyz(
				0.4273033440113067627f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.639719843864440918f - 0.4273033440113067627f))),
				0.218611598014831543f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.7708090543746948242f - 0.218611598014831543f))),
				0.0975274890661239624f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.8590958118438720703f - 0.0975274890661239624f)))
			);

			particles[i].setColor(glm::vec4(
				xyz2rgb(xyz),
				1.0f	//初始不透明
			));

			particles[i].setRadius(
				0.002f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.001)))//初始半径限制在[0.002,0.003]
			);
		}

		std::cout << "Boom!" << std::endl;
	}
}

void Firework::explode(float dt)
{
	for (int i = 0; i < particleNum * particleNum; i++)
	{
		if (particles[i].getColor().a > 0.0f)
		{
			particles[i].setPosition(particles[i].getPosition() + (particles[i].getVelocity() * dt));
			particles[i].setVelocity(particles[i].getVelocity() + glm::vec3(0.0f, 0.0f, Particle::GRAVITY * dt));

			float alphaFactor = 0.000005f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.8)));
			particles[i].setColor(particles[i].getColor() - glm::vec4(0.0f, 0.0f, 0.0f, alphaFactor * dt));
			if (particles[i].getColor().a <= 0.0f)
			{
				particleAliveNum--;
			}
		}
	}

	if (particleAliveNum == 0)
	{
		initialise();
	}
}

GLboolean Firework::isLaunched()
{
	return framesUntilLaunch <= 0;
}

void Firework::updateFramesUntilLaunch()
{
	framesUntilLaunch--;
}

GLboolean Firework::isExploded()
{
	return hasExploded;
}

glm::vec3 Firework::getPosition()
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