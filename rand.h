//
//  controlla.h
//  final_graficas
//
//  Created by Ruben Cuadra on 4/10/17.
//  Copyright © 2017 Ruben Cuadra. All rights reserved.
//

#pragma once
#include <random>

class Rand
{
public:
    static int randI(int nMin, int nMax)
    {
        return (int)(rand()%nMax)+nMin;//nMin + (int)((double)rand() / (RAND_MAX+1) * (nMax-nMin+1));
    }
    static unsigned long randL(long nMin, long nMax)
    {
        return (long)(rand()%nMax)+nMin;
    }
    static float randColor()
    {
        return ((double) rand() / (RAND_MAX));
    }
};
