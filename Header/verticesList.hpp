#ifndef _verticesList_hpp_
#define _verticesList_hpp_
#include "vertex.hpp"
#include "sfarrow.hpp"
#include "input.hpp"
#include <fstream>
#include <stack>

class verticesList{
    int num = 0, vertexName = 1;
    int choosingVertex = -1;
    Input input;
    string str;
    vertex* Vertices[MAX_NUM_VERTICES];
    vertex* bufferV;
    Clock clock;
public:
    verticesList();
    ~verticesList();
    /*
    ------------------------------------------------------------------------------
                                VERTICES LIST ALGORITHMS
    ------------------------------------------------------------------------------                            
    */
    bool isEmpty();
    bool isFull();
    bool check();
    bool checkName(string VertexName);
    bool findVertex(int);
    void createVertex();
    void eraseVertex();
    void removeVertexfromMatrix(int);
    void removeRow(int);
    void removeCol(int);
    void removeLast();
    void addEdge();
    void renameVertex();
    void moveVertex(Vector2f, int);
    void refresh();
    void refreshMatrix();
    void exportFile(string);
    void importFile(string);
    /*
    ------------------------------------------------------------------------------
                                GRAPHICS HANDLING
    ------------------------------------------------------------------------------                            
    */
    void showEdges();
    void drawEdges(int ,int ,Color);
    void chooseVertex();
    void Draw();
    void highlightVertex(int index, Color color);
    void highlightOutlineVertex(int index, Color color);
    void resetVertexColor();
    /*
    ------------------------------------------------------------------------------
                                    GET INFOS
    ------------------------------------------------------------------------------                            
    */
    int getVertex();
    int getSize();
    string getName(int); 
};
verticesList::verticesList(){ 
    refreshMatrix();
}
void verticesList::refreshMatrix(){
    for(int i = 0;i<MAX_NUM_VERTICES;i++){
        for(int j = 0;j<MAX_NUM_VERTICES;j++){
            graph[i][j]=NO_EDGE_VALUE;
        }
    }
}
int verticesList::getSize(){
    return num;
}
bool verticesList::check(){
    if(isEmpty())return false;
    for(int i = 0 ; i < num;i++){
        if(bufferV->getGlobalBounds().intersects(Vertices[i]->getGlobalBounds())){
            return true;
        }
    }
    return false;
}
bool verticesList::findVertex(int i){
    return Vertices[i]->getGlobalBounds().contains(Vector2f(Mouse::getPosition(*window)));
}
void verticesList::moveVertex(Vector2f newPos, int i){
    Vertices[i]->Move(newPos);
}
void verticesList::chooseVertex(){
    if(isEmpty())return;    
    for(int i = 0;i<num;++i){
        Vertices[i]->update();
    }
}
int verticesList::getVertex(){
    int k = -1;
    for(int i = 0;i<num;i++){
        if(Vertices[i]->isClicked()){
            k = i;
            break;
        }
    }
    return k;
}
bool verticesList::checkName(string VertexName){
    if(isEmpty())return false;
    for(int i = 0;i<num;i++){
        if(Vertices[i]->getName()==VertexName)
            return false;
    }
    return true;
}
void verticesList::renameVertex(){
    if(isEmpty())return;
    for(int i = 0;i<num;i++){
        if(Vertices[i]->isClicked()){
            input.reset();
            choosingVertex = i;
            input.init(Vector2f(Vertices[choosingVertex]->getPosition().x+RADIOUS,Vertices[choosingVertex]->getPosition().y));
            input.draw();
            break;
        }
    }
    if(Mouse::isButtonPressed(Mouse::Right)){
        choosingVertex = -1;
        input.reset();
        return;
    }
    if(choosingVertex != -1){
        Vertices[choosingVertex]->highlight(Color::Cyan);
        string nameBuffer = Vertices[choosingVertex]->getName();   
        input.draw();
        if(e->type==Event::TextEntered){
            if(getKey(BACKSPACES)){
                input.pop();
            }
            if (getASCII()) {
                input.add(e->text.unicode,2);
            }
            if(getKey(ENTER)){
                if(input.check()){
                    string newName = input.get();
                    if(checkName(newName)){
                        Vertices[choosingVertex]->Rename(input.get());
                        choosingVertex = -1;
                        return;
                    }
                    else{
                        input.reset();
                    }
                }
            }
        }
    }
}
void verticesList::eraseVertex(){
    if(isEmpty())return;
    for(int i = 0;i<num;i++){
        if(Vertices[i]->isClicked()){
            for(int j = i;j<num;j++)Vertices[j]=Vertices[j+1];
            removeVertexfromMatrix(i);
            num--;
        }
    }
}
void verticesList::removeVertexfromMatrix(int v){
    removeRow(v);
    removeCol(v);
    removeLast();
}
inline void verticesList::removeRow(int row)
{
	for (int i = row; i < num - 1; ++i)
		for (int j = 0; j < num; ++j)
			graph[i][j] = graph[i + 1][j];
}
inline void verticesList::removeCol(int col)
{
	for (int i = 0; i < num; ++i)
		for (int j = col; j < num - 1; ++j)
			graph[i][j] = graph[i][j + 1];
}
inline void verticesList::removeLast()
{
	const int &LAST = num - 1;
	for (int i = 0; i < num; ++i) //Remove last row & last column
	{
		graph[LAST][i] = NO_EDGE_VALUE;
		graph[i][LAST] = NO_EDGE_VALUE;
	}
}
void verticesList::addEdge(){
    if(isEmpty())return;
    static int weight = 0;
    for(int i = 0;i<num;i++){
        if(Vertices[i]->isClicked()){
            choosingVertex = i;
        }
    }
    if(Mouse::isButtonPressed(Mouse::Right)){
        choosingVertex = -1;
        weight=0;
        return;
    }
    if(choosingVertex != -1){
        Vertices[choosingVertex]->highlight(Color::Cyan);
        Vector2f sPoint = Vertices[choosingVertex]->getPosition();
        Vector2i ePoint = Mouse::getPosition(*window);
        sfLine temp;
        temp.connect(sPoint,(Vector2f)ePoint,*window,RenderStates::Default,LINE_COLOR);
        if(!inWorkingZone(Vector2f(ePoint)))return;
        Vector2f midPoint;
        midPoint.x=(sPoint.x+ePoint.x)/2.f;
        midPoint.y=(sPoint.y+ePoint.y)/2.f;
        input.init(midPoint);
        input.changeSize(20);
        input.draw();
        if(e->type==Event::TextEntered){
            if(getKey(BACKSPACES)){
                input.pop();
            }
            if(getNum()||getKey(MINUS)) {
                input.add(e->text.unicode,2);
            }
            if(getKey(ENTER)){
                if(inputWeightIsSafe(input.get())){
                    weight = stoi(input.get()); //stoi(string): convert string to integer
                    input.changeColor(Color::White);
                }
            }
        }
        for(int curVertex = 0; curVertex < num ; curVertex++){
            if(Vertices[curVertex]->getGlobalBounds().contains(ePoint.x,ePoint.y)&&curVertex!=choosingVertex){
                graph[choosingVertex][curVertex] = weight;
                isSaved=false;
                input.reset();
                weight = 0;
                choosingVertex = -1;
            }
        }
    }
}
void verticesList::showEdges(){
    if(isEmpty())return;
    for(int i = 0;i<num;i++){
        for(int j = 0; j<num;j++){
            if(graph[i][j]!=NO_EDGE_VALUE){
                drawEdges(i,j, LINE_COLOR);
            }
        }
    }
}

void verticesList::drawEdges(int start, int end, Color color){
    Vector2f s = Vertices[start]->getPosition();
    Vector2f e = Vertices[end]->getPosition();
	if (graph[end][start] == NO_EDGE_VALUE || start < end){
        sfLine Arrow(s,e,color,to_string(graph[start][end]));
        Arrow.draw(*window,RenderStates::Default);
    }
	else{
        sfLine CurvedLine;
        CurvedLine.drawCurvedLine(e,s,color,to_string(graph[start][end]));
    }
}
bool verticesList::isEmpty(){
    return num == 0;
}
bool verticesList::isFull(){
    return num == MAX_NUM_VERTICES;
}
void verticesList::createVertex(){
    if(isEmpty())vertexName=1;
    bufferV = new vertex((Vector2f)Mouse::getPosition(*window),to_string(vertexName));
    if(!isFull()&&!check()){
        Vertices[num]=bufferV;
        num++;
        vertexName++;
        isSaved=false;      
        return;// add new vertex successfully!
    }
    delete bufferV;
}
void verticesList::Draw(){
    if(isEmpty())return;
    for(int i = 0; i < num ; i++){
        Vertices[i]->Draw();
    }
}
void verticesList::highlightVertex(int index, Color color){
    Vertices[index]->highlight(color);
}
void verticesList::highlightOutlineVertex(int index, Color color){
    Vertices[index]->highlightOutline(color);
}
void verticesList::resetVertexColor(){
    for(int i = 0; i< num; i++){
        Vertices[i]->highlight(VERTEX_NORMAL);
    }
}
void verticesList::refresh(){
    while (--num >= 0)
		delete Vertices[num];
	num = 0;
    vertexName=0;
    refreshMatrix();
    vertexName = 1;
}
string verticesList::getName(int index){
    return Vertices[index]->getName();
}
void verticesList::exportFile(string newFile){
    if(isEmpty())return;
    string path = "Save/"+newFile;
    ofstream outputFile(path);
    outputFile<<num<<"\n";
    for(int i = 0;i < num;i++){
        str = Vertices[i]->getName();
        outputFile<<Vertices[i]->getPosition().x<<" "<<Vertices[i]->getPosition().y<<" "<<str<<"\n";
    }
    for(int i = 0;i<num;i++){
        for(int j = 0;j<num;j++)
            outputFile<<graph[i][j]<<" ";
        outputFile<<"\n";
    }    
    outputFile.close();
    isSaved=true;
    return;
}
void verticesList::importFile(string input){
    string path = "Save/"+input;
    ifstream inputFile(path);
    isSaved=true;
    if(!inputFile.is_open()){
        cout<<"Cannot open: "<<input<<"\n";
        return;
    }// => just in case for debugging
    int numOfVertex;
    if(!inputFile.is_open())return;
    refresh();
    inputFile>>numOfVertex;
    for(int i = 0;i < numOfVertex;i++){
        Vector2f position;
        float x,y;
        inputFile>>position.x>>position.y>>str;
        bufferV = new vertex(position,str);
        Vertices[num]=bufferV;
        num++;
        vertexName++;
    }
    for(int i = 0;i<numOfVertex;i++){
        for(int j = 0;j<numOfVertex;j++){
            inputFile>>graph[i][j];
        }
    }
    inputFile.close();
    return;
}
verticesList::~verticesList(){
    //delete[] Vertices;
    //delete bufferV; 
}
#endif
    

