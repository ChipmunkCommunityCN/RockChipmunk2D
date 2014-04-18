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

#import "cocos2d.h"

#define USE_LAGRANGE    1
#define USE_STL_LIST    0
const float EPSILON = 0.00001;

NS_CC_BEGIN
class Blade : public Node
{
public:
    static Blade* createWithMaximumPoint(int limit);
    bool initWithMaximumPoint(int limit);
    Blade();
    virtual ~Blade();
    
    inline float fangle(Point point)
    {
        if (point.x <= EPSILON && point.x >= -EPSILON && point.y <= EPSILON && point.y >= -EPSILON)
        {
            return 0;
        }
        
        if (point.x <= EPSILON && point.x >= -EPSILON)
        {
            return point.y > 0 ? M_PI/2 : -M_PI/2;
        }
        
        if (point.y <= EPSILON && point.y >= -EPSILON && point.x < 0)
        {
            return -M_PI;
        }
        
        float angle = atan(point.y / point.x);
        
        return point.x < 0 ? angle + M_PI : angle;
    }
    
    inline void f1(Point p1, Point p2, float d, Point *o1, Point *o2)
    {
        float l = p1.getDistance(p2);
        float angle = fangle(p2 - p1);
        *o1 = (p1 + Point(l, d)).rotateByAngle(p1, angle);
        *o2 = (p1 + Point(l, -d)).rotateByAngle(p1, angle);
    }
               
    inline float lagrange1(Point p1, Point p2, float x)
    {
       return (x-p1.x)/(p2.x - p1.x)*p2.y + (x-p2.x)/(p1.x - p2.x)*p1.y ;
    }
   
    inline void PointSet(Point *v, float x, float y)
    {
       v->x = x;
       v->y = y;
    }
    
    inline void reset() { _reset = true; };
    inline void setDim(bool dim) { _reset = dim; };
    inline void finish() { _finish = true; }
    
    const std::vector<Point>& getPath() const;
    void push(Point point);
    void pop(int n);
    CC_SYNTHESIZE(bool, _autoDim, AutoDim);
    CC_SYNTHESIZE_RETAIN(Texture2D*, _texture, Texture);
    CC_SYNTHESIZE_READONLY(unsigned int, _pointLimit, PointLimit);
    virtual void draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated);
private:
    std::vector<Point> _path;
    Point* _vertices;
	Point* _coordinates;
    bool _reset;
	int _count;
    bool _finish;
    bool _willPop;
    float _bladeWidth;
    
    void setBladeWidth(float bladeWidth);
    void populateVertices();
    void clear();
};
NS_CC_END