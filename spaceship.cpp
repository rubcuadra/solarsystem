//
//  spaceship.cpp
//  final_graficas
//
//  Created by Ruben Cuadra on 4/8/17.
//  Copyright © 2017 Ruben Cuadra. All rights reserved.
//

#include "spaceship.h"
#include <iostream>
#include <math.h>

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>
#else
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

#include "constants.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>



Spaceship::Spaceship(float *initialPos,float *finalPos)
{
    std::cout<<"\t\t("<<initialPos[0]<<","<<initialPos[1]<<","<<initialPos[2]<<")\n";
    std::cout<<"\t\t("<<finalPos[0]<<","<<finalPos[1]<<","<<finalPos[2]<<")\n";
    for (int i = 0; i<3; ++i)
    {
        offset[i] = initialPos[i];
        deltas[i] = finalPos[i]-initialPos[i];
    }
    forward = true;
    lookAt(finalPos); //Seteara los grados de rotacion
    calculatePosition(0);
}
Spaceship::Spaceship(float x1,float y1,float z1,float x2,float y2,float z2)
{
    std::cout<<"\t\t("<<x1<<","<<y1<<","<<z1<<")\n";
    std::cout<<"\t\t("<<x2<<","<<y2<<","<<z2<<")\n";
    offset[0] = x1;
    offset[1] = y1;
    offset[2] = z1;
    
    deltas[0] = x2-x1;
    deltas[1] = y2-y1;
    deltas[2] = z2-z1;
    
    forward = true;
    lookAt(x2,y2,z2);       //Seteara los grados de rotacion
    calculatePosition(0);   //Para que si se manda a llamar render sin calculate no truene
}
void Spaceship::lookAt(float x,float y,float z)  //TODO Fixear el Look
{
    look_rotation[0] = glm::degrees( atanf( (z-offset[2])/(y-offset[1]) )) ;
    look_rotation[1] = glm::degrees( atanf( (z-offset[2])/(x-offset[0]) )) ;
    look_rotation[2] = glm::degrees( atanf( (x-offset[0])/(y-offset[1]) ));
}
void Spaceship::lookAt(float *vec)
{
    look_rotation[0] = glm::degrees( atanf( (vec[2]-offset[2])/(vec[1]-offset[1]) )) ;
    look_rotation[1] = glm::degrees( atanf( (vec[2]-offset[2])/(vec[0]-offset[0]) )) ;
    look_rotation[2] = glm::degrees( atanf( (vec[0]-offset[0])/(vec[1]-offset[1]) ));
}

void Spaceship::calculatePosition(float t)
{
    float p = fmodf(t/50,2); // /50 para que se mueva mas lento
    forward = p<=1;         //Nos da la direccion, de 0 a 1 es de frente, 1.000..1 a 2 es hacia atras
    p = forward?p:1-fmodf(p,1); //Solo nos dejara los decimales el modulo, la resta nos dara la inversa
    for (int i = 0; i<3; ++i)
        current[i] = offset[i] + deltas[i]*p; //Ecuacion de recta en 3D
}

void Spaceship::renderTrajectory()
{
    glPushMatrix();
    
        glColor3f(1.0,1.0,1.0);  //Linea Blanca
        glBegin(GL_LINE_STRIP);
    
        for (float t = 0.0f; t < 1.01f; t += 0.05f) //0 a 1 usando la formula
            glVertex3f(offset[0]+deltas[0]*t,offset[1]+deltas[1]*t,offset[2]+deltas[2]*t);
        glEnd();
    
    glPopMatrix();
}

void Spaceship::render()
{
    glPushMatrix();
    
        glTranslatef(current[0],current[1],current[2] ); //Movernos a donde fue calculado
    
        //Como es un Cono de Origen sale hacia arriba, AL ser nave ajustar para que vean hacia la misma direccion, hacia nosotros
        glRotatef(90, 1, 0, 0);
    
        //Rotar para que apunte hacia el destino
        glRotatef(look_rotation[0], 1, 0, 0);
        glRotatef(look_rotation[1], 0, 1, 0);
        glRotatef(look_rotation[2], 0, 0, 1);
    
        if (forward) glScalef(-1, -1, -1); //Switch para que parezca que la nave va defrente
    
        glutSolidCone(0.05, 0.1, 10, 10);
        
    
    glPopMatrix();
}

void Spaceship::getPosition(float* vec)
{
    vec[0] = current[0]; //*distanceScale
    vec[1] = current[1];
    vec[2] = current[2];
}

