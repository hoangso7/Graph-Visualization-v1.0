#ifndef _vertex_hpp_
#define _vertex_hpp_
#include "globalVariables.hpp"
#include "constants.hpp"
#include "helpers.hpp"
using namespace std;
using namespace sf;

class vertex : public CircleShape{
    Text vertexName;
    Vector2f Pos;
    public:
        vertex();  /* : default constructor*/
        vertex(Vector2f Position, string name);
        void Draw();
        void Move(Vector2f);
        void update();
        void highlight(Color color);
        void highlightOutline(Color color);
        bool isClicked();
        void Rename(string newName);
        string getName();
};
vertex::vertex(Vector2f Position, string name){ 
    //Vertex's Name settings :   
    this -> setOrigin(RADIOUS,RADIOUS);
    this -> vertexName.setString(name);
    this -> vertexName.setCharacterSize(20);
    this -> vertexName.setFont(*programFont);
    this -> vertexName.setFillColor(Color::Black);
    this -> vertexName.setPosition(Position.x-vertexName.getGlobalBounds().width/2,Position.y-vertexName.getGlobalBounds().height);
    // Vertex's Circle settings :
    this -> setPosition(Position);
    this -> setRadius(RADIOUS);
    this -> setFillColor(Color(Color::White));
    this -> setOutlineColor(Color::Black);
    this -> setOutlineThickness(2);
}
void vertex::Draw(){
    window->draw(*this);
    window->draw(vertexName);
}
void vertex::Move(Vector2f target){
    setPosition(target);
    this -> vertexName.setPosition(getPosition().x-vertexName.getGlobalBounds().width/2,getPosition().y-vertexName.getGlobalBounds().height);
}
void vertex::update(){
    if(this -> getGlobalBounds().contains(Mouse::getPosition(*window).x, Mouse::getPosition(*window).y)){
        this -> setFillColor(VERTEX_CHOOSE);
        this -> vertexName.setFillColor(Color::Red);
        this -> setOutlineThickness(3.5);
        this -> setOutlineColor(VERTEX_OUTLINE_VISITED);
    }
    else{
        this->setFillColor(VERTEX_NORMAL);
        this -> vertexName.setFillColor(Color::Black);
        this -> setOutlineThickness(2);
        this -> setOutlineColor(Color::Black);
    }
}
void vertex::highlight(Color color){
    this->setFillColor(color);
}
void vertex::highlightOutline(Color color){
    this->setOutlineColor(color);
}
bool vertex::isClicked(){
    if(Mouse::isButtonPressed(Mouse::Left))
        if(this->getGlobalBounds().contains(Mouse::getPosition(*window).x,Mouse::getPosition(*window).y)){
            return true;
        }
    return false; 
}
void vertex::Rename(string newName){
    this -> vertexName.setString(newName);
    Pos = this->getPosition();
    this -> vertexName.setPosition(Pos.x-vertexName.getGlobalBounds().width/2,Pos.y-vertexName.getGlobalBounds().height);
    Draw();
}
string vertex::getName(){
    return this->vertexName.getString();
}
#endif
