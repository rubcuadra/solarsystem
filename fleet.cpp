//
//  fleet.cpp
//  final_graficas
//
//  Created by Ruben Cuadra on 4/10/17.
//  Copyright © 2017 Ruben Cuadra. All rights reserved.
//

#include "fleet.h"
#include "spaceship.h"
#include "rand.h"
#include <iostream>

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>
#else
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

Fleet::Fleet(){}

void Fleet::calculatePositions(float t)
{
    for (int i = 0; i < ships.size(); i++)
    {
        ships[i].calculatePosition(t);
    }
}
void Fleet::addShip(float x1,float y1,float z1,float x2,float y2,float z2)
{
    ships.push_back( Spaceship( x1, y1, z1, x2, y2, z2 ) );
}

void Fleet::render()
{
    glPushMatrix();
        for (int i = 0; i < ships.size(); i++) ships[i].render();
    glPopMatrix();
}

void Fleet::renderTrajectories()
{
    glPushMatrix();
        for (int i = 0; i < ships.size(); i++) ships[i].renderTrajectory();
    glPopMatrix();
}

int Fleet::getTotalShips()
{
    return ships.size();
}

void Fleet::getShipPosition(int index, float* vec)
{
    ships[index].getPosition(vec); //Llenara vec con la pos
}

RandomFleet::RandomFleet() : Fleet()
{
    int numShips = Rand::randI(5, 10);
    std::cout<<"Total Ships "<<numShips<<"\n";
    for (int i = 0; i < numShips; ++i)
    {
        std::cout<<"\tShip "<<i<<"\n";
        addShip(Rand::randI(-1000,1000),
                Rand::randI(-1000,1000),
                Rand::randI(-1000,1000),
                Rand::randI(-1000,1000),
                Rand::randI(-1000,1000),
                Rand::randI(-1000,1000));
    }
    //test = new Spaceship(0,0,0,3,4,-2); //Con este jala bien...
}
