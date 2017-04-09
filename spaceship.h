//
//  spaceship.hpp
//  final_graficas
//
//  Created by Ruben Cuadra on 4/8/17.
//  Copyright © 2017 Ruben Cuadra. All rights reserved.
//

#pragma once
class Spaceship
{
private:
    float offset[3]; //initial x,y,z
    float current[3];
    float deltas[3]; //Se calcula con la posicion final - inicial
    float finals[3];
    float look_rotation[3];
    float theta;
public:
    Spaceship(float *initialPos,float *finalPos);
    void calculatePosition(float t);
    void renderTrajectory();
    void getPosition(float *);
    void render();
    void lookAt(float *vec);
    static void crossProduct(float *v1, float*v2, float* r);
    static float getMagnitude(float *);
    static bool normalize(float *);
    static float dotProduct(float *,float *);
};
