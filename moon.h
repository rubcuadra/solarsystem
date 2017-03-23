#pragma once
#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>
#else
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

class Moon
{
private:
	// Distancia desde el planeta
	float distanceFromPlanet;

	// Tiempo en dias terrestres en dar la orbtia
	float orbitTime;

	// Tiempo de rotacion
	float rotationTime;

	// Radio
	float radius;

	// Coordenadas
	float position[3];
	// Helper para rotacion
	float rotation;

public:
	// Distancias en km y tiempo en dias terrestres
	Moon(float distanceFromPlanet, float orbitTime, float rotationTime, float radius);

	
	void calculatePosition(float time);
	void render(void);
	void renderOrbit(void);
};

