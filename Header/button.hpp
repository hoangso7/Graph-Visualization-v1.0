#ifndef _button_hpp_
#define _button_hpp_
#include "constants.hpp"
#include "globalVariables.hpp"
#include <iostream>
using namespace sf;
using namespace std;

class menuButton : public Sprite
{
    Texture normal_texture;
    Texture hightlight_texture;
    RectangleShape AlgoButton; 
    Text AlgoName;  
    RectangleShape hintBox;
    Text Hint;
public:
    menuButton(){}
    //FOR TEXTURE BUTTONS
    void initialize(Vector2f,string,string,wstring);
    void draw();
    void keepButtonHighlighted();
    void reset();
    bool isLeftClicked();
    //FOR ALGORITHMS BOXES
    void initialize_2(Vector2f , wstring );
    void draw_2();
    void keepButtonHighlighted_2();
    void reset_2();
    bool isLeftClicked_2();
};
/*  Function Box : */
void menuButton::initialize(Vector2f position, string normalFileName, string highlightFileName, wstring hint){
    //buttons settings:
    this->setPosition(position);
    normal_texture.loadFromFile(normalFileName);
    hightlight_texture.loadFromFile(highlightFileName);
    this->setTexture(normal_texture);
    //Hint box settings:
    this->hintBox.setPosition(position.x+63.f,position.y+10.f);
    this->hintBox.setFillColor(HINTBOX_COLOR);
    this->hintBox.setSize(Vector2f(100,30));
    this->hintBox.setOutlineColor(HINTBOX_OUTLINE_COLOR);
    this->hintBox.setOutlineThickness(LINE_THICKNESS);
    //Hint text settings
    this->Hint.setString(hint);
    this->Hint.setFont(*programFont);
    this->Hint.setCharacterSize(18);
    this->Hint.setPosition(position.x+67.f,position.y+12.f);
    this->Hint.setFillColor(HINT_WORD_COLOR);
}

void menuButton::draw(){
    window->draw(*this);
}

void menuButton::keepButtonHighlighted(){
    this->setTexture(hightlight_texture);
}
void menuButton::reset(){
    this->setTexture(normal_texture);
}

bool menuButton::isLeftClicked(){
    if(this->getGlobalBounds().contains(Mouse::getPosition(*window).x,Mouse::getPosition(*window).y)){
        if(Mouse::isButtonPressed(Mouse::Left) ){
            this->setTexture(hightlight_texture);
            return true;
        }
            this->setTexture(hightlight_texture);
            window->draw(this->hintBox);
            window->draw(this->Hint);
            return false;
    }
    this->setTexture(normal_texture);
    return false;
}
/*  Algorithm Box : */ 
void menuButton::initialize_2(Vector2f position, wstring Name){
    this -> AlgoButton.setPosition(position);
    this -> AlgoButton.setSize(Vector2f(130,40));
    this -> AlgoButton.setOutlineColor(ZONE_OUTLINE_COLOR);
    this -> AlgoButton.setOutlineThickness(LINE_THICKNESS);

    FloatRect TextBounds(this->AlgoName.getGlobalBounds());
    Vector2f box(this->AlgoButton.getSize());
    
    this -> AlgoName.setString(Name);
    this -> AlgoName.setPosition(position.x-AlgoName.getGlobalBounds().width/2,position.y-AlgoName.getGlobalBounds().height);
    this -> AlgoName.setCharacterSize(20);
    this -> AlgoName.setFont(*programFont);
    this -> AlgoName.setFillColor(Color::Black);
}
void menuButton::draw_2(){
    window->draw(this -> AlgoButton);
    window->draw(this -> AlgoName);
}

void menuButton::keepButtonHighlighted_2(){
    this -> AlgoButton.setFillColor(BUTTON_HIGHLIGHT);
    this -> AlgoName.setStyle(AlgoName.Bold);
    this -> AlgoName.setCharacterSize(23);
}
void menuButton::reset_2(){
    this-> AlgoButton.setFillColor(BUTTON_NORMAL);
    this -> AlgoName.setStyle(AlgoName.Regular);
    this -> AlgoName.setCharacterSize(20);
}
bool menuButton::isLeftClicked_2(){
    if(this->AlgoButton.getGlobalBounds().contains(Mouse::getPosition(*window).x,Mouse::getPosition(*window).y)){
        if(Mouse::isButtonPressed(Mouse::Left) ){
            keepButtonHighlighted_2();
            return true;
        }
            keepButtonHighlighted_2();
            return false;
    }
    reset_2();
    return false;
}

#endif

