#ifndef WORKSHOP
#define WORKSHOP

#include "para.h"
#include <vector>
#include <iostream>
using namespace std;

class Workshop
{
private:
    static int shopNum;
    static int regionNum;
    static int type[maxShop];
public:
    static vector<int> regionsInShop[maxRegion];
    static void getShopNum(){cin>>shopNum;}
    static void getRegionNum(){cin>>regionNum;}
    static void getRegionInfo(){
        int shop;
        for(int i=0;i<regionNum;++i){
            cin>>shop>>type[i];
            regionsInShop[shop].push_back(i);
        }
    }
    static int getEnergyType(int i){return type[i];}
    Workshop(/* args */) {}
    ~Workshop() {}
};
int Workshop::shopNum=0;
int Workshop::regionNum=0;
vector<int> Workshop::regionsInShop[maxRegion]={};
int Workshop::type[maxShop]={};

#endif
