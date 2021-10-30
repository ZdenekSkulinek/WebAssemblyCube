//
// Created by zdenek on 16.10.21.
//

#include "CubeTextured.h"
#include <stdio.h>
#include <SDL2/SDL_opengles2.h>
#include <cassert>


void CubeTextured::subDestroy()
{
    glDeleteBuffers(1, &texCoordBuffer);
    GLE
}
void CubeTextured::subInitBuf()
{
    glGenBuffers(1, &texCoordBuffer);
    GLE
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    GLE
    glBufferData(GL_ARRAY_BUFFER ,2*36*sizeof(GLfloat), texcoords, GL_STATIC_DRAW);
    GLE
    Texture = loadBMP("Robot.bmp");
    assert(Texture != -1);
    GLE
    simpleCubeProgram = createProgram(glVertexShader, glFragmentShader);
    assert(simpleCubeProgram != -1);
}
void CubeTextured::subInitAttr()
{
    vertexUVLocation = glGetAttribLocation(simpleCubeProgram, "vertexUV");
    GLE
    assert(vertexUVLocation != -1);
}
void CubeTextured::draw()
{
    glBindTexture(GL_TEXTURE_2D, Texture);
    GLE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GLE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    GLE
    glEnableVertexAttribArray(vertexLocation);
    GLE
    glEnableVertexAttribArray(vertexUVLocation);
    GLE
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    GLE
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    GLE
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    GLE
    glVertexAttribPointer(vertexUVLocation, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    GLE
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix);
    GLE
    glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, modelviewmatrix);
    GLE
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
    GLE
    glDisableVertexAttribArray(vertexUVLocation);
    GLE
    glDisableVertexAttribArray(vertexLocation);
    GLE
}

char CubeTextured::glVertexShader[] =
    "attribute vec3 vertexPosition;\n"
    "attribute vec2 vertexUV;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 modelView;\n"
    "varying vec2 UV;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = projection * modelView * vec4(vertexPosition, 1.0);\n"
    "    UV = vertexUV;\n"
    "}\n";

char CubeTextured::glFragmentShader[] =
    "precision mediump float;\n"
    "varying vec2 UV;\n"
    "uniform sampler2D myTextureSampler;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = texture2D( myTextureSampler, UV );\n"
    "}\n";

GLfloat CubeTextured::texcoords[] = {
    0.0, 0.0,
    1.0,0.0,
    1.0,1.0,
    0.0,1.0,
    0.0, 0.0,
    1.0,0.0,
    1.0,1.0,
    0.0,1.0,
};

