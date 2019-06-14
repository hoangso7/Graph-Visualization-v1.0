#ifndef _globalVariables_hpp_
#define _globalVariables_hpp_
#include <SFML/Graphics.hpp>
#include "constants.hpp"

//init window
sf::RenderWindow* window = new RenderWindow(VideoMode(SCRWIDTH, SCRHEIGHT),L"Graph Visualizations v1.0", sf::Style::Close);
//handle events
sf::Event *e = new Event;
//delay speed:
int delaySpeed = 500; //miliseconds
//Program font
Font* programFont = new Font;
//save file check
bool isSaved = true;
//adj graph
int graph[MAX_NUM_VERTICES][MAX_NUM_VERTICES];
int trace[MAX_NUM_VERTICES]; 

#endif