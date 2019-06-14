#ifndef _helpers_hpp_
#define _helpers_hpp_
#include <math.h>
#include "globalVariables.hpp"

using namespace sf;

extern Vector2f normalize(Vector2f v){
    float d = sqrt(pow(v.x,2)+pow(v.y,2));
    v.x = 1.0/d*v.x;
    v.y = 1.0/d*v.y;
    return v;
}

extern float distance(Vector2f a, Vector2f b){
    return sqrt(pow((a.x-b.x),2)+pow((a.y-b.y),2));
}

float vector_length(Vector2f v){
	return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

template <typename Type>
void setArrayTo(Type *arr, int num, Type value){
	for (int i = 0; i < num; ++i) arr[i] = value;
}

bool getASCII(){
    return (e->text.unicode>=32&&e->text.unicode<=128);
}

bool getNum(){
    return (e->text.unicode>=48&&e->text.unicode<=57);
}

bool getKey(int Key){
    return (e->text.unicode==Key);
}

bool inWorkingZone(Vector2f Pos){ 
    if((WZ_LEFT<=Pos.x&&WZ_TOP<=Pos.y)&&(Pos.x<=WZ_RIGHT&&Pos.y<=WZ_BOTTOM)){
        return true;
    }
    return false;
}

bool getInputWeight(int w){
    return ((w>=MIN_VALUE_PER_EDGE)&&(w<=MAX_VALUE_PER_EDGE));
}

bool inputWeightIsSafe(string input){
    return (input!="--"&&input[1]!='-'&&input!="-");
}
#endif