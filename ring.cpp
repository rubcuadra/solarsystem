#pragma once
#include "ring.h"
#include <cmath>

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>
#else
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif
#include "constants.h"

float ringSizeFactor = 0.1;
Ring::Ring(float inner_distanceFromPlanet, float rotationTime, float outer_distanceFromPlanet,float angle_x):
inner_distanceFromPlanet(inner_distanceFromPlanet),rotationTime(rotationTime),outer_distanceFromPlanet(outer_distanceFromPlanet),angle_x(angle_x){}

// Siempre esta en el centro del planeta, calculate solo lo hara girar
void Ring::calculatePosition(float time)
{
    rotation = time*360/rotationTime; //Girarlo
}

// Renderear
void Ring::render(void)
{
    glPushMatrix();
        // Esto lo manda a llamar un planeta, de modo que ya estamos en el centro de el
        glRotatef(-rotation, 0.0f, 0.0f, 1.0f);
        glRotatef(angle_x,1.0,0.0,0.0);
        glScalef (planetSizeScale,planetSizeScale,planetSizeScale*ringSizeFactor); //Tamano real, Z debe ser muy bajo para que sea anillo y no una dona
        //glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat[]) {0.5,0.1,0.5,1.0}); //Color del material
        glDisable(GL_LIGHTING);
            glColor3f(0.5, 0.1,1.0); //Deberiamos poder modificarlo pero neh
            glutSolidTorus(inner_distanceFromPlanet, outer_distanceFromPlanet, 30,30);
        glEnable(GL_LIGHTING);
    
    glPopMatrix();
}
