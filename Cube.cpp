//
// Created by zdenek
//
#include "Cube.h"
#include <stdio.h>
#include <SDL2/SDL_opengles2.h>

GLfloat Cube::vertices[] = {
    0.5, -0.5,  0.5 ,
    0.5, -0.5, -0.5 ,
    0.5,  0.5, -0.5 ,
    0.5,  0.5,  0.5 ,

    -0.5, -0.5,  0.5 ,
    -0.5, -0.5, -0.5 ,
    -0.5,  0.5, -0.5 ,
    -0.5,  0.5,  0.5 ,
};
GLubyte Cube::indices[] = {
    4, 0, 3,
    4, 3, 7,
    0, 1, 2,
    0, 2, 3,
    1, 5, 6,
    1, 6, 2,
    5, 4, 7,
    5, 7, 6,
    7, 3, 2,
    7, 2, 6,
    0, 5, 1,
    0, 4, 5,
};
void Cube::init()
{
    glEnable(GL_CULL_FACE);

    glGenBuffers(1, &vertexBuffer);
    GLE
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    GLE
    glBufferData(GL_ARRAY_BUFFER ,3*36+sizeof(GLfloat),vertices, GL_STATIC_DRAW);
    GLE
    glGenBuffers(1, &indexBuffer);
    GLE
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    GLE
    glBufferData(GL_ELEMENT_ARRAY_BUFFER ,1*36*sizeof(GLushort),indices, GL_STATIC_DRAW);
    GLE

    subInitBuf();

    glUseProgram(simpleCubeProgram);
    GLE
    vertexLocation = glGetAttribLocation(simpleCubeProgram, "vertexPosition");
    GLE
    projectionLocation = glGetUniformLocation(simpleCubeProgram, "projection");
    GLE
    modelViewLocation = glGetUniformLocation(simpleCubeProgram, "modelView");
    GLE

    subInitAttr();

}

void Cube::display()
{
    //glClearColor(0x00 / 256.0, 0x1C / 256.0, 0x47 / 256.0, 1.0);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    GLE
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    GLE
    draw();
    glFlush();
    GLE
}

void Cube::destroy()
{
    subDestroy();
    glDeleteBuffers(1, &vertexBuffer);
    GLE
    glDeleteProgram(simpleCubeProgram);
    GLE
    glDisable(GL_CULL_FACE);
    GLE
}

void Cube::subDestroy()
{

}

void Cube::subInitBuf()
{

}
void Cube::subInitAttr()
{

}
void Cube::draw()
{
}

GLuint Cube::loadBMP(const char * imagepath)
{
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3
    // Actual RGB data
    unsigned char * data;

    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file){printf("Image could not be opened\n"); return 0;}
    if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
        return -1;
    }
    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        return -1;
    }
    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);
    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way
    // Create a buffer
    data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);

    unsigned char* ptr = data;
    for(int i=0;i<imageSize;i+=3) {
        unsigned char red = ptr[i+2];
        ptr[i+2] = ptr[i+0];
        ptr[i+0] = red;
    }

    //Everything is in memory now, the file can be closed
    fclose(file);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    GLE

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLE

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    GLE

    return textureID;
}

GLuint Cube::loadShader(GLenum shaderType, const char* shaderSource)
{
    GLuint shader = glCreateShader(shaderType);
    if (shader)
    {
        glShaderSource(shader, 1, &shaderSource, NULL);
        GLE
        glCompileShader(shader);
        GLE
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        GLE
        if (!compiled)
        {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            GLE
            if (infoLen)
            {
                char * buf = (char*) malloc(infoLen);
                if (buf)
                {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    GLE
                    printf("Could not Compile Shader %d:\n%s\n", shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                GLE
                shader = 0;
            }
        }
    }
    return shader;
}

void Cube::matrixIdentityFunction(float* matrix)
{
    if(matrix == NULL)
    {
        return;
    }
    matrix[0] = 1.0f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = 1.0f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;
    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = 1.0f;
    matrix[11] = 0.0f;
    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
}

void Cube::matrixMultiply(float* destination, float* operand1, float* operand2)
{
    float theResult[16];
    int row, column = 0;
    int i,j = 0;
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            theResult[4 * i + j] = operand1[j] * operand2[4 * i] + operand1[4 + j] * operand2[4 * i + 1] +
                                   operand1[8 + j] * operand2[4 * i + 2] + operand1[12 + j] * operand2[4 * i + 3];
        }
    }
    for(int i = 0; i < 16; i++)
    {
        destination[i] = theResult[i];
    }
}

float Cube::matrixDegreesToRadians(float arg)
{
    return arg*3.14/360.0;
}

void Cube::matrixRotateX(float* matrix, float angle)
{
    float tempMatrix[16];
    matrixIdentityFunction(tempMatrix);
    tempMatrix[5] = cos(matrixDegreesToRadians(angle));
    tempMatrix[9] = -sin(matrixDegreesToRadians(angle));
    tempMatrix[6] = sin(matrixDegreesToRadians(angle));
    tempMatrix[10] = cos(matrixDegreesToRadians(angle));
    matrixMultiply(matrix, tempMatrix, matrix);
}

void Cube::matrixRotateY(float *matrix, float angle)
{
    float tempMatrix[16];
    matrixIdentityFunction(tempMatrix);
    tempMatrix[0] = cos(matrixDegreesToRadians(angle));
    tempMatrix[8] = sin(matrixDegreesToRadians(angle));
    tempMatrix[2] = -sin(matrixDegreesToRadians(angle));
    tempMatrix[10] = cos(matrixDegreesToRadians(angle));
    matrixMultiply(matrix, tempMatrix, matrix);
}

void Cube::matrixRotateZ(float *matrix, float angle)
{
    float tempMatrix[16];
    matrixIdentityFunction(tempMatrix);
    tempMatrix[0] = cos(matrixDegreesToRadians(angle));
    tempMatrix[4] = -sin(matrixDegreesToRadians(angle));
    tempMatrix[1] = sin(matrixDegreesToRadians(angle));
    tempMatrix[5] = cos(matrixDegreesToRadians(angle));
    matrixMultiply(matrix, tempMatrix, matrix);
}

GLuint Cube::createProgram(const char* vertexSource, const char * fragmentSource)
{
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    if (!vertexShader)
    {
        return 0;
    }
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!fragmentShader)
    {
        return 0;
    }
    GLuint program = glCreateProgram();
    if (program)
    {
        glAttachShader(program , vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program , GL_LINK_STATUS, &linkStatus);
        if( linkStatus != GL_TRUE)
        {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength)
            {
                char* buf = (char*) malloc(bufLength);
                if (buf)
                {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    printf("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

void Cube::printGLError(const char* file, int line)
{
    const char* errString = "";
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        char buff[64];
        switch(err)
        {
            case GL_INVALID_ENUM: errString = "GL_INVALID_ENUM - An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n";break;
            case GL_INVALID_VALUE: errString = "GL_INVALID_VALUE - A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n";break;
            case GL_INVALID_OPERATION: errString = "GL_INVALID_OPERATION - The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n}";break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: errString = "GL_INVALID_FRAMEBUFFER_OPERATION - The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n";break;
            case GL_OUT_OF_MEMORY: errString = "GL_OUT_OF_MEMORY - There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n}";break;
            case GL_STACK_UNDERFLOW: errString = "GL_STACK_UNDERFLOW - An attempt has been made to perform an operation that would cause an internal stack to underflow.\n";break;
            case GL_STACK_OVERFLOW: errString = "GL_STACK_OVERFLOW - An attempt has been made to perform an operation that would cause an internal stack to overflow.\n}";break;
            default:sprintf(buff,"Unknown code %i\n}", err);errString = buff;
        }
        printf("GL error %s:%i: %s",file,line, errString);
    }
}

void Cube::move(float t, float mouseTX, float mouseTY)
{
    matrixIdentityFunction(modelviewmatrix);
    matrixIdentityFunction(projectionMatrix);
    matrixRotateX(modelviewmatrix, t-mouseTY);
    matrixRotateY(modelviewmatrix, t+mouseTX);
}