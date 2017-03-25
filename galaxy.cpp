//
//  galaxy.cpp
//  glutCpp
//
//  Created by Ruben Cuadra on 3/24/17.
//  Copyright © 2017 Ruben Cuadra. All rights reserved.
//

#include "galaxy.h"

Galaxy::Galaxy(){}

// Manda a llamar a los sistemas que calculen sus posiciones
void Galaxy::calculatePositions(float time)
{
    for (int i = 0; i < sistemas.size(); i++)
    {
        sistemas[i].calculatePositions(time);
    }
}

// Agrega sistemas al arreglo
void Galaxy::addSystem(SolarSystem *system)
{
    if (sistemas.size() < 8)
        sistemas.push_back( *system );
    else
        printf("No se agrego el sistema, ya son mas de 8(Debido a la cantidad de luces no se agrego)\n");
}

void Galaxy::render()
{
    for (int i = 0; i < sistemas.size(); i++)
    {
        sistemas[i].render();
    }
}

void Galaxy::renderOrbits()
{
    for (int i = 0; i < sistemas.size(); i++)
    {
        sistemas[i].renderOrbits();
    }
}

// Manda a llamar getPosition del Sistema en ese indice
void Galaxy::getSystemPosition(int index, float* vec)
{
    vec[0] = sistemas[index].getPosition()[0];
    vec[1] = sistemas[index].getPosition()[1];
    vec[2] = sistemas[index].getPosition()[2];
}
unsigned long Galaxy::getTotalSystems(){return sistemas.size();}

SolarSystem * Galaxy::getSystem(int index)
{
    return &sistemas[index];
}
