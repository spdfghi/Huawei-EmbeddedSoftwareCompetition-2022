#ifndef WINDOW
#define WINDOW

#include "para.h"
#include "inst.h"
#include<iostream>
using std::cin;
using std::cout;
class Window
{
private:
    int preMake[maxPreMake];
public:
    int coef;
    bool canSelfRepeat;
    int workshopId;
    static int windowNum;
    static void getNum(){cin>>windowNum;}
    bool canPre(int id){
        return preMake[instList[id].type];
    }
    void getInfo(){
        cin>>canSelfRepeat>>workshopId>>coef;
        for(int i=0;i<maxPreMake;++i){
            cin>>preMake[i];
        }
    }
    void print(){
        for(int i=0;i<maxPreMake;++i){
            cout<<preMake[i];
        }
        cout<<'\n';

    }
    Window(/* args */) {}
    ~Window() {}
}windowList[maxWinNum];
int Window::windowNum=0;

#endif
