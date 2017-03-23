//
//  constants.h
//  glutCpp
//
//  Created by Ruben Cuadra on 2/10/17.
//  Copyright © 2017 Ruben Cuadra. All rights reserved.
//

#pragma once
//La escala de distancia, si un planeta esta a 149600000km del sol
//La escala de distancia es 0.00000001
//Se pintara el planeta a 1.486 unidades del Sol
const float distanceScale = 0.00000001f;
//La escala, si el radio del planeta es 6371km
//La variable planetSizeScale es 0.0001
//Se dibujaria una esfera de 0.6371 unidades
extern float planetSizeScale ;
extern float ringSizeFactor ; //To reduce Z component to turn it into ring instead of donut(Fue al tanteo)
//Diferentes escalas para que se pueda visualizar todo de manera correcta, si usaramos la misma escala distancia planetas no se verian o tardariamos mucho en mover la camara
