#ifndef _sfarrow_hpp_
#define _sfarrow_hpp_
#include "helpers.hpp"
#include <SFML/Graphics.hpp>
#include <math.h>
#define PI 3.14159265359
class sfLine : public sf::Drawable
{
    sf::Vertex vertices[4];
    sf::Vertex triangles[3];
    float arrowLength = 20;
    float thickness = LINE_THICKNESS;
    sf::Color color = LINE_COLOR;
    sf::Text weight;
    sf::RectangleShape weightBox;
public:
    sfLine(){}
    sfLine(sf::Vector2f point1, sf::Vector2f point2, sf::Color c, string w);
    void connect(sf::Vector2f point1, sf::Vector2f point2, sf::RenderTarget &target, sf::RenderStates states, sf::Color c);
    void drawCurvedLine(Vector2f point1, Vector2f point2, sf::Color c, string w);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void straightLine(Vector2f point1,Vector2f point2 ,Color c );
    void drawTriangle(Vector2f point1,Vector2f point2, Color c);
};

sfLine::sfLine(sf::Vector2f point1, sf::Vector2f point2, sf::Color c, string w){
    //Algorithm for line:
    color = c;
    sf::Vector2f direction = point2 - point1;
    sf::Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
    sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

    sf::Vector2f offset = (thickness/2.f)*unitPerpendicular;

    vertices[0].position = point1 + offset;
    vertices[1].position = point2 + offset;
    vertices[2].position = point2 - offset;
    vertices[3].position = point1 - offset;

    
    for (int i=0; i<4; ++i)
        vertices[i].color = color;

    Vector2f midPoint;
    midPoint.x = (point1.x+point2.x)/2.f;
    midPoint.y = (point1.y+point2.y)/2.f;
    weight.setCharacterSize(20);
    weight.setFillColor(Color::Red);
    weight.setFont(*programFont);
    weight.setString(w);
    weight.setPosition(midPoint);

    weightBox.setPosition(midPoint);
    weightBox.setFillColor(Color::White);
    weightBox.setOutlineThickness(2);
    weightBox.setOutlineColor(Color::Red);
    weightBox.setSize(Vector2f(25,25));

    point2 -= normalize(direction) * 20.0f;
    drawTriangle(point1,point2,c);
}

void sfLine::connect(sf::Vector2f point1, sf::Vector2f point2, sf::RenderTarget &target, sf::RenderStates states, sf::Color c){
    //thickness = 5.f;
    color = c;
    //Algorithm for line:
    sf::Vector2f direction = point2 - point1;
    sf::Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
    sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

    sf::Vector2f offset = (thickness/2.f)*unitPerpendicular;

    vertices[0].position = point1 + offset;
    vertices[1].position = point2 + offset;
    vertices[2].position = point2 - offset;
    vertices[3].position = point1 - offset;
    
    for (int i=0; i<4; ++i)
        vertices[i].color = color;
    target.draw(vertices,4,sf::Quads);
    Vector2f midPoint;
    midPoint.x = (point1.x+point2.x)/2.f;
    midPoint.y = (point1.y+point2.y)/2.f;
    weightBox.setPosition(midPoint);
    weightBox.setFillColor(Color::White);
    weightBox.setSize(Vector2f(25,25));

    window->draw(weightBox);
    drawTriangle(point1,point2,c);
}
void sfLine::straightLine(Vector2f point1, Vector2f point2, Color c){
    //Algorithm for line:
    sf::Vector2f direction = point2 - point1;
    sf::Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
    sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

    sf::Vector2f offset = (thickness/2.f)*unitPerpendicular;

    vertices[0].position = point1 + offset;
    vertices[1].position = point2 + offset;
    vertices[2].position = point2 - offset;
    vertices[3].position = point1 - offset;
    for (int i=0; i<4; ++i)
        vertices[i].color = c;
    window->draw(vertices,4,sf::Quads);
    
}
void sfLine::drawCurvedLine(Vector2f point1, Vector2f point2, sf::Color c, string w) 
{
    color = c;
    Vector2f center = (point1 + point2) / 2.0f;
    float offset_x = vector_length(point1-center) ;
    float offset_y = vector_length(point2-center) ;
    RectangleShape rectangle(Vector2f(3,3));
    Vector2f direction = point2 - point1;
    float rotate = atan2f(direction.y, direction.x) ;

    int i = 0;
    for (float angle = 0; angle < PI; angle += 0.01) {
        
        float x = offset_x * cos(angle + rotate);
        float y = offset_y * sin(angle + rotate);
        rectangle.setPosition(center.x + x, center.y + y);
        rectangle.setFillColor(color);
        window->draw(rectangle);
    }

    float x = offset_x * cos(1.57 + rotate);//Pi / 2
    float y = offset_y * sin(1.57 + rotate);
    Vector2f midPoint;
    midPoint.x = center.x+x;
    midPoint.y = center.y+y;
    weight.setCharacterSize(20);
    weight.setFillColor(Color::Red);
    weight.setFont(*programFont);
    weight.setString(w);
    weight.setPosition(midPoint);

    weightBox.setPosition(midPoint);
    weightBox.setFillColor(Color::White);
    weightBox.setOutlineThickness(2);
    weightBox.setOutlineColor(Color::Red);
    weightBox.setSize(Vector2f(25,25));

    window->draw(weightBox);
    window->draw(weight);

    float x1 = offset_x * cos(3 + rotate)+center.x;
    float y1 = offset_y * sin(3 + rotate)+center.y;
    float x2 = offset_x * cos(2.8 + rotate)+center.x;
    float y2 = offset_y * sin(2.8 + rotate)+center.y;
    drawTriangle(Vector2f(x2,y2),Vector2f(x1,y1),c);
}	
void sfLine::drawTriangle(Vector2f point1, Vector2f point2, Color c){
    Vector2f direction = point2 - point1;
    Vector2f tmp;
    float k = (arrowLength/(sqrt(direction.x*direction.x+direction.y*direction.y)))-1; // k : độ dài cạnh huyền 
    k = -k;
    tmp.x = point1.x + k*direction.x;
    tmp.y = point1.y + k*direction.y;

    Vector2f triangleHeight =  tmp - point2 ;
    
    int deg = 45;

    triangles[0] = point2 ;
    triangles[1].position.x = triangleHeight.x*cos(deg)-triangleHeight.y*sin(deg)+point2.x;
    triangles[1].position.y = triangleHeight.x*sin(deg)+triangleHeight.y*cos(deg)+point2.y;
    triangles[2].position.x = triangleHeight.x*cos(-deg)-triangleHeight.y*sin(-deg)+point2.x;
    triangles[2].position.y = triangleHeight.x*sin(-deg)+triangleHeight.y*cos(-deg)+point2.y;
    
    for (int i=0; i<3; ++i)
        triangles[i].color = color;
    window->draw(triangles,3, sf::Triangles);
}
void sfLine::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(vertices,4,sf::Quads);
    target.draw(triangles,3, sf::Triangles);
    target.draw(weightBox);
    target.draw(weight);
}

#endif