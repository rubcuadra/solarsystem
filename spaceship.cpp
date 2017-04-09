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

Spaceship::Spaceship(float *initialPos,float *finalPos)
{
    for (int i = 0; i<3; ++i)
    {
        offset[i] = initialPos[i];
        finals[i] = finalPos[i];
        deltas[i] = finalPos[i]-initialPos[i];
    }
    calculatePosition(0.5);
}
float Spaceship::getMagnitude(float *vec)
{
    return sqrtf(  powf(vec[0],2)+powf(vec[1],2)+powf(vec[2],2)  );
}
bool Spaceship::normalize(float *vec)
{
    float magnitude = getMagnitude(vec);
    if (magnitude!=0.0f)
    {
        vec[0] /= magnitude;
        vec[1] /= magnitude;
        vec[2] /= magnitude;
        return true;
    }
    std::cout<<"error al normalizar vector\n";
    return false;
}
float Spaceship::dotProduct(float *u,float*v)
{
    return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}

void Spaceship::lookAt(float *vec) //TODO : ARREGLAR ESTA MADRE
{
    float dotP = dotProduct( current , vec );
    std::cout<<dotP<<"\n";
    dotP = fmodf(dotP, 1);
    theta = acosf( dotP );

    theta *= 180/3.1459; //Pasamos a grados
    crossProduct(current, vec, look_rotation); //lookRotation tendra el axis
    
}

void Spaceship::calculatePosition(float t)
{
    float p = fmodf(t/10,2); // /10 para que se mueva mas lento
    bool forward = p<=1;
    p = forward?p:1-fmodf(p,1); //Solo nos dejara los decimales el modulo, la resta nos dara la inversa
    for (int i = 0; i<3; ++i) current[i] = offset[i] + deltas[i]*p; //Ecuacion de recta en 3D
    
    lookAt( forward?finals:offset ); //Se debe arreglar esto
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
        glRotatef(theta, look_rotation[0], look_rotation[1], look_rotation[2]);
        glutSolidCone(0.05, 0.1, 10, 10);
    
    glPopMatrix();
}

void Spaceship::crossProduct(float *u, float*v, float* r) //Si esta jalando, uXv y guarda en r
{
    r[0] = (u[1]*v[2]) - ( v[1]*u[2] );
    r[1] = (v[0]*u[2]) - ( u[0]*v[2] );
    r[2] = (u[0]*v[1]) - ( v[0]*u[1] );
}

void Spaceship::getPosition(float* vec)
{
    vec[0] = current[0]; //*distanceScale
    vec[1] = current[1];
    vec[2] = current[2];
}

