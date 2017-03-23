#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>
#else
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

#ifdef _WIN32
    #include <windows.h>
#endif

class Ring
{
private:
    // Distancia del centro del planeta a la circunferencia mas pequena
    float inner_distanceFromPlanet;
    // time it takes to spin 360 degrees
    float rotationTime;
    // Distancia de inner circle al outer
    float outer_distanceFromPlanet;
    // its rotation around its axis
    float rotation;
    float angle_x; //Que tan inclinado respecto x
    
public:
    // Distancias en kilometros, rotation time en dias terrestres(Los dias que tarda en dar una rotacion)
    // angulo de inclinacion sobre el plano x
    Ring(float inner_distanceFromPlanet, float rotationTime, float outer_distanceFromPlanet,float angle);
    void calculatePosition(float time);
    
    // Pintarlo
    void render(void);
};

