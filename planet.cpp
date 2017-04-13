#include "planet.h"
#include <cmath>

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>
#else
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif
#include "constants.h"
#include "rand.h"

float planetSizeScale = 0.000005f;

Planet::Planet(float distanceFromSun, float orbitTime, float rotationTime, float radius,float red,float green,float blue)
:r(red),g(green),b(blue),distanceFromSun(distanceFromSun),orbitTime(orbitTime),rotationTime(rotationTime),radius(radius){}

//Generamos sus coordenadas segun el tiempo (En que parte de la orbita se encuentra,traslacion, y que parte de si mismo,rotacion)
void Planet::calculatePosition(float time)
{
	// Angulo de orientacion referente al sol
	float angle = time * 3.1419f / orbitTime;
	// Formula trigonometrica
	position[0] = sin(angle) * distanceFromSun;
	position[1] = cos(angle) * distanceFromSun;
	position[2] = 0;
	// Su rotacion
	rotation = time * 360 / rotationTime;

	// calculate positions of moons
	for (int i = 0; i < moons.size(); i++)
	{
		moons[i].calculatePosition(time);
	}
}

void Planet::render(void)
{
	glPushMatrix();
        //Nos movemos a donde debe ser pintado
        glTranslatef(position[0] * distanceScale, position[1] * distanceScale, position[2] * distanceScale);

        for(Moon m:moons) m.render(); //Iteramos lunas
        for (Ring r : rings) r.render();

        ///Rotacion del planeta
        glRotatef(rotation, 0.0f, 0.0f, 1.0f);
        glPushMatrix();
            if (distanceFromSun < 0.001f) //El sol esta limitado si no seria demasiado grande
            {
                float radiusScaled = radius*planetSizeScale;
                if (radiusScaled > 0.5f) radiusScaled = 0.5f;
                glDisable(GL_LIGHTING);     //Si no hacemos esto el sol se ve muy opaco, la luz esta dentro de el
                    glColor3f(r,g,b);
                    glutSolidSphere(radiusScaled,30,30);
                glEnable(GL_LIGHTING);
            }
            else
            {
                glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat[]) { 0.6, 0.6, 0.0, 1.0 }); //Reflejo amarillo
                glMaterialfv(GL_FRONT, GL_SHININESS, (GLfloat[]) {100.0});
                glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat[]) {r,g,b,1.0}); //Color del material
                glutSolidSphere(radius*planetSizeScale,30,30);
            }
        glPopMatrix();
	glPopMatrix();
}

// render this planets orbit circle
void Planet::renderOrbit(void)
{
    //Orbita de este planeta
    glColor3f(1.0,1.0,1.0);  //Orbitas blancas para todos
	glBegin(GL_LINE_STRIP); //Las orbitas son LINE_STRIPs
        //Las orbitas son con formula trigonometrica, de 0 a 2PI
        for (float angle = 0.0f; angle < 6.283185307f; angle += 0.05f)
            glVertex3f(sin(angle)*distanceFromSun*distanceScale,cos(angle)*distanceFromSun*distanceScale, 0.0f);
        glVertex3f(0.0f,distanceFromSun*distanceScale, 0.0f); //Punto de cierre
	glEnd();
    
    //Orbitas de sus respectivas lunas
	glPushMatrix();
        //Translate a las coordenadas del planeta (Nuevo centro)
        glTranslatef(position[0]*distanceScale,position[1]*distanceScale,position[2]*distanceScale);
    
    for(Moon m: moons) m.renderOrbit(); //Por cada luna renderear
    
	glPopMatrix();
}


//Coordenadas del planeta(las escalamos)
void Planet::getPosition(float* vec)
{
	vec[0] = position[0] * distanceScale;
	vec[1] = position[1] * distanceScale;
	vec[2] = position[2] * distanceScale;
}

float Planet::getRadius(void)
{
	return radius;
}

// Agregamos lunas
void Planet::addMoon(float distanceFromPlanet, float orbitTime, float rotationTime, float radius)
{
	moons.push_back(Moon(distanceFromPlanet, orbitTime, rotationTime, radius));
}
void Planet::addRing(float inner_distanceFromPlanet, float rotationTime, float outer_distanceFromPlanet,float angle)
{
    rings.push_back(Ring(inner_distanceFromPlanet,rotationTime,outer_distanceFromPlanet,angle));
}

RandomPlanet::RandomPlanet(float distanceFromSun,float radius) : Planet(distanceFromSun,Rand::randI(1, 100000),Rand::randI(1, 100000),radius,Rand::randColor(),Rand::randColor(),Rand::randColor())
{
    int moons = Rand::randI(0,6); //Maximo unas 6 lunas estara bien
    int distance = 7000000*radius/6371; //Initial
    
    for (int i = 0; i < moons; i++)
    {
        float rad = Rand::randI(30, radius/2);
        addMoon(distance, Rand::randI(-100, 100), 1, rad);
        distance+=rad*2;
    }
    //Agregarle anillos
}
