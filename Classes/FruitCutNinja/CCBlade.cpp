/*
 * cocos2d+ext for iPhone
 *
 * Copyright (c) 2011 - Ngo Duc Hiep
 * Copyright (c) 2014 - ChildhoodAndy
 *
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

#include "CCBlade.h"
USING_NS_CC;

static const float DEFAULT_BLADE_WIDTH = 5.0f;
Blade* Blade::createWithMaximumPoint(int limit)
{
    Blade* blade = new Blade();
    if (blade && blade->initWithMaximumPoint(limit))
    {
        blade->autorelease();
        return blade;
    }
    
    CC_SAFE_DELETE(blade);
    return nullptr;
}

Blade::Blade()
: _bladeWidth(DEFAULT_BLADE_WIDTH)
, _texture(nullptr)
, _reset(false)
{
    
}

bool Blade::initWithMaximumPoint(int limit)
{
    if (!Node::init())
    {
        return false;
    }
    
    _pointLimit = limit;
    _vertices = (Point*)malloc(sizeof(Point) * (2 * _pointLimit + 5));
    _coordinates = (Point*)malloc(sizeof(Point) * (2 * _pointLimit + 5));
    
    PointSet(_coordinates + 0, 0.00f, 0.5f);
    
    setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));
    
    return true;
}

Blade::~Blade()
{
    CC_SAFE_RELEASE(_texture);
    CC_SAFE_FREE(_vertices);
    CC_SAFE_FREE(_coordinates);
    _path.clear();
}

void Blade::draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
{
    if ((_reset && _path.size() > 0) || (_autoDim && _willPop))
    {
        pop(1);
        if (_path.size() < 3)
        {
            clear();
        }
    }
    
    if (_path.size() < 3)
    {
        return;
    }
    
    _willPop = true;
    
    CCASSERT(_texture, "NO TEXTURE SET");
    CC_NODE_DRAW_SETUP();
    
    GL::blendFunc(CC_BLEND_SRC, CC_BLEND_DST);
    GL::bindTexture2D(_texture->getName());
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(_vertices[0]), _vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, sizeof(_coordinates[0]), _coordinates);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * _path.size() - 2);
}

void Blade::setBladeWidth(float bladeWidth)
{
    _bladeWidth = _bladeWidth * CC_CONTENT_SCALE_FACTOR();
}

const std::vector<Point>& Blade::getPath() const
{
    return _path;
}

void Blade::pop(int n)
{
    while (_path.size() > 0 && n > 0) {
        _path.pop_back();
        n--;
    }
    
    if (_path.size() > 2)
    {
        populateVertices();
    }
}

void Blade::populateVertices()
{
    _vertices[0] = _path.at(0);
    Point pre = _vertices[0];
    
    unsigned int i = 0;
    Point it = _path.at(1);
    float dd = _bladeWidth / _path.size();
    while (i < _path.size() - 2)
    {
        f1(pre, it, _bladeWidth - i * dd, _vertices + 2 * i + 1, _vertices + 2 * i + 2);
        PointSet(_coordinates + 2 * i + 1, 0.5f, 1.0f);
        PointSet(_coordinates + 2 * i + 2, 0.5f, 0.0f);
        
        i++;
        pre = it;
        
        it = _path.at(i + 1);
    }
    
    PointSet(_coordinates + 1, 0.25f, 1.0f);
    PointSet(_coordinates + 2, 0.25f, 1.0f);
    
    _vertices[2 * _path.size() - 3] = it;
    PointSet(_coordinates + 2 * _path.size() - 3, 0.75f, 0.5f);
}

#define DISTANCE_TO_INTERPOLATE 10

void Blade::push(Point point)
{
    _willPop = false;
    
    if (_reset)
    {
        return;
    }
    if (CC_CONTENT_SCALE_FACTOR() != 1.0f)
    {
        point = point * CC_CONTENT_SCALE_FACTOR();
    }

#if USE_LAGRANGE
    
    if (_path.size() == 0)
    {
        _path.push_back(point);
        return;
    }
    
    Point first = _path.at(0);
    if (point.getDistance(first) < DISTANCE_TO_INTERPOLATE)
    {
        _path.insert(_path.begin(), point);
        if (_path.size() > _pointLimit)
        {
            _path.pop_back();
        }
    }
    else
    {
        int num = point.getDistance(first) / DISTANCE_TO_INTERPOLATE;
        Point iv = (point - first) * (float)1.0f / (num + 1);
        for (int i = 1; i <= num + 1; i++)
        {
            _path.insert(_path.begin(), (first + iv * i));
        }
        while (_path.size() > _pointLimit)
        {
            _path.pop_back();
        }
    }
#else
    _path.insert(_path.begin(), point);
    if (_path.size() > _pointLimit)
    {
        _path.pop_back();
    }
#endif
    
    populateVertices();
}

void Blade::clear()
{
    _path.clear();
    _reset = false;
    if (_finish)
    {
        removeFromParentAndCleanup(true);
    }
}