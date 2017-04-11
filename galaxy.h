//
//  galaxy.hpp
//  glutCpp
//
//  Created by Ruben Cuadra on 3/24/17.
//  Copyright © 2017 Ruben Cuadra. All rights reserved.
//

#pragma once
#include "solarsystem.h"
#include <vector>

class Galaxy
{
protected:
    std::vector<SolarSystem> sistemas; //Deben ser maximo 8 por la cantidad de luces que permite opengl
public:
    Galaxy(); //Coordenadas
    // Manda a llamar a los sistemas que calculen sus posiciones
    void calculatePositions(float time);
    // Agrega sistemas al arreglo
    void addSystem(SolarSystem *system);

    void render();
    void renderOrbits();
    // Manda a llamar getPosition del Sistema en ese indice
    void getSystemPosition(int index, float* vec);
    SolarSystem * getSystem(int index);
    unsigned long getTotalSystems();
};

class RandomGalaxy : public Galaxy
{
public:
    RandomGalaxy(); 
};
