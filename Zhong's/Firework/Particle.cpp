#include "Particle.h"

Particle::Particle()
{
}

Particle::~Particle()
{
}

glm::vec3 Particle::getPosition()
{
	return position;
}

glm::vec3 Particle::getVelocity()
{
	return velocity;
}

glm::vec4 Particle::getColor()
{
	return color;
}

GLfloat Particle::getRadius()
{
	return radius;
}


void Particle::setPosition(glm::vec3 position)
{
	this->position = position;
}

void Particle::setVelocity(glm::vec3 velocity)
{
	this->velocity = velocity;
}

void Particle::setColor(glm::vec4 color)
{
	this->color = color;
}

void Particle::setRadius(GLfloat radius)
{
	this->radius = radius;
}

const GLfloat Particle::GRAVITY = -0.01f;