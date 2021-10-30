//
// Created by zdenek on 16.10.21.
//

#ifndef HELLO_ALL_CUBE_H
#define HELLO_ALL_CUBE_H


#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#endif
#include <cmath>


#define GLE printGLError(__FILE__, __LINE__);

class Cube {
protected:
    static GLfloat vertices[];
    static GLubyte indices[];
    GLuint simpleCubeProgram;
    GLuint vertexBuffer;
    GLuint indexBuffer;
    GLint vertexLocation;
    GLint projectionLocation;
    GLint modelViewLocation;
    float modelviewmatrix[16];
    float projectionMatrix[16];

    GLuint loadBMP(const char * imagepath);
    GLuint loadShader(GLenum shaderType, const char* shaderSource);
    GLuint createProgram(const char* vertexSource, const char * fragmentSource);
public:
    static void matrixIdentityFunction(float* matrix);
    static void matrixMultiply(float* destination, float* operand1, float* operand2);

    static void matrixRotateX(float* matrix, float angle);
    static void matrixRotateY(float *matrix, float angle);
    static void matrixRotateZ(float *matrix, float angle);
    static float matrixDegreesToRadians(float arg);

    void printGLError(const char* file, int line);

    void init();
    void move(float t, float mouseTX, float mouseTY);
    void display();
    void destroy();

protected:
    virtual void subInitBuf();
    virtual void subInitAttr();
    virtual void draw();

    virtual void subDestroy();
};

#endif //HELLO_ALL_CUBE_H
