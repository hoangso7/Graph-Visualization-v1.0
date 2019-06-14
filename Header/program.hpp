#ifndef _program_hpp_
#define _program_hpp_
#include "menu.hpp"
#include "mainBoard.hpp"
#include "sfarrow.hpp"
#include "verticesList.hpp"
#include "NPC.hpp"
#include "IOfile.hpp"
#include "stack.hpp"
#include "queue.hpp"
#include <unordered_map>
#include <queue>
using namespace sf;
class Program{
    //main data structure:
    verticesList* vList;//Verices List

    //menu objects:
    Menu menu;//Menu buttons, Function zone, buttons zone, etc...
    mainBoard BOARD;//Workzone Board
    int cur_button = -1;//menu button being choosen
    Clock clock;//Handling time
    Texture BG; //Background Texture
    Sprite BGimage;//Background Sprite
    Image Icon;//Program Icon
    Texture saveTexture_on;//Saved texture
    Texture saveTexture_off;//Not-Saved texture
    Sprite Save;//Save texture
    NPC printText;// NPC : printText -> annoucement/stack-queue result
    IOfile fileUI;// input & output file UI
    Input input;//input from keyboard

    //  Algorithm Global Variables: 
    int start = -1;// use for DFS/BFS Algorithm
    int *source,*dest; //use for Knot Vertex/ Shortest Path
    int hamiltonCount;//use for num of hamilton cycle
    bool flag = false;//use for topo sort with many vertices with degIn = 0
public:
    Program();
    ~Program();
    /*
    ------------------------------------------------------------------------------
                                PROCESS LOOP
    ------------------------------------------------------------------------------                            
    */
    void mainMenu();
    void run();
    void Process();
    int on_quit();
    /*
    ------------------------------------------------------------------------------
                                FRAMES && DRAW 
    ------------------------------------------------------------------------------                            
    */
    void drawBoard();
    void drawMenu();
    void drawVertexList();
    void drawMatrix();
    void updateFrame();
    void showSaveState(bool isSaved);
    bool emptyGraph();
    /*
    ------------------------------------------------------------------------------
                                MAIN MENU
    ------------------------------------------------------------------------------                            
    */
    void saveFile();
    void loadFile();
    void loadFileStart();
    void addVertex();
    void delVertex();
    void addEdge();
    void Rename();
    void delAll();
    void moveVertex();
    void Help();
    void contactMe();
    /*
    ------------------------------------------------------------------------------
                                ALGORITHMS MENU
    ------------------------------------------------------------------------------                            
    */
    void dfs();
    void bfs();
    void SCC(); 
    void bridgeEdge();
    void cutVertex();
    void knotVertex();
    void findShortestPath();
    void hamilton();
    void euler();
    void topoSort();
    /*--------------------------------------------------------------------------
                            SUB-ALGORITHMS FUNCTIONS USED
    ----------------------------------------------------------------------------
    */
        // DEPTH FIRST SEARCH:
    void dfsTraveler(int);
        // BREADTH FIRST SEARCH:
    void bfsTraveler(int);
        // STRONG CONNECTED COMPONENTS:
    void tarjanVisit(int, int *, int *, mstack &, int &, int &, bool);
    void tarjanResult(mstack &, int, int);
    int tarjanAlgo(bool , int remove);
        // FIND KNOT VERTICES
    int** getTempGraph(int,int);
    bool bfsToCheckKnot(int,int,int,int**);
    bool dfsToCheckConnected(int,int);
        //TOPO SORT:
    void topoSortStart(int);
    bool KahnAlgorithm_TopoSort(int*,int*);
    void findAllPossiblePath(int,int,bool*,int*,int&);
    void printPath(int*,int);
        // EULER CIRCUIT:
    void traverse(int , bool[]);
    bool isConnected();
    bool isEulerCircuit();
    void findEulerCycle();
    void printEulerCycle(vector<int>);
        // HAMILTON CIRCUIT:
    bool recursiveHamilton(int[], int);
    bool canBeAdded(int,int[],int);
    void printHamiltonCycle(int[]);
        // FIND THE SHORTEST PATH BETWEEN X & Y
    bool BellmanFord();
    bool isNegativeCycle(int*);
    void showShortestPath(int*,int*);
    /*
    ------------------------------------------------------------------------------
                                HELPERS
    ------------------------------------------------------------------------------                            
    */
    bool getTwoVertex(int*, int*);
    bool userWantExit();
    void backToMenu();
};

Program::Program(){
    vList = new verticesList;
    /////////////////////////////////////////////////
    BOARD.init(Vector2f(130,37),Vector2f(0.75,0.75),"Background/mainboard.jpg");
    printText.init(Vector2f(1000,800),Vector2f(130,730));
    /////////////////////////////////////////////////
    window->setFramerateLimit(MAX_FPS_ALLOWED); 
    /////////////////////////////////////////////////
    BG.loadFromFile("Background/background.jpg");
    BGimage.setTexture(BG);
    BGimage.setScale(0.83,0.83);
    /////////////////////////////////////////////////
    programFont->loadFromFile("Font/Arial.ttf");
    /////////////////////////////////////////////////
    Icon.loadFromFile("Icons/GraphLogo.png");
    window->setIcon(Icon.getSize().x,Icon.getSize().y,Icon.getPixelsPtr());
    /////////////////////////////////////////////////
    saveTexture_off.loadFromFile("Icons/not_saved.png");
    saveTexture_on.loadFromFile("Icons/saved.png");
    Save.setPosition(Vector2f(55,530));
    /////////////////////////////////////////////////
    source = new int;
    dest = new int;
    *source = -1;
    *dest = -1;
}
void Program::showSaveState(bool isSaved){
    if(isSaved)Save.setTexture(saveTexture_on);
    else Save.setTexture(saveTexture_off);
    window->draw(Save);
}
void Program::Process(){
    window->clear();
    window->draw(BGimage);
    drawBoard();
    drawVertexList();
    mainMenu();
    drawMenu();
    drawMatrix();
    printText.drawPlayer();
    if(printText.isClicked()){
        printText.welcome();
        updateFrame();
    }
    else printText.reset();
    cur_button = menu.Select_Button();
    showSaveState(isSaved);
    window->display(); 
}
void Program::updateFrame(){
    window->clear();
    window->draw(BGimage);
    drawBoard();
    drawVertexList();
    drawMenu();
    drawMatrix();
    printText.drawAll();
    showSaveState(isSaved);
}
void Program::run(){
    while (window->isOpen()){
        while (window->pollEvent(*e)){
            if(e->type==Event::Closed){
                switch(on_quit()){
                    case NOT_SAVED:{
                        menu.jumpTo(EXPORT);
                        break;// save file.
                    }
                    case FORCE_QUIT:{
                        vList->exportFile("AUTO_SAVE.GRAPH");//to recover last unsaved-session
                        window->close();//exit program, break program loop
                        break;
                    }
                    case SAVED:{
                        break;//do nothing, just quit
                    }
                }
            }
        }
        Process();
    }
}
int Program::on_quit(){
    menu.reset();
    while(1){
        if(isSaved==false){
            if( Keyboard::isKeyPressed(Keyboard::Y)){
                return 0;
            }
            if( Keyboard::isKeyPressed(Keyboard::N)){
                menu.jumpTo(EXPORT);
                return 1;
            }
            if( Keyboard::isKeyPressed(Keyboard::Escape)){
                return 2;
            }
            printText.announcement(L"Nhận thấy đồ thị hiện tại chưa được lưu, bạn có muốn lưu không?\n\tPhím <Y> : CÓ\n\tPhím <N> : KHÔNG\n\tPhím <ESC> : QUAY LẠI");
            updateFrame();
            window->display();
        }else{
            return 1;
        }
    }
    return 0;
}

bool Program::userWantExit(){
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
		return true;
	}
    return false;
} 
void Program::backToMenu(){
    printText.announcement(L"Kết Thúc");
    updateFrame();
    menu.reset();
}
void Program::loadFile(){
    if(vList->isEmpty())loadFileStart();
    else{
        if(isSaved==false){
            printText.announcement(L"Nhận thấy đồ thị hiện tại chưa được lưu, bạn có muốn lưu không?\n\tPhím <Y> : CÓ\n\tPhím <N> : KHÔNG\n\tPhím <ESC> : HỦY BỎ");
            updateFrame();
            if(e->type==Event::TextEntered){
                if(getKey(Y)){
                    menu.jumpTo(EXPORT);
                }
                if(getKey(N)){
                    vList->refresh();
                    loadFileStart();
                }
            }
            if(userWantExit()){
                menu.reset();
            }
        }
        else{
            vList->refresh();
            loadFileStart();
        }
    }
}
void Program::loadFileStart(){
    if(userWantExit())menu.reset();
    printText.announcement(L"Chọn File bạn cần load");
    updateFrame();
    fileUI.readMode();
    string input = fileUI.getImportFileName();
    if(input != ""){
        vList->importFile(input);
        menu.reset();
    }
}
void Program::saveFile(){
    if(vList->isEmpty()){
        printText.announcement(L"Không có gì để lưu cả!");
        updateFrame();
        if(userWantExit()){
            menu.reset();
            return;
        }
    }else{
        static bool ok = true;
        if(userWantExit()){
            menu.reset();
            ok = true;
            return;
        }
        if(ok==false){
            printText.announcement(L"Tên File không hợp lệ, vui lòng kiểm tra lại!\nẤn <ESC> để thoát.");
            updateFrame();
            return;
        }
        else{
            printText.announcement(L"Nhập vào tên cho file, lưu ý: \n\t- Không được trùng tên\n\t- Không được chứa ký tự '/' ");
            updateFrame();
            fileUI.writeMode();
            string newFile = fileUI.getString();
            if(newFile!=""){
                if(newFile.find("/")!=string::npos){    //UNIX system does not allow file's name contains "/" character
                    fileUI.emptyBufferString();
                    ok = false;
                }
                else{
                    newFile+=".GRAPH";
                    vList->exportFile(newFile);
                    fileUI.updateNewFile(newFile);
                    fileUI.emptyBufferString();
                    menu.reset();
                }
            }
        }
    }
}
void Program::addVertex(){
    if(userWantExit())menu.reset();
    if(vList->getSize() == MAX_NUM_VERTICES){
        printText.announcement(L"Danh sách đỉnh đã đầy!");
    }
    else{
        printText.announcement(L"Click vào bảng để tạo đỉnh mới, tối đa 20 đỉnh.");
    }
    if(BOARD.IsClicked()){
        vList->createVertex();
    }
    updateFrame();
}
void Program::delVertex(){
    if(emptyGraph())return;
    if(userWantExit())menu.reset();
    printText.announcement(L"Click vào đỉnh để xóa.");
    updateFrame();
    vList->chooseVertex();
    vList->eraseVertex();
}
void Program::addEdge(){
    if(emptyGraph())return;
    if(userWantExit())menu.reset();
    printText.announcement(L"Click vào đỉnh để chọn đỉnh đầu.\n\t- Click chuột phải để hủy.\n\t- Nhập và ENTER trọng số trước khi nối tới đỉnh kế\n\t- Điều kiện trọng số : [-9;99],  giá trị mặc định : 0");
    updateFrame();
    vList->chooseVertex();
    vList->addEdge();
    vList->showEdges();
    drawVertexList();
}
void Program::Rename(){
    if(emptyGraph())return;
    if(userWantExit())menu.reset();
    printText.announcement(L"Click vào đỉnh để đổi tên. Tên mới phải: \n\t- Không được rỗng hoặc quá 2 kí tự.\n\t- Không được trùng với những đỉnh có trên bảng.\n\t- Cho phép sử dụng ký tự đặc biệt.");
    updateFrame();
    vList->chooseVertex();
    vList->renameVertex();
}
void Program::moveVertex(){
    if(emptyGraph())return;
    printText.announcement(L"Click và giữ đỉnh, sau đó rê chuột để di chuyển.");
    if(userWantExit())menu.reset();
    vList->chooseVertex();
    for(int i = 0;i < vList->getSize(); i++){
        if(vList->findVertex(i)){
            while(Mouse::isButtonPressed(Mouse::Left)){
                if(!inWorkingZone(Vector2f(Mouse::getPosition(*window))))break;
                vList->moveVertex(Vector2f(Mouse::getPosition(*window)),i);
                isSaved=false;
                updateFrame();
                window->display();
            }
        }
    }
}
void Program::Help(){
    if(userWantExit())menu.reset();
    printText.announcement(L"\t\t\t\t\t\t\t\t\tHướng dẫn sử dụng:\n\t1. Thực hiện tạo đỉnh và cạnh trước khi chạy mô phỏng thuật toán.\n\t2. Nhấn giữ phím <ESC> hủy tiến trình thực hiện thuật toán.\n\t3. Để khôi phục lần hoạt động gần nhất, load file AUTO_SAVE.\nTác giả: Nguyễn Minh Hoàng (D17CQAT01-N, MSSV: N17DCAT029).");
}
void Program::delAll(){
    if(emptyGraph())return;
    if(isSaved==false){
        printText.announcement(L"Nhận thấy đồ thị hiện tại chưa được lưu, bạn có muốn lưu không?\n\tPhím <Y> : CÓ\n\tPhím <N> : KHÔNG\n\tPhím <ESC> : HỦY BỎ");
        updateFrame();
        if(e->type==Event::TextEntered){
            if(getKey(Y)){
                menu.jumpTo(EXPORT);
            }
            if(getKey(N)){
                vList->refresh();
                menu.reset();
            }
        }
        if(userWantExit()){
            menu.reset();
        }
    }
    else{
        vList->refresh();
        menu.reset();
    }
}

void Program::drawBoard(){
    BOARD.draw();
}
void Program::drawMenu(){
    menu.draw();
}
void Program::drawVertexList(){
    vList->showEdges();
    vList->Draw();
}
void Program::drawMatrix(){
    const int &n = vList->getSize();
    if(n==0){// if matrix is empty
        Text Announcement;
        Announcement.setCharacterSize(25);
        Announcement.setFillColor(Color::Black);
        Announcement.setFont(*programFont);
        Announcement.setString(L"Đồ thị rỗng!");
        Announcement.setPosition(Vector2f(1250,440));
        window->draw(Announcement);
    }else{
        const float width = float(M_RIGHT - M_LEFT) / n;
        const float height = float(M_BOTTOM - M_TOP) / n;
        sfLine line;
        Text Weight,vName_x,vName_y;
        Weight.setCharacterSize(15);
        Weight.setFillColor(Color::Red);
        Weight.setFont(*programFont);
        vName_x.setCharacterSize(15);
        vName_x.setFillColor(Color::Black);
        vName_x.setFont(*programFont);
        vName_y.setCharacterSize(15);
        vName_y.setFillColor(Color::Black);
        vName_y.setFont(*programFont);
        for(int i = 0; i<n ; i++){
            line.straightLine(Vector2f(M_LEFT, M_TOP + i * height), Vector2f(M_RIGHT, M_TOP + i * height),ZONE_OUTLINE_COLOR);
            line.straightLine(Vector2f(M_LEFT + i * width, M_TOP), Vector2f(M_LEFT + i * width, M_BOTTOM),ZONE_OUTLINE_COLOR);
            vName_x.setString(vList->getName(i));
            vName_x.setPosition(Vector2f(1094,260+i*height));
            window->draw(vName_x);
            vName_y.setString(vList->getName(i));
            vName_y.setPosition(Vector2f(1112+i*width,240));
            window->draw(vName_y);
            for(int j = 0;j<n;j++){
                if (graph[i][j] != NO_EDGE_VALUE){
                    Weight.setString(to_string(graph[i][j]));
                    Weight.setPosition(Vector2f(1112+j*width,260+i*height));
                    window->draw(Weight);
                }
            }
        }
    }
}
void Program::contactMe(){
    if (clock.getElapsedTime() >= sf::milliseconds(500)) {//check if multy-clicks in a short period of time (500ms)
		clock.restart();
		system("xdg-open https://www.facebook.com/HoangNM07");// open url in defalut browser
	}
	else {
        menu.reset();
        return;
    }
}
bool Program::emptyGraph(){
    if(vList->isEmpty()){
        printText.announcement(L"Hiện đồ thị đang trống, hãy tạo vài đỉnh để bắt đầu.");
        updateFrame();
        return true;
    }
    return false;
}
void Program::dfs(){
    if(emptyGraph())return;
    else{
        printText.announcement(L"Đây là thuật toán duyệt đồ thị theo chiều sâu\nHãy chọn đỉnh bắt đầu.");
        updateFrame();
        vList->chooseVertex();
        if(start == -1){
            start = vList->getVertex();
        }
        if(start == -1){
            return;
        }
        dfsTraveler(start);
        if(userWantExit()){
            menu.reset();
            printText.reset();
            start = -1 ;
        }
    }
}
void Program::dfsTraveler(int start){
    bool notvisited[vList->getSize()];
    wstring result = L"Đỉnh bắt đầu: "+vList->getName(start)+L"\nStack: ";
    setArrayTo(notvisited,vList->getSize(),true);
    int s;
    mstack Stack;
    Stack.push(start);
    notvisited[start]=false;
    while(!Stack.isEmpty()){
        if(userWantExit()){
            menu.reset();break;
        }
        s = Stack.pop();
        vList->highlightVertex(s,VERTEX_VISTITED);
        sleep(seconds(0.5f));
        result+= L"(" + vList->getName(s) + L")=>";
        printText.showResult(result);
        updateFrame();
        window->display();
        for(int v = 0; v < MAX_NUM_VERTICES; v++){
            if(userWantExit()){
            menu.reset();break;
            }
            if(notvisited[v]&&graph[s][v] != NO_EDGE_VALUE){
                vList->highlightVertex(v,VERTEX_ON_STACK);
                notvisited[v]=false;
                Stack.push(v);  
                updateFrame();
                sleep(seconds(0.5f));
                window->display();
            }
        }
    }
}
void Program::bfs(){
    if(emptyGraph())return;
    else{
        printText.announcement(L"Đây là thuật toán duyệt đồ thị theo chiều rộng\nHãy chọn đỉnh bắt đầu.");
        updateFrame();
        vList->chooseVertex();
        if(start == -1){
            start = vList->getVertex();
        }
        if(start == -1){
            return;
        }
        bfsTraveler(start);
        if(userWantExit()){
            menu.reset();
            printText.reset();
            start = -1 ;
        }
    }
}
void Program::bfsTraveler(int start){
    bool notvisited[vList->getSize()];
    wstring result = L"Đỉnh bắt đầu: "+vList->getName(start)+L"\nQueue: ";
    setArrayTo(notvisited,vList->getSize(),true);
    int s;
    mqueue Queue;
    Queue.push(start);
    notvisited[start]=false;
    vList->highlightVertex(start,VERTEX_VISTITED);
    while(!Queue.isEmpty()){
        if(userWantExit()){
            menu.reset();break;
        }
        s = Queue.pop();
        vList->highlightVertex(s,VERTEX_VISTITED);
        result+= L"(" + vList->getName(s) + L")=>";
        printText.showResult(result);
        updateFrame();
        sleep(seconds(0.5f));
        window->display();
        for(int v = 0; v < MAX_NUM_VERTICES; v++){
            if(userWantExit()){
            menu.reset();break;
            }
            if(notvisited[v]&&graph[s][v] != NO_EDGE_VALUE){
                vList->highlightVertex(v,VERTEX_ON_STACK);
                notvisited[v]=false;
                Queue.push(v);  
                sleep(seconds(0.5f));
                updateFrame();
                window->display();
            }
        }
    }
}
void Program::SCC(){
    if(emptyGraph())return;
    else{
        int count = tarjanAlgo(1,-1);
        wstring result = L"Số thành phần liên thông tìm được: " + to_wstring(count);
        printText.announcement(result);
        updateFrame();
    }
    if(userWantExit()){
        menu.reset();
        vList->resetVertexColor();
    }
}
int Program::tarjanAlgo(bool showResult, int remove){
    const int &NUM = vList->getSize();
	int disc[NUM];
	int low[NUM];
	setArrayTo(disc, NUM, 0);
	setArrayTo(trace, NUM, 1);
	int count(0), components(0);
	mstack s;
	if (remove != -1){
		disc[remove] = 1; // ignore remove
		trace[remove] = 0; // ignore remove
	}
	for (int i = 0; i < NUM; ++i)
		if (disc[i] == 0) tarjanVisit(i, disc, low, s, count, components, showResult);
	return components;
}
void Program::tarjanVisit(int u, int *disc, int *low, mstack &s, int &count, int &components, bool callTarjanResult){
	low[u] = disc[u] = ++count;
	s.push(u);
	int v;
	for (v = 0; v < vList->getSize(); ++v)
		if (trace[v] != 0 && graph[u][v] != NO_EDGE_VALUE)
			if (disc[v] != 0)
				low[u] = min(low[u], disc[v]);
			else
			{
				tarjanVisit(v, disc, low, s, count, components, callTarjanResult);
				low[u] = min(low[u], low[v]);
			}
	if (disc[u] == low[u]){
		++components;
		if (callTarjanResult)
			tarjanResult(s, u, components);
		else
			do{
				v = s.pop();
				trace[v] = 0;
			}while (v != u);
	}
}
void Program::tarjanResult(mstack &s, int stop, int components){
	int v;
	do{
		v = s.pop();
		trace[v] = 0;
        Color RandomColor(Color(rand()%255,rand()%255,rand()%255));//rgb random color
        vList->highlightVertex(v,VERTEX_MORE_COLOR[components]);
	} while (v != stop);
    updateFrame();
}
void Program::bridgeEdge(){
    if(emptyGraph())return;
    else{
        int SCC = tarjanAlgo(0,-1); // first, find current SCC
        int count(0);
        wstring output = L"Các cạnh cầu tìm được: ";
        for (int i = 0; i < vList->getSize(); ++i)
            for (int j = 0; j < vList->getSize(); ++j)
                if (graph[i][j] != NO_EDGE_VALUE){
                    float backup = graph[i][j]; // backup edge
                    graph[i][j] = NO_EDGE_VALUE; // Delete edge
                    int SCCAfterRemoveEdge = tarjanAlgo(0,-1); // recalculate SCC
                    graph[i][j] = backup; // restore
                    if (SCCAfterRemoveEdge > SCC) // is strong bridge
                    {
                        ++count;
                        vList->drawEdges(i,j,LINE_HIGHLIGHT_COLOR);
                        vList->Draw();
                        output+= L"("+vList->getName(i)+L","+vList->getName(j)+L"); ";
                    }
                }
        
        if(count==0){
            output.clear();
            output=L"Không tìm thấy cạnh cầu nào!";
        }
        else{
            output+=(L"\nTổng cộng: ")+to_wstring(count);
        }
        printText.showResult(output);
    }
}
void Program::cutVertex(){
    if(emptyGraph())return;
    else{
        int SCC = tarjanAlgo(0,-1);
        int count(0);
        wstring output = L"Các đỉnh trụ tìm được: ";
        for (int i = 0; i < vList->getSize(); ++i){
            int SCCAfterRemove = tarjanAlgo(0, i); //tarjan algorithm without show result & delete vertex i
            if (SCCAfterRemove > SCC) // is a cut vertex
            {
                vList->highlightVertex(i,VERTEX_VISTITED);
                output+= L""+vList->getName(i);
                output+= L", ";
                ++count;
            }
        }
        if(count==0){
            output.clear();
            output=L"Không tìm thấy đỉnh trụ nào!";
        }
        else{
            output.erase(output.end()-2);
            output+=L"\nTổng cộng: "+to_wstring(count);
        }
        printText.showResult(output);
        updateFrame();
    }
}
void Program::knotVertex(){
    if(emptyGraph())return;
    if(userWantExit()){menu.reset();*source=*dest=-1;}
    if(getKey(R)){*source=*dest=-1;}
    printText.announcement(L"Tìm kiếm đỉnh thắt giữa 2 đỉnh.\n\t-Chọn đỉnh đầu sau đó giữ chuột tại đỉnh kế để xem kết quả\n\t-Phím <R> để reset đỉnh đầu");
    if(getTwoVertex(source,dest)){
        int n = vList->getSize();
        int count = 0;
        wstring result = L"Các đỉnh thắt tìm thấy từ đỉnh (" + vList->getName(*source)+ L") đến đỉnh ("+ vList->getName(*dest)+ L") là:\n\t";
        if(*source!=-1&&*dest!=-1){
            if (graph[*source][*dest] != NO_EDGE_VALUE ||
            !dfsToCheckConnected(*source, *dest)){
                printText.announcement(L"Không có đỉnh thắt(Tồn tại đường đi trực tiếp/Không có đường đi nào)");
                updateFrame();
                return;
            }
            else{
                int** temp;
                for(int i = 0;i < n; i++){
                    temp = getTempGraph(i,n);//get temporary matrix size of n which i'th collumn & row was removed
                    if(!bfsToCheckKnot(*source,*dest,n,temp)){// no paths found between source & dest vertex
                        if(i!=*source && i!=*dest){//make sure i dont equal source & dest
                            count ++;// increase count of knot vertices by 1
                            result += vList->getName(i)+L", ";
                            vList->highlightVertex(i, VERTEX_VISTITED);
                        }
                    }
                }
                for(int i = 0; i<n ;i++)delete [] temp[i];
                    delete [] temp;// clean up memory
                if(count){
                    result += L"\nTổng cộng: " + to_string(count); 
                    printText.announcement(result);
                    vList->highlightVertex(*source,VERTEX_CHOOSE);
                    vList->highlightOutlineVertex(*source,VERTEX_OUTLINE_VISITED);
                }
                else{
                    vList->highlightVertex(*source,VERTEX_CHOOSE);
                    vList->highlightOutlineVertex(*source,VERTEX_OUTLINE_VISITED);
                    result = L"Không tìm thấy đỉnh thắt nào từ đỉnh ("+ vList->getName(*source)+ L") đến đỉnh ("+ vList->getName(*dest)+ L")";
                    printText.announcement(result);
                }
                updateFrame();
            }
            
        }
    }
}
int** Program::getTempGraph(int k, int n){
    int** newGraph;
    newGraph = new int*[n];
    for(int i = 0; i<n; i++)
        newGraph[i] = new int[n];//initialize memory
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            if(i==k){//don't get value of k'th collum & row
                newGraph[i][j] = NO_EDGE_VALUE;
                newGraph[j][i] = NO_EDGE_VALUE;
            }
            else newGraph[i][j] = graph[i][j];
        }
    }
    return newGraph;
}
bool Program::bfsToCheckKnot(int start,int end,int n, int **tmp){
    int notVisited[n];
    setArrayTo(notVisited,n,0);
    mqueue queue;
    notVisited[start] = 1;
    queue.push(start);
    while (!queue.isEmpty()){
        int s = queue.pop();
        for(int i = 0;i < n;i++){
            if((tmp[s][i]!=NO_EDGE_VALUE)&&(notVisited[i]==0)){
                queue.push(i);
                notVisited[i]=1;
                if(i == end)return true;
            }
        }
    }
    return false;    
}
bool Program::dfsToCheckConnected(int start, int stop){
    int n = vList->getSize();
	setArrayTo(trace, n, 0);
	mstack s;
	s.push(start);
	trace[start] = 1;
	int numOfCheckedVertex(0);
	while (!s.isEmpty()){
		start = s.pop();
		if (start == stop) return 1;
		++numOfCheckedVertex;
		for (int i = 0; i < n; ++i)
			if (trace[i] == 0 && graph[start][i]!=NO_EDGE_VALUE){
				trace[i] = 1;
				s.push(i);
			}
	}
	return (numOfCheckedVertex == n);
}
void Program::topoSort(){
    if(emptyGraph())return;
    else{
        printText.announcement(L"Đây là thuật toán Sắp xếp Tô-Pô \nHãy chọn một đỉnh bất kỳ.");
        updateFrame();
        vList->chooseVertex();
        if(start == -1){
            start = vList->getVertex();
        }
        if(start == -1){
            return;
        }
        topoSortStart(start);
        if(userWantExit()){
            menu.reset();
            start = -1 ;
        }
        start=-1;
    }
    
}
void Program::topoSortStart(int start){
	int const &NUM = vList->getSize();
	int degIn[NUM];
	int order[NUM];
	for(int i = 0; i<NUM; ++i){//calc deg of each vertices
	    degIn[i]=0;
	    for(int j = 0; j<NUM; ++j){
		if(graph[j][i]!= NO_EDGE_VALUE)++degIn[i];
	    }
	    if(degIn[i]==0)vList->highlightVertex(i,Color::Magenta);
	}
	if(!KahnAlgorithm_TopoSort(degIn,order)){
	    printText.announcement(L"Đồ thị tồn tại chu trình");
	    updateFrame();
	}
}
bool Program::KahnAlgorithm_TopoSort(int *degIn, int *order){
    int const &NUM = vList->getSize();
    mqueue q;
    for(int i = 0; i<NUM; ++i){
        if(degIn[i]==0){
            q.push(i);
        }
    }
    int u, count = 0;
    while(!q.isEmpty()){
        u = q.pop();
        order[count++]=u;
        for(int i = 0; i<NUM; ++i){
            if(graph[u][i]!=NO_EDGE_VALUE){
                if(--degIn[i]==0){
                    q.push(i);
                    bool *visited = new bool[NUM];
                    int *path = new int[NUM];
                    int path_index = 0;
                    findAllPossiblePath(i,start,visited,path,path_index);
                    delete[] visited;
                    delete[] path;
                }
            }
        }
    }
    return (count == NUM);
}
void Program::findAllPossiblePath(int u, int d, bool visited[], int path[], int &path_index){
    visited[u] = true;
    path[path_index] = u;
    path_index++;
    if(u==d)printPath(path,path_index);
    else{
        for(int i = 0; i<vList->getSize();i++){
            if(!visited[i]&&graph[u][i]!=NO_EDGE_VALUE)
                findAllPossiblePath(i,d,visited,path,path_index);
        }
    }
    path_index--;
    visited[u]=false;
}
void Program::printPath(int path[], int size){
    wstring result = L"Hoàn tất. Thứ tự sắp xếp Tô-Pô đến đỉnh của bạn được tìm thấy!";
    vList->highlightVertex(path[0],VERTEX_VISTITED);
    printText.announcement(result);
    updateFrame();
}

void Program::euler(){
    if(emptyGraph())return;
    else{
        switch (isEulerCircuit()){
            case false:{
                printText.announcement(L"Không có chu trình Euler nào. :(");
                updateFrame();
                break;
            }
            case true:{
                findEulerCycle();
                break;
            }
        }
    }
    return;
}
bool Program::isEulerCircuit(){
    if(isConnected() == false) {    //when graph is not connected
      return false;
    }
    const int NODE = vList->getSize();
    vector<int> inward(NODE, 0), outward(NODE, 0);
            
    for(int i = 0; i<NODE; i++) {
        int sum = 0;
        for(int j = 0; j<NODE; j++) {
            if(graph[i][j]) {
                inward[j]++;    //increase inward edge for destination vertex
                sum++;    //how many outward edge
            }
        }
        outward[i] = sum;
    }

    if(inward == outward)    //when number inward edges and outward edges for each node is same
        return true;
    return false;
}
bool Program::isConnected(){
    bool *vis = new bool[vList->getSize()];
    for(int u = 0; u< vList->getSize(); u++){
        for(int i = 0; i<vList->getSize();i++)
            vis[i]=false;
        traverse(u, vis);
        for(int i = 0; i<vList->getSize();i++){
            if(!vis[i])
                return false;
        }
    }
    return true;
}
void Program::traverse(int u , bool visited[]){
    visited[u]=true;
    for(int v = 0; v<vList->getSize();v++){
        if(graph[u][v]!=NO_EDGE_VALUE){
            if(!visited[v])
                traverse(v,visited);
        }
    }
}
void Program::findEulerCycle(){
    vector< vector<int> > adjGraph;
    //copy current graph to a temporary adj graph
    const int &num = vList->getSize();
    for(int i = 0; i<num; i++){
        vector<int>temp;
        for(int j = 0; j<num ; j++){
            if(graph[i][j] != NO_EDGE_VALUE){
                temp.push_back(j);
            }
        }
        adjGraph.push_back(temp);
    }
    //using hash map to store <index of vertex : num of edges>
    unordered_map<int,int> edgeCount;
    for(int i = 0; i<adjGraph.size(); i++){
        edgeCount[i] = adjGraph[i].size();
    }
    //if empty graph:
    if(!adjGraph.size())return;
    // Maintain a stack to keep vertices 
    mstack currPath;
    //circuit array to store the result
    vector<int> circuit;
    //start from any vertex:
    int currV = 0;
    currPath.push(0);
    while(!currPath.isEmpty()){
        if(edgeCount[currV]){
            currPath.push(currV);
            int nextV = adjGraph[currV].back();
            edgeCount[currV]--;
            adjGraph[currV].pop_back();
            currV = nextV;
        }
        else{
            circuit.push_back(currV);
            currV = currPath.pop();
        }
    }
    printEulerCycle(circuit);
    adjGraph.clear();
}
void Program::printEulerCycle(vector<int> circuit){
    wstring result = L"Có chu trình Euler: :)\n";
    for(int i = circuit.size()-1; i>=0; i--){
        result+= vList->getName(circuit[i])+L" -> ";
        if(userWantExit()){
            menu.reset();break;
        }
        if(circuit[i]==0){//highlight starting vertex
            vList->highlightVertex(0,Color::Red);
            vList->highlightOutlineVertex(0,Color::Blue);
        }
        else
            vList->highlightVertex(circuit[i],VERTEX_VISTITED);
        printText.showResult(result);
        sleep(seconds(0.5f));
        updateFrame();
        window->display();
    }
    vList->resetVertexColor();
}
void Program::hamilton(){
    if(emptyGraph())return;
    else{
        int path[vList->getSize()];
        setArrayTo(path,vList->getSize(),-1);
        path[0]=0;
        if(recursiveHamilton(path,1)==false){
            printText.announcement(L"Không tìm thấy chu trình Hamilton nào. :(");
            updateFrame();
            return;
        }
        printHamiltonCycle(path);
        return;
    }
}
bool Program::recursiveHamilton(int path[], int i){
    if(i == vList->getSize()){
        if(graph[ path[i-1] ][ path[0] ] != NO_EDGE_VALUE)
            return true;
        else    
            return false;
    }
    for(int v = 1; v<vList->getSize();v++){
        if(canBeAdded(v,path,i)){
            path[i]=v;
            if(recursiveHamilton(path, i+1) == true)
                return true;
            path[i]=-1;
        }
    }
    return false;
}
bool Program::canBeAdded(int v,int path[],int pos){
    if(graph[path[pos-1]][v]==NO_EDGE_VALUE)return false;
    for(int i = 0;i<pos;i++)
        if(path[i]==v)
            return false;
    return true;
}
void Program::printHamiltonCycle(int path[]){
    wstring result = L"Chu trình Hamilton:\n";
    for(int i = 0; i<vList->getSize(); i++){
        result+= vList->getName(path[i])+L"->";
        if(userWantExit()){
            menu.reset();break;
        }
        vList->highlightVertex(path[i],VERTEX_VISTITED);
        printText.showResult(result);
        sleep(seconds(0.5f));
        updateFrame();
        window->display();
    }
    result+= L""+vList->getName(path[0]);
    sleep(seconds(0.5f));
    printText.showResult(result);
    updateFrame();
    vList->resetVertexColor();
}
void Program::findShortestPath(){
    if(emptyGraph())return;
    if(userWantExit()){
        menu.reset();
        *source=*dest=-1;
    }
    if(getKey(R)){
        *source=*dest=-1;
    }
    printText.announcement(L"Tìm kiếm đường đi từ 1 đỉnh tới các đỉnh còn lại.\n\t-Phím <R> để reset đỉnh đầu");
    if(getTwoVertex(source,dest)){
        if(*source!=-1&&*dest!=-1){
            if(!BellmanFord()){
                printText.announcement(L"Đồ thị tồn tại chu trình âm.");
                updateFrame();
            }
        }
        return ;
    }
}
bool Program::BellmanFord(){
    const int &NUM = vList->getSize();
	int dist[NUM],truoc[NUM];
	setArrayTo(truoc, NUM, *source);
	for (int i = 0; i < NUM; ++i)
		dist[i] = (graph[*source][i] == NO_EDGE_VALUE) ? MAX_EDGE_VALUE : graph[*source][i];
	dist[*source] = 0;
	for (int k = 1; k < NUM - 1; ++k){
		bool stop = 1;
		for (int i = 0; i < NUM; ++i){
			if (i == *source) continue;
			for (int j = 0; j < NUM; ++j){
				if (i == j) continue;
				double weight = graph[j][i];
				if (weight != NO_EDGE_VALUE && dist[i] > dist[j] + weight && dist[j] != NO_EDGE_VALUE){
					dist[i] = dist[j] + weight;
					truoc[i] = j;
					stop = 0;
				}
			}
		}
		if (stop) break; // if not update any dist ==> stop
	}
	if (isNegativeCycle(dist)) return false;
	showShortestPath(dist,truoc);
	return true;
}
bool Program::isNegativeCycle(int *dist){
    const int &NUM = vList->getSize();
	for (int i = 0; i < NUM; ++i)
		for (int j = 0; j < NUM; ++j)
			if (graph[j][i] != NO_EDGE_VALUE && dist[j] != MAX_EDGE_VALUE && dist[i] > dist[j] + graph[j][i])
				return true;
	return false;
}
void Program::showShortestPath(int *dist, int *truoc){
    if(dist[*dest] == MAX_EDGE_VALUE||dist[*dest]>vList->getSize()*MAX_VALUE_PER_EDGE){
        printText.announcement(L"Không tồn tại đường đi!");
        updateFrame();
        return;
    }
    else{
        wstring result = L"Tổng trọng số: "+to_wstring(dist[*dest]);
        result+=L"\nĐường đi ngắn nhất:\n";
        mstack s;
        while(*source!=*dest){
            s.push(*dest);
            *dest = truoc[*dest];
        }
        s.push(*source);
        // show stack result:
        int v, u = s.pop();
        result+=L""+vList->getName(u);
        vList->highlightVertex(u,VERTEX_VISTITED);
        while(!s.isEmpty()){
            v=s.pop();
            result+=L"->"+vList->getName(v);
            vList->drawEdges(u,v,LINE_HIGHLIGHT_COLOR);
            vList->highlightVertex(v,VERTEX_VISTITED);
            u=v;
        }
        printText.announcement(result);
        vList->Draw();
    }
}
bool Program::getTwoVertex(int *start, int *stop){
    vList->chooseVertex();
    if(*start == -1){
        *start = vList->getVertex();
    }
    if(*start != -1 || *stop == *start){
        *stop = vList->getVertex();
    }
    if(*stop==-1||*start==-1||*start==*stop){
        return false;
    }
    return true;
}
void Program::mainMenu(){
    switch(cur_button){
        case EXPORT:                        return saveFile();                    
        case IMPORT:                        return loadFile();                    
        case ADD_VERTEX:                    return addVertex();               
        case DEL_VERTEX:                    return delVertex();               
        case ADD_EDGE:                      return addEdge();                   
        case RENAME:                        return Rename();                      
        case MOVE_VERTEX:                   return moveVertex();             
        case HELP:                          return Help();                          
        case DEL_ALL:                       return delAll();                     
        // case SPEED_DOWN:                    return menu.reset();//not done yet              
        // case SPEED_UP:                      return menu.reset();//not done yet                
        case FB:                            return contactMe();                     
        case DFS:                           return dfs();                       
        case BFS:                           return bfs();                       
        case STRONGLY_CONNECTED_COMPONENTS: return SCC();  
        case BRIDGE_EDGE:                   return bridgeEdge();             
        case CUT_VERTEX:                    return cutVertex();              
        case KNOT_VERTEX:                   return knotVertex();                           
        case TOPO_SORT:                     return topoSort();
        case HAMILTON:                      return hamilton();
        case SHORTEST_PATH:                 return findShortestPath();
        case EULER_PATH:                    return euler();
    };
}
Program::~Program(){
}

#endif
