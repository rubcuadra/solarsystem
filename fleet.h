//
//  fleet.hpp
//  final_graficas
//
//  Created by Ruben Cuadra on 4/10/17.
//  Copyright © 2017 Ruben Cuadra. All rights reserved.
//

#pragma once
#include <vector>
#include "spaceship.h"

class Fleet
{
protected:
    std::vector<Spaceship> ships;
    
public:
    Fleet();
    // Manda a llamar a las naves para que calculen sus posiciones
    void calculatePositions(float time);
    // Agrega naves al arreglo
    void addShip( float x1,float y1,float z1,float x2,float y2,float z2 );
    // Manda a llamar renders de cada nave
    void render();
    void renderTrajectories();
    // Manda a llamar getPosition de la nave en esa posicion
    void getShipPosition(int index, float* vec);
    // Total de planetas
    int getTotalShips();
};

class RandomFleet : public Fleet
{
public:
    RandomFleet();
};
