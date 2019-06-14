#ifndef _menu_hpp_
#define _menu_hpp_
#include "button.hpp"
#include "globalVariables.hpp"
#include "constants.hpp"
class Menu {
    menuButton* buttons;
    int select = -1;
    RectangleShape functionZone;
    RectangleShape fileZone;
    RectangleShape matrixZone;
public:
    Menu();
    ~Menu();
    int Select_Button();
    void resetButtons();
    void draw();
    void jumpTo(int);
    void reset();
};

Menu::Menu(){
    //Zone settings:
    fileZone.setPosition(40,40);
    fileZone.setSize(Vector2f(70,125));
    fileZone.setOutlineColor(ZONE_OUTLINE_COLOR);
    fileZone.setFillColor(BUTTON_ZONE);
    fileZone.setOutlineThickness(3);
    /////////////////////////////////////////////////
    functionZone.setPosition(40,165);
    functionZone.setSize(Vector2f(70,420));
    functionZone.setOutlineColor(ZONE_OUTLINE_COLOR);
    functionZone.setFillColor(BUTTON_ZONE);
    functionZone.setOutlineThickness(3);
    /////////////////////////////////////////////////
    matrixZone.setPosition(1110,260);
    matrixZone.setSize(Vector2f(410,410));
    matrixZone.setOutlineColor(ZONE_OUTLINE_COLOR);
    matrixZone.setFillColor(MATRIX_ZONE);
    matrixZone.setOutlineThickness(3);
    /////////////////////////////////////////////////
    //init array of buttons:
    buttons = new menuButton[NUM_OF_BUTTON];
    //Functions buttons:
    buttons[0].initialize(Vector2f(50,170),"Icons/plus.png","Icons/plus2.png",L"Thêm đỉnh");
    buttons[1].initialize(Vector2f(50,220),"Icons/cross.png","Icons/cross2.png",L"Xóa đỉnh");
    buttons[2].initialize(Vector2f(50,270),"Icons/upRight.png","Icons/upRight2.png",L"Thêm cạnh");
    buttons[3].initialize(Vector2f(50,320),"Icons/wrench.png","Icons/wrench2.png",L"Đổi tên");
    buttons[4].initialize(Vector2f(50,370),"Icons/larger.png","Icons/larger2.png",L"Di chuyển");
    buttons[5].initialize(Vector2f(50,420),"Icons/trashcanOpen.png","Icons/trashcanOpen2.png",L"Xóa tất cả");
    buttons[6].initialize(Vector2f(50,470),"Icons/question.png","Icons/question2.png",L"Giúp đỡ");
    buttons[7].initialize(Vector2f(50,50),"Icons/import.png","Icons/import2.png",L"Lưu file");
    buttons[8].initialize(Vector2f(50,100),"Icons/export.png","Icons/export2.png",L"Load file");
    //Algorithms:
    buttons[9].initialize_2(Vector2f(1110,40),L"DFS");
    buttons[10].initialize_2(Vector2f(1250,40),L"BFS");
    buttons[11].initialize_2(Vector2f(1390,40),L"TPLT");
    buttons[12].initialize_2(Vector2f(1110,90),L"Cạnh Cầu");
    buttons[13].initialize_2(Vector2f(1250,90),L"Đỉnh Trụ");
    buttons[14].initialize_2(Vector2f(1390,90),L"Đỉnh Thắt");
    buttons[15].initialize_2(Vector2f(1110,140),L"X => Y");
    buttons[16].initialize_2(Vector2f(1250,140),L"Hamilton");
    buttons[17].initialize_2(Vector2f(1390,140),L"Euler");
    buttons[18].initialize_2(Vector2f(1250,190),L"SX Topo");
    //FB logo:
    buttons[19].initialize(Vector2f(1500,855),"Icons/fbBlack.png","Icons/fbBlue.png",L"fb");
    buttons[19].setScale(Vector2f(0.3,0.3));
}
void Menu::reset(){
    select = -1;
}
void Menu::jumpTo(int Function){
    select = Function;
}
int Menu::Select_Button(){
    for(int curButton = ADD_VERTEX ;curButton<=FB;++curButton){
        if(curButton == select){
            buttons[select].keepButtonHighlighted();
        }
        else if(buttons[curButton].isLeftClicked()){
            select = curButton;  
        }
        if(curButton>=DFS&&curButton<FB){
            if(curButton == select){
                buttons[select].keepButtonHighlighted_2();
            }
            else if(buttons[curButton].isLeftClicked_2()){
                select = curButton;  
            }
        }
    }
    return select;
}
void Menu::resetButtons(){
    for(int curButton = ADD_VERTEX ;curButton<=EXPORT;++curButton){
        buttons[curButton].reset();
    }
    for(int curButton = DFS; curButton <= TOPO_SORT; ++curButton){
        buttons[curButton].reset_2();
    }
    buttons[FB].reset();
}
void Menu::draw(){
    window->draw(fileZone);
    window->draw(functionZone);
    window->draw(matrixZone);
    for(int  i = 0;i<DFS;i++){
        window->draw(buttons[i]);
    }
    for(int i = DFS;i<=TOPO_SORT;i++){
        buttons[i].draw_2();
    }
    window->draw(buttons[FB]);
}
Menu::~Menu(){
 	//delete[] buttons;
}
#endif