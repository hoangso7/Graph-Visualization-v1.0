#ifndef _NPC_hpp_
#define _NPC_hpp_
#include "globalVariables.hpp"
using namespace sf;
class NPC{
    Texture NPC_Idle;
    Texture NPC_Talking;
    Texture NPC_Welcome;
    Sprite NPC_Player;

    Vertex Triangles[3];
    RectangleShape Speech_Bubble;

    Text Message;
public:
    NPC();
    void init(Vector2f NPC_Pos, Vector2f Bubble_Pos);
    void drawPlayer();
    void drawBubble();
    void drawMsg();
    void drawAll();
    void reset();
    bool isClicked();
    void showResult(wstring msg);
    void announcement(wstring msg);
    void welcome();
};
NPC::NPC(){
    NPC_Idle.loadFromFile("Character/adventurer_idle.png");
    NPC_Talking.loadFromFile("Character/adventurer_action1.png");
    NPC_Welcome.loadFromFile("Character/adventurer_cheer1.png");
    NPC_Player.setTexture(NPC_Idle);
    NPC_Player.setScale(0.8,0.8);
    
    Speech_Bubble.setSize(Vector2f(800,150));
    Speech_Bubble.setFillColor(BUTTON_NORMAL);
    Speech_Bubble.setOutlineThickness(3);

    Message.setCharacterSize(25);
    Message.setFont(*programFont);
    Message.setFillColor(Color::Black);
}
void NPC::init(Vector2f NPC_Pos, Vector2f Bubble_Pos){
    NPC_Player.setPosition(NPC_Pos);
    Speech_Bubble.setPosition(Bubble_Pos);

    Triangles[0].position.x=Speech_Bubble.getGlobalBounds().width+120.f;
    Triangles[0].position.y=Speech_Bubble.getGlobalBounds().width+40.f;
    Triangles[1].position.x=Speech_Bubble.getGlobalBounds().width+120.f;
    Triangles[1].position.y=Speech_Bubble.getGlobalBounds().width-30.f;
    Triangles[2].position=NPC_Pos+Vector2f(0,50);
    for (int i=0; i<3; ++i)
        Triangles[i].color = BUTTON_NORMAL;
}
void NPC::drawPlayer(){
    window->draw(NPC_Player);
}
void NPC::drawBubble(){
    window->draw(Speech_Bubble);
    window->draw(Triangles,3, sf::Triangles);
}
void NPC::drawMsg(){
    window->draw(Message);
}
void NPC::drawAll(){
    drawPlayer();
    drawBubble();
    drawMsg();
}
void NPC::welcome(){
    NPC_Player.setTexture(NPC_Welcome);
    Message.setPosition(Speech_Bubble.getPosition());
    Message.setString(L"\n\tChào mừng bạn tới chương trình mô phỏng thuật toán trên đồ thị.\n\t\t\t\t\t\t\t\t♥ Chúc các bạn học tốt ♥");
    drawAll();
}
//auto endline with text's width >= 750 pixels. -> sometimes cause bugs :>
void NPC::showResult(wstring msg){
    float current_width = 0 ;
    wstring buffer;
    NPC_Player.setTexture(NPC_Talking);
    Message.setPosition(Speech_Bubble.getPosition());
    for(int i = 0; i< msg.length(); i++){
        Message.setString(msg[i]);
        current_width+=Message.getGlobalBounds().width;
        buffer+=msg[i];
        if(current_width+Message.getPosition().x>=750){
            buffer+=L"\n";
            current_width=0;
        }
    }
    Message.setString(buffer);
    drawAll();
}
//only use this function if you can control the flow of string, else, use "showResult" function
void NPC::announcement(wstring msg){
    NPC_Player.setTexture(NPC_Talking);
    Message.setPosition(Speech_Bubble.getPosition());
    Message.setString(msg);
    drawAll();
}
void NPC::reset(){
    NPC_Player.setTexture(NPC_Idle);
    Message.setString("");
}
bool NPC::isClicked(){
    if(Mouse::isButtonPressed(Mouse::Left))
        if(NPC_Player.getGlobalBounds().contains(Mouse::getPosition(*window).x,Mouse::getPosition(*window).y)){
            return true;
        }
    return false;
}
#endif