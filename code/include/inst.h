#ifndef INST
#define INST

#include "para.h"
#include<iostream>
#include<limits>
using std::cin;
class Inst
{
private:
public:
    static int makeTime[maxEnergy];
    int id;
    int type;
    int cost[maxEnergy];
    static int instNum;
    static void getNum(){
        cin>>instNum;
    }
    void getInfo(){
        static int idx=0;
        id=idx++;
        cin>>type;
        for(int i=0;i<maxEnergy;++i){
            cin>>cost[i];
        }
    }
    static void getTime(){
        for(int i=0;i<maxEnergy;++i){
            cin>>makeTime[i];
        }
    }
    static int timeCost(int i){
        return makeTime[i];
    }
    
    // bool canSet(int type){return cost[type];}
    Inst(/* args */) {}
    ~Inst() {}
}instList[maxInstNum];
int Inst::makeTime[maxEnergy]={};
int Inst::instNum=0;

#endif