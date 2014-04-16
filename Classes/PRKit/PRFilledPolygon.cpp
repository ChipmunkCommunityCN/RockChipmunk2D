/*
 PRFilledPolygon.m
 
 PRKit:  Precognitive Research additions to Cocos2D.  http://cocos2d-iphone.org
 Contact us if you like it:  http://precognitiveresearch.com
 
 Created by Andy Sinesio on 6/25/10.
 Copyright 2011 Precognitive Research, LLC. All rights reserved.
 
 This class fills a polygon as described by an array of NSValue-encapsulated points with a texture.
 
 Translated in C++ for Cocos2d-X by Damiano Mazzella on 19/03/2012
 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "PRFilledPolygon.h"
#include "PRRatcliffTriangulator.h"

#define __GLES2 1

PRFilledPolygon* PRFilledPolygon::filledPolygonWithPointsAndTexture(Vector2dVector &polygonPoints, Texture2D * fillTexture) {
    PRFilledPolygon *pRet = new PRFilledPolygon();
    if(pRet && pRet->initWithPointsAndTexture(polygonPoints, fillTexture))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}


PRFilledPolygon* PRFilledPolygon::filledPolygonWithPointsAndTextureUsingTriangulator(Vector2dVector &polygonPoints, Texture2D *fillTexture, PRRatcliffTriangulator *polygonTriangulator) {
    
    PRFilledPolygon *pRet = new PRFilledPolygon();
    if(pRet && pRet->initWithPointsandTextureusingTriangulator(polygonPoints, fillTexture,polygonTriangulator))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool PRFilledPolygon::initWithPointsAndTexture(Vector2dVector &polygonPoints, Texture2D * fillTexture) {
    PRRatcliffTriangulator *polygonTriangulator = new PRRatcliffTriangulator();
    return initWithPointsandTextureusingTriangulator(polygonPoints, fillTexture, polygonTriangulator);;
}


bool PRFilledPolygon::initWithPointsandTextureusingTriangulator(Vector2dVector &polygonPoints, Texture2D *fillTexture, PRRatcliffTriangulator* polygonTriangulator) {
    
    triangulator = polygonTriangulator;
    
#ifdef __GLES2
    setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));
#endif
    
    setTexture(fillTexture);
    setPoints(polygonPoints);
    
	return true;
}

void PRFilledPolygon::setPoints(Vector2dVector &points) {
    CC_SAFE_FREE(areaTrianglePoints);
    CC_SAFE_FREE(textureCoordinates);
    
    Vector2dVector triangulatedPoints = PRRatcliffTriangulator::triangulateVertices(points);
    
    areaTrianglePointCount = triangulatedPoints.size();
    areaTrianglePoints = (Point*) malloc(sizeof(Point) * areaTrianglePointCount);
    textureCoordinates = (Point*) malloc(sizeof(Point) * areaTrianglePointCount);
    
    for (int i = 0; i < areaTrianglePointCount; i++) {
        Vector2d v = (Vector2d)triangulatedPoints.at(i);
        areaTrianglePoints[i] = Point(v.GetX(), v.GetY());
    }
    
    calculateTextureCoordinates();
}

void PRFilledPolygon::cleanup()
{
    CC_SAFE_RELEASE_NULL(texture);
	CC_SAFE_FREE(triangulator);
    CC_SAFE_FREE(areaTrianglePoints);
    CC_SAFE_FREE(textureCoordinates);
    CCNode::cleanup();
}

void PRFilledPolygon::calculateTextureCoordinates() {
    for (int j = 0; j < areaTrianglePointCount; j++) {
        textureCoordinates[j] = areaTrianglePoints[j] * 1.0f / texture->getPixelsWide() * CC_CONTENT_SCALE_FACTOR();
        textureCoordinates[j].y = 1.0f - textureCoordinates[j].y;
    }
}

void PRFilledPolygon::updateBlendFunc() {
    // it's possible to have an untextured sprite
    if( !texture || !texture->hasPremultipliedAlpha() ) {
        blendFunc.src = GL_SRC_ALPHA;
        blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
    } else {
        blendFunc.src = CC_BLEND_SRC;
        blendFunc.dst = CC_BLEND_DST;
    }
}

void PRFilledPolygon::setBlendFunc(BlendFunc blendFuncIn) {
    blendFunc = blendFuncIn;
}

void PRFilledPolygon::setTexture(Texture2D* texture2D) {
    CCAssert(texture2D, "NO TEXTURE SET");
    CC_SAFE_RELEASE(texture);
	
    texture = texture2D;
    CC_SAFE_RETAIN(texture);
    
    Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT };
	texture->setTexParameters(texParams);
    
    updateBlendFunc();
	calculateTextureCoordinates();
}

Texture2D* PRFilledPolygon::getTexture() {
    return texture;
}

void PRFilledPolygon::draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated)
{
    if(areaTrianglePointCount <= 1)
        return;
    
#ifdef __GLES2
    
    CC_NODE_DRAW_SETUP();
    
    GL::bindTexture2D(texture->getName());
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    GL::blendFunc(blendFunc.src, blendFunc.dst);
    
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORDS);
    
    if(sizeof(Point) == sizeof(Vertex2F)) {
        
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, areaTrianglePoints);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, textureCoordinates);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)areaTrianglePointCount);

    } else {
        
        Vertex2F* newAreaTrianglePoints = new Vertex2F[areaTrianglePointCount];
        Vertex2F* newTextureCoordinates = new Vertex2F[areaTrianglePointCount];
        
        if(newAreaTrianglePoints != NULL && newTextureCoordinates != NULL) {
           
            for( unsigned int i = 0; i < areaTrianglePointCount; i++) {
                newTextureCoordinates[i].x = textureCoordinates[i].x;
                newTextureCoordinates[i].y = textureCoordinates[i].y;
                
                newAreaTrianglePoints[i].x = areaTrianglePoints[i].x;
                newAreaTrianglePoints[i].y = areaTrianglePoints[i].y;
            }
            glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, newAreaTrianglePoints);
            glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, newTextureCoordinates);
            glDrawArrays(GL_TRIANGLES, 0, (GLsizei)areaTrianglePointCount);
            
            CC_SAFE_DELETE_ARRAY(newAreaTrianglePoints);
            CC_SAFE_DELETE_ARRAY(newTextureCoordinates);
        }
    }
    
    CC_INCREMENT_GL_DRAWS(1);
    
#else
    
    Node::draw();
    
    glDisableClientState(GL_COLOR_ARRAY);
    
    // we have a pointer to vertex points so enable client state
    glBindTexture(GL_TEXTURE_2D, texture->getName());
    
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glVertexPointer(2, GL_FLOAT, 0, areaTrianglePoints);
	glTexCoordPointer(2, GL_FLOAT, 0, textureCoordinates);
	
    glDrawArrays(GL_TRIANGLES, 0, areaTrianglePointCount);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    //Restore texture matrix and switch back to modelview matrix
    glEnableClientState(GL_COLOR_ARRAY);
#endif
}