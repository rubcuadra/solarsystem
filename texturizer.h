//
//  texturizer.hpp
//  final_graficas
//
//  Created by Ruben Cuadra on 5/11/17.
//  Copyright © 2017 Ruben Cuadra. All rights reserved.
//

#pragma once
#include <png.h>

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>
#else
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

class Texturizer
{
private:
    GLubyte * textureHandler;
    int width, height;
public :
    Texturizer(char *name, bool outHasAlpha);
    static bool loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData);
    int getWidth() {return width;}
    int getHeight(){return height;}
    GLubyte * getTextureHandler() { return textureHandler; }
};
