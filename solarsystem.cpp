#include "solarsystem.h"

SolarSystem::SolarSystem(GLfloat *coords):pos(coords){}

void SolarSystem::calculatePositions(float time)
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].calculatePosition(time);
	}
}

void SolarSystem::addPlanet(float distanceFromSun, float orbitTime, float rotationTime, float radius,float r,float g,float b)
{
	planets.push_back(Planet(distanceFromSun, orbitTime, rotationTime, radius,r,g,b));
}


void SolarSystem::addMoon(int planetIndex, float distanceFromPlanet, float orbitTime, float rotationTime, float radius)
{
	planets[planetIndex].addMoon(distanceFromPlanet, orbitTime, rotationTime, radius);
}
void SolarSystem::addRing(int planetIndex, float inner_distanceFromPlanet, float rotationTime, float outer_distanceFromPlanet,float angle)
{
    planets[planetIndex].addRing(inner_distanceFromPlanet, rotationTime, outer_distanceFromPlanet,angle);
}

void SolarSystem::render()
{
    glPushMatrix();
        glTranslatef( pos[0], pos[1], pos[2] );
        for (int i = 0; i < planets.size(); i++)
        {
            planets[i].render();
        }
    glPopMatrix();
}
GLfloat * SolarSystem::getPosition()
{
    return pos;
}
void SolarSystem::renderOrbits()
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].renderOrbit();
	}
}

unsigned long SolarSystem::getTotalPlanets()
{
    return planets.size();
}

void SolarSystem::getPlanetPosition(int index, float* vec)
{
	planets[index].getPosition(vec);
    vec[0] += pos[0];
    vec[1] += pos[1];
    vec[2] += pos[2];
    
}

float SolarSystem::getRadiusOfPlanet(int index)
{
	return planets[index].getRadius();
}
