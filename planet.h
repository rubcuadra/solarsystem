#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>
#else
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

#include <vector>
#include "moon.h"
#include "ring.h"

class Planet
{
private:
	float distanceFromSun;
	// Tiempo en orbitar
	float orbitTime;
	// Tiempo en dar 360 grados
	float rotationTime;
	// Radio del planeta
	float radius;
	// Coordenadas x,y,z
	float position[3];
	// Rotacion sobre su eje
	float rotation;
	std::vector<Moon> moons;
    std::vector<Ring> rings;

    float r,g,b; //Colores de material
    
public:
	// Distancia en km (kilometers) y tiempo en dias terrestres (365.25 orbit time for earth)
	Planet(float distanceFromSun, float orbitTime, float rotationTime, float radius,float r,float g,float b);
	void calculatePosition(float time); // Genera la posicion en 3d con base en el tiempo(Saber en que angulo se encuentra resp Sol)
	void render(void);
	void renderOrbit(void);
	void getPosition(float* vec); //Posicion en 3d
	float getRadius(void);

	// Agregar lunas y anillos
	void addMoon(float distanceFromPlanet, float orbitTime, float rotationTime, float radius);
    void addRing(float inner_distanceFromPlanet, float rotationTime, float outer_distanceFromPlanet,float angle);
};

