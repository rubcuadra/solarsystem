#include "moon.h"
#include <cmath>

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>
#else
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

#include "constants.h"

Moon::Moon(float distanceFromPlanet, float orbitTime, float rotationTime, float radius)
{
	this->distanceFromPlanet = distanceFromPlanet;
	this->orbitTime = orbitTime;
	this->rotationTime = rotationTime;
	this->radius = radius;
}

void Moon::calculatePosition(float time)
{
    //Igual que planetas
	float angle = time * 3.1419f / orbitTime;
	
	position[0] = sin(angle) * distanceFromPlanet;
	position[1] = cos(angle) * distanceFromPlanet;
	position[2] = 0;

	// Encontrar como esta rotando ahorita
	rotation = time * 360 / rotationTime;
}

void Moon::render(void)
{
	glPushMatrix();
        // Llevarla a la distancia desde el centro del planeta y despues que gire sobre si misma
        glTranslatef(position[0] * distanceScale, position[1] * distanceScale, position[2] * distanceScale);
        glRotatef(-rotation, 0.0f, 0.0f, 1.0f);
        //Cambiar la orbita modificaria la obtencion de calculo de position
        glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat[]) {0.1,0.1,0.1,1.0}); //Color del material
    
        glutSolidSphere(radius * planetSizeScale,30,30);
	glPopMatrix();
}

void Moon::renderOrbit(void)
{
	// Las orbitas son LINE STRIPs
	glBegin(GL_LINE_STRIP);

	// Orbita generada de iterar de 0 a 2PI y pintar puntos usando trigo
	for (float angle = 0.0f; angle < 6.283185307f; angle += 0.1f)
	{
		glVertex3f(sin(angle) * distanceFromPlanet * distanceScale, cos(angle) * distanceFromPlanet * distanceScale, 0.0f);
	}
	glVertex3f(0.0f, distanceFromPlanet * distanceScale, 0.0f);

	glEnd();
}
