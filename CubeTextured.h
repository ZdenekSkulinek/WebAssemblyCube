//
// Created by zdenek on 16.10.21.
//

#ifndef HELLO_ALL_CUBETEXTURED_H
#define HELLO_ALL_CUBETEXTURED_H

#include "Cube.h"

class CubeTextured: public Cube {
    static char  glVertexShader[];
    static char  glFragmentShader[];
    static GLfloat texcoords[];
    GLuint Texture;
    GLint vertexUVLocation;
    GLuint texCoordBuffer;

    void subDestroy();
    void subInitBuf();
    void subInitAttr();
    void draw();
};


#endif //HELLO_ALL_CUBETEXTURED_H
