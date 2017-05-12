//
//  texturizer.cpp
//  final_graficas
//
//  Created by Ruben Cuadra on 5/11/17.
//  Copyright © 2017 Ruben Cuadra. All rights reserved.
//

#include "texturizer.h"
#include <png.h>
#include <stdlib.h>
#include <string.h> 
#include <iostream>

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>
#else
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

Texturizer::Texturizer(char *name, int &outWidth, int &outHeight, bool &outHasAlpha)
{
    
    if ( !loadPngImage(name,outWidth,outHeight,outHasAlpha, &textureHandler) )
    {
        std::cout << "Unable to load png file" << std::endl ;
        exit(0);
    }
}

bool Texturizer::loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData)
{
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;
    FILE *fp;
    
    if ((fp = fopen(name, "rb")) == NULL)
        return false;
    
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                     NULL, NULL, NULL);
    
    if (png_ptr == NULL) {
        fclose(fp);
        return false;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }
    
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        /* Free all of the memory associated
         * with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        /* If we get here, we had a
         * problem reading the file */
        return false;
    }
    
    /* Set up the output control if
     * you are using standard C streams */
    png_init_io(png_ptr, fp);
    
    /* If we have already
     * read some of the signature */
    png_set_sig_bytes(png_ptr, sig_read);

    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
    
    png_uint_32 width, height;
    int bit_depth;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, NULL, NULL);
    outWidth = width;
    outHeight = height;
    
    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    *outData = (unsigned char*) malloc(row_bytes * outHeight);
    
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
    
    for (int i = 0; i < outHeight; i++)
    {
        memcpy(*outData+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
    }
    
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    fclose(fp);

    return true;
}
