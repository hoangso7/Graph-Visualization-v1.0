#ifndef _mainBoard_hpp_
#define _mainBoard_hpp_
#include "globalVariables.hpp"
using namespace sf;
using namespace std;

class mainBoard : public Sprite{
    Texture Board_Texture;
public:
    void init(Vector2f position, Vector2f scale,string FileName);
    void draw();
    bool IsClicked();
};
void mainBoard::init(Vector2f position, Vector2f scale,string FileName){
    this->setPosition(position);
    Board_Texture.loadFromFile(FileName);
    this->setTexture(Board_Texture);
    this->setScale(scale);
}
void mainBoard::draw(){
    window->draw(*this);
}
bool mainBoard::IsClicked(){  
    if(Mouse::isButtonPressed(Mouse::Left))      
        if(this->getGlobalBounds().contains(Mouse::getPosition(*window).x,Mouse::getPosition(*window).y)){
            return true;
        }
    return false;
}
#endif
    

