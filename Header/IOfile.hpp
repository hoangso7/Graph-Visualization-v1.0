#ifndef _IOfile_hpp_
#define _IOfile_hpp_
#include "vertex.hpp"
#include "input.hpp"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class IOfile{
private:
    RectangleShape Border;
    RectangleShape SearchBox;
    RectangleShape fileBorder;
    RectangleShape* fileBox;
    Text* fileName;
    Vector2f pos = Vector2f(550,155);
    Vector2f offset = Vector2f(0,70);
    Texture magnifierTexture;
    Sprite magnifier;
    Input userInput;
    string dir = string("Save/");
    string extension = ".GRAPH";
    vector<string> files = vector<string>();
    Text Hook;
    Clock clock;
    int head = 0, tail = 7;
    string getStr;
public:
    IOfile();
    /*
    ------------------------------------------------------------------------------
                                    GRAPHICS & UI
    ------------------------------------------------------------------------------                            
    */
    void init();
    void draw();    
    void drawFileList();
    void chooseFile();
    void reset();
    void getFiles();
    void readMode();
    void writeMode();
    void updateNewFile(string);
    /*
    ------------------------------------------------------------------------------
                        ALGORITHMS WITH STRINGS &  EVENTS HANDLING
    ------------------------------------------------------------------------------                            
    */
    void getUserInput();
    string getString();
    string getImportFileName();
    bool search();
    bool compare(string );
    void findSubstring(string );
    void emptyBufferString();
    /* MOUSE WHEEL: */
    void ScrolledDown();
    void ScrolledUp();   
};
IOfile::IOfile(){
    /////////////////////////////////////////////////
    Hook.setFillColor(Color::Black);
    Hook.setFont(*programFont);
    Hook.setCharacterSize(30);
    Hook.setPosition(Vector2f(550,650));
    /////////////////////////////////////////////////
    Border.setPosition(Vector2f(500,150));
    Border.setSize(Vector2f(500,550));
    Border.setOutlineThickness(FILEIO_LINE_THICKNESS);
    Border.setOutlineColor(Color::White);
    Border.setFillColor(Color(65,105,225));
    /////////////////////////////////////////////////
    SearchBox.setPosition(Vector2f(550,650));
    SearchBox.setSize(Vector2f(445,45));
    SearchBox.setFillColor(VERTEX_MORE_COLOR[14]);
    SearchBox.setOutlineThickness(LINE_THICKNESS);
    SearchBox.setOutlineColor(Color::Black);
    /////////////////////////////////////////////////
    fileBorder.setPosition(Vector2f(550,155));
    fileBorder.setSize(Vector2f(445,493));
    fileBorder.setFillColor(BUTTON_ZONE);
    fileBorder.setOutlineThickness(LINE_THICKNESS);
    fileBorder.setOutlineColor(Color::Black);
    /////////////////////////////////////////////////
    magnifierTexture.loadFromFile("Icons/SearchIcon.png");
    magnifier.setTexture(magnifierTexture);
    magnifier.setPosition(Vector2f(500,650));
    /////////////////////////////////////////////////
    userInput.init(Vector2f(560,650));
    /////////////////////////////////////////////////
    getFiles();
    init();
}
void IOfile::init(){
    //files.clear();
    if(files.empty())return;
    sort(files.begin(),files.end());
    fileBox = new RectangleShape[files.size()];
    fileName = new Text[files.size()];
    pos = Vector2f(550,155);
    for(int i = 0; i<files.size(); i++){
        string bufferName = to_string(i+1)+" . "+files[i]; 
        /////////////////////////////////////////////////
        fileBox[i].setPosition(pos);
        fileBox[i].setSize(Vector2f(445,70));
        fileBox[i].setOutlineThickness(3);
        fileBox[i].setOutlineColor(Color::Black);
        fileBox[i].setFillColor(Color::Transparent);
        /////////////////////////////////////////////////
        fileName[i].setString(bufferName);
        fileName[i].setFont(*programFont);
        fileName[i].setFillColor(Color::Black);
        fileName[i].setCharacterSize(30);
        fileName[i].setPosition(pos);
        /////////////////////////////////////////////////
        pos += offset;
    }
}
void IOfile::updateNewFile(string newfile){
    files.push_back(newfile);
    init();
    reset();
}
void IOfile::reset(){
    pos = Vector2f(550,155);
    head = 0; tail = 7;
    for(int i = 0; i<files.size(); i++){
        string bufferName = to_string(i+1)+" . "+files[i]; 
        fileBox[i].setPosition(pos);
        fileName[i].setPosition(pos);
        pos += offset;
    }
    draw();
}
void IOfile::getUserInput(){
    if(e->type==Event::TextEntered){
        if(getKey(BACKSPACES)){
            userInput.pop();
            reset();
        }
        if(getASCII()&&clock.getElapsedTime() >= sf::milliseconds(100)) {
            clock.restart();
            userInput.add(e->text.unicode,20);
            reset();
        }
        if(getKey(ENTER)){
            getStr = userInput.get();
        }
    }
    if(userInput.get()== "")return;
    findSubstring(userInput.get());
}
bool IOfile::search(){
    for(int i = 0; i< files.size(); i++){
        if(getStr == files[i])return false;
    }
    return true;
}
void IOfile::draw(){
    window->draw(Border);
    window->draw(magnifier);
    window->draw(SearchBox);
    if(userInput.isEmpty())window->draw(Hook);
    window->draw(fileBorder);
    drawFileList();
}
void IOfile::drawFileList(){
    int i;
    if(files.size()==0)return;
    if(files.size()<=MAX_FILES_SHOW){
        for(i = 0; i<files.size();i++){
            window->draw(fileBox[i]);
            window->draw(fileName[i]);
        }
    }
    else{
        for(i = 0;i<MAX_FILES_SHOW;i++){
            window->draw(fileBox[i]);
        }
        for(i = head;i<tail;i++){
            window->draw(fileName[i]);
        }
        if (clock.getElapsedTime() >= sf::milliseconds(200)) {
            clock.restart();
            if(e->type == sf::Event::MouseWheelScrolled){
                if(e->mouseWheelScroll.delta==MOUSE_WHEEL_UP){
                    ScrolledUp();                    
                }
                else if(e->mouseWheelScroll.delta==MOUSE_WHEEL_DOWN){
                    ScrolledDown();
                }
            }
        }
        
    }
}
void IOfile::ScrolledUp(){
    if(head==0)return;
    head--;tail--;
    int p = 0, i;
    for(i = head;i<tail;i++){
        fileName[i].setPosition(fileBox[p++].getPosition());
    }
}
void IOfile::ScrolledDown(){
    if(tail==files.size())return;
    head++;tail++;
    int k = 0, i;
    for(i = head;i<tail;i++){
        fileName[i].setPosition(fileBox[k++].getPosition());
    }
}
void IOfile::readMode(){
    Hook.setString(L"  Tìm kiếm...");
    chooseFile();
    draw();
    getUserInput();
    userInput.draw();
}
void IOfile::writeMode(){
    Hook.setString(L"  Nhập tên File mới...");
    chooseFile();
    draw();
    getUserInput();
    userInput.draw();
}
string IOfile::getString(){
    if(search())return getStr;
    else return "";
}
void IOfile::chooseFile(){
    if(files.size()==0)
        for(int i = 0; i<MAX_FILES_SHOW;i++){
            if(fileBox[i].getGlobalBounds().contains(Mouse::getPosition(*window).x, Mouse::getPosition(*window).y)){
                fileBox[i].setFillColor(BUTTON_HIGHLIGHT);
            }
            else{
                fileBox[i].setFillColor(Color::Transparent);
            }
        }
    else{
        for(int i = 0; i<files.size();i++){
            if(fileBox[i].getGlobalBounds().contains(Mouse::getPosition(*window).x, Mouse::getPosition(*window).y)){
                fileBox[i].setFillColor(BUTTON_HIGHLIGHT);
            }
            else{
                fileBox[i].setFillColor(Color::Transparent);
            }
        }
    }

}
string IOfile::getImportFileName(){
    for(int i = 0; i<MAX_FILES_SHOW;i++){
        if(Mouse::isButtonPressed(Mouse::Left)){
            if(fileBox[i].getGlobalBounds().contains(Mouse::getPosition(*window).x, Mouse::getPosition(*window).y)){
                return files[i+head];
            }
        }
    }
    return string("");
}
void IOfile::emptyBufferString(){
    getStr="";
    userInput.reset();
}
void IOfile::findSubstring(string subStr){
    if(files.size()==0)return;
    window->draw(fileBorder);
    pos = Vector2f(550,155);
    for(int i = 0; i< files.size(); i++){
        if(files[i].find(subStr)!=string::npos){
            fileBox[i].setPosition(pos);
            fileName[i].setPosition(pos);
            pos += Vector2f(0,70);
            window->draw(fileBox[i]);
            window->draw(fileName[i]);
            if(i==MAX_FILES_SHOW-1)break;
        }
    }
}
//get this code from : https://www.linuxquestions.org/questions/programming-9/c-list-files-in-directory-379323/ ;))
void IOfile::getFiles (){
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
    }
    while ((dirp = readdir(dp)) != NULL) {
        if(compare(string(dirp->d_name))){
            files.push_back(string(dirp->d_name));
        }
    }
    closedir(dp);
}
bool IOfile::compare(string fileName){
    int Index = 0;
    if(fileName.length()<7)return false;
    for(int i = fileName.length()-6;i<fileName.length();i++){
        if(fileName[i] != extension[Index])return false;
        Index ++;
    }
    return true;
}
#endif