#ifndef _constants_hpp_
#define _constants_hpp_
#include <SFML/Graphics.hpp>
#include <limits.h>
using namespace sf;
//maximum number of buttons
const int NUM_OF_BUTTON = 20;
//delay Speed:
const int MIN_SPEED = 100;    //miliseconds
const int MAX_SPEED = 1000;   //miliseconds
//maximum number of vertices
const int MAX_NUM_VERTICES = 20;
//Radious of vertex:
const int RADIOUS = 18;
//button:
const int SCRWIDTH = 1550; //(pixels)
const int SCRHEIGHT = 900; //(pixels)
//fps:
const int MAX_FPS_ALLOWED = 60;
//max files can show:
const int MAX_FILES_SHOW= 7;
//mouse wheel handle:
const int MOUSE_WHEEL_UP = 1, MOUSE_WHEEL_DOWN = -1;
//none-connected edge between 2 vertices value -> Put in matrix
enum EDGE_VALUE{
    NO_EDGE_VALUE = -99,
    MAX_EDGE_VALUE = 1981,
    MIN_VALUE_PER_EDGE = -9,
    MAX_VALUE_PER_EDGE = 99
};
enum SAVE_STATE{
    NOT_SAVED,
    FORCE_QUIT,
    SAVED
};
enum MenuButton{
    ADD_VERTEX,
    DEL_VERTEX,
    ADD_EDGE,
    RENAME,
    MOVE_VERTEX,
    DEL_ALL,
    HELP,
    EXPORT,
    IMPORT,
    DFS,
    BFS,
    STRONGLY_CONNECTED_COMPONENTS,
    BRIDGE_EDGE,
    CUT_VERTEX,
    KNOT_VERTEX,
    SHORTEST_PATH,
    HAMILTON,
    EULER_PATH,
    TOPO_SORT,
    FB
};
//COLOR:
const Color VERTEX_VISTITED=Color(244, 198, 88);
const Color VERTEX_OUTLINE_VISITED=Color(232, 110, 34);
const Color VERTEX_ON_STACK=Color(185, 209, 247);
const Color VERTEX_CHOOSE=Color(Color::Cyan);
const Color VERTEX_NORMAL=Color(Color::White);
const Color BUTTON_ZONE=Color(117, 190, 255);
const Color MATRIX_ZONE=Color(145, 203, 255);
const Color LINE_COLOR=Color(68, 91, 163);
const Color LINE_HIGHLIGHT_COLOR = Color(255,102,102);
const Color ZONE_OUTLINE_COLOR=Color(Color::White);
const Color HINTBOX_COLOR=Color(224, 216, 210);
const Color HINTBOX_OUTLINE_COLOR=Color(Color::White);
const Color HINT_WORD_COLOR=Color(Color::Black);
const Color BUTTON_HIGHLIGHT=Color(232, 110, 34);
const Color BUTTON_NORMAL=Color(145, 203, 255);

static Color const VERTEX_MORE_COLOR[]={
    Color(0,0,0),       //BLACK
    Color(0,255,0),     //LIME
    Color(0,0,255),     //BLUE
    Color(255,255,0),   //YELLOW
    Color(0,255,255),   //AQUA
    Color(255,0,255),   //MAGENTA
    Color(192,192,192), //SILVER
    Color(128,128,128), //GRAY
    Color(128,0,0),     //MAROON
    Color(128,128,0),   //OLIVE
    Color(0,128,0),     //GREEN
    Color(128,0,128),   //PURPLE
    Color(0,128,128),   //TEAL
    Color(0,0,128),     //NAVY
    Color(240,230,140), //KHAKI
    Color(138,43,226),  //BLUE VIOLET
    Color(222,184,135), //BURLY WOOD
    Color(255,192,203), //PINKY
    Color(135,206,235), //SKY BLUE
    Color(95,158,160) , //CADET BLUE
//All colors can be found here: https://www.rapidtables.com/web/color/RGB_Color.html
};
//LINE THICKNESS:
const int LINE_THICKNESS = 3;
const int FILEIO_LINE_THICKNESS = 7;
//KEY CODES:

enum KEY{
    BACKSPACES=8,
    ENTER=13,
    ESCAPE=27,
    Y = 121,
    N = 110,
    R = 114,
    MINUS = 45
};

enum WORKING_ZONE{
    WZ_TOP = 55,
    WZ_LEFT = 148,
    WZ_RIGHT = 1064,
    WZ_BOTTOM = 637
};
enum MATRIX_ZONE{
    M_TOP=260,
    M_LEFT=1110,
    M_RIGHT=1520,
    M_BOTTOM=670
};
enum LANGUAGE{
    VIETNAMESE,
    ENGLISH
};
#endif