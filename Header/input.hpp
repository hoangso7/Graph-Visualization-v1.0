#ifndef _input_hpp_
#define _input_hpp_
#include "globalVariables.hpp"
using namespace sf;
//using namespace std;
class Input{
	Text text;
	string str;
	Clock clock;
public:
	Input();
	void init(Vector2f position);
	void add(char , int);
	void pop();
	void draw();
	string get();
	bool isEmpty();
	int check();
	void reset();
	void changeSize(int );
	void changeColor(Color);
};
Input::Input(){
}
void Input::init(Vector2f position){
	text.setCharacterSize(30);
	text.setFont(*programFont);
	text.setPosition(position);
	text.setFillColor(Color::Red);
}
void Input::changeSize(int size){
	text.setCharacterSize(size);
}
void Input::changeColor(Color c){
	text.setFillColor(c);
	draw();
}
void Input::add(char c, int max){
	if (clock.getElapsedTime() >= sf::milliseconds(150)) {
		clock.restart();
		if(str.length()<max){
			str += c;
			text.setString(str);
		}
	}
	else return;
}

void Input::pop(){
	if (clock.getElapsedTime() >= sf::milliseconds(150)) {
		clock.restart();
		if(str.empty()){
			return;
		}
		str.pop_back();
		text.setString(str);
	}
}

void Input::draw(){
	window->draw(text);
}

string Input::get(){
	return str;
}
int Input::check(){
	if(str.length()==0)return 0;
	else if(str.length()>0&&str.length()<=2)return 1;
	else return -1;
}

void Input::reset(){
	str.clear();
	text.setString("");
}

bool Input::isEmpty(){
	return str.length()==0;
}
#endif