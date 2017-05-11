//
//  spaceship.hpp
//  final_graficas
//
//  Created by Ruben Cuadra on 4/8/17.
//  Copyright © 2017 Ruben Cuadra. All rights reserved.
//

#pragma once
#include "loader/glm.h"
#include <cstdlib>

class Spaceship
{
private:
    float offset[3]; //initial x,y,z
    float current[3]; //Current Pos
    float deltas[3]; //Se calcula con la posicion final - inicial
    float look_rotation[3]; //Grados en x,y,z
    bool forward;
    
public:
    inline static GLMmodel *pmodel = NULL;	/* the loaded model */
    static void setModel(GLMmodel * model)
    {
        pmodel = model;
    }
    
    Spaceship(float *initialPos,float *finalPos);
    Spaceship(float x1,float y1,float z1,float x2,float y2,float z2);
    
    void calculatePosition(float t);
    void renderTrajectory();
    void getPosition(float *);
    void render();
    void lookAt(float *vec);
    void lookAt(float x,float y,float z);
};
