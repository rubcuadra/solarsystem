#include "solarsystem.h"
#include "rand.h"
#include <iostream>

SolarSystem::SolarSystem(GLfloat *coords):pos(coords){}
SolarSystem::SolarSystem(float x,float y,float z)
{
    pos = new GLfloat[4];
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    pos[3] = 1.0; //Para la luz
}

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
    glPushMatrix();
        glTranslatef( pos[0], pos[1], pos[2] );
        for (int i = 0; i < planets.size(); i++)
        {
            planets[i].renderOrbit();
        }
    glPopMatrix();
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
void SolarSystem::addPlanet(Planet p)
{
    planets.push_back(p);
}
RandomSolarSystem::RandomSolarSystem(float x,float y,float z) : SolarSystem(x,y,z)
{
    int totalPlanets = Rand::randI(1,10); //De 1 a 10 planetas por sistema
    std::cout<<"\tTotal Planets "<<totalPlanets;
    std::cout<< "("<<x<<","<<y<<","<<z<<")\n";
    
    //Debemos crear 1..10 planetas Random, la cosa aqui es que la distancia del sol se debe ver influenciada por el radio y distancia del anterior + radio del nuevo
    
    addPlanet(0, 1, Rand::randI(1,1000), Rand::randI(695500,1000000), Rand::randColor(), Rand::randColor(), 0);
    
    long offset_from_sun = Rand::randL(57910000,83263838); //Initial
    long planet_size;
    
    for (int i = 1; i<totalPlanets; ++i) //El 0 sera el Sol
    {
        planet_size = Rand::randI(80,100000);
        std::cout<<"\t\tPlaneta "<<i<<" "<<offset_from_sun<<" "<<planet_size<<"r\n";
        addPlanet( RandomPlanet(offset_from_sun, planet_size));
        offset_from_sun += Rand::randL(planet_size+57910000,planet_size*5+83263838+220960489*2);
    }
}

