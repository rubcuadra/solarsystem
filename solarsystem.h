#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>
#else
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

#include <vector>

#include "planet.h"

class SolarSystem
{
private:
	std::vector<Planet> planets;
    GLfloat *pos; //Un array de 3 posiciones
public:
	SolarSystem(GLfloat *coords); //Coordenadas
    SolarSystem(float x,float y,float z); //Coordenadas
	// Manda a llamar a los planetas que calculen sus posiciones
	void calculatePositions(float time);
	// Agrega planetas al arreglo
	void addPlanet(float distanceFromSun, float orbitTime, float rotationTime, float radius,float r,float g,float b);
	// Agrega lunas/anillos al planeta en ese indice
	void addMoon(int planetIndex, float distanceFromPlanet, float orbitTime, float rotationTime, float radius);
    void addRing(int planetIndex, float inner_distanceFromPlanet, float rotationTime, float outer_distanceFromPlanet,float angle);
	// Manda a llamar renders de cada planeta
	void render();
	void renderOrbits();
	// Manda a llamar getPlanetPosition del planeta en ese indice
	void getPlanetPosition(int index, float* vec);
	// Radio del planeta en ese indice
	float getRadiusOfPlanet(int index);
    // Total de planetas
    unsigned long getTotalPlanets();
    GLfloat * getPosition();
};

class RandomSolarSystem  : public SolarSystem
{
public:
    RandomSolarSystem(float x,float y,float z); //Numero de planetas en este sistema
    
};


