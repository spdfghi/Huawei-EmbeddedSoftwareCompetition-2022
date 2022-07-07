#include "workshop.h"
#include "window.h"
#include "inst.h"
#include "graph.h"
#include "alloc.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    // freopen("case0.in","r",stdin);
    // freopen("out.out","w",stdout);
    auto workshop=Workshop();
    auto alloc=Alloc();
    auto window=Window();
    auto inst=Inst();
    auto graph=Graph();
    
    // alloc.getK();
    inst.getTime();

    workshop.getShopNum();
    workshop.getRegionNum();
    workshop.getRegionInfo();

    alloc.getCircleNum();
    alloc.getWindowNum();

    window.getNum();
    for(int i=0;i<window.windowNum;++i){
        windowList[i].getInfo();
    }

    inst.getNum();
    for(int i=0;i<inst.instNum;++i){
        instList[i].getInfo();
    }

    graph.getInstNum();
    graph.getEdgeNum();
    graph.build();
    graph.getcoreEdge();
    
    alloc.strategy();
    alloc.print();

    return 0;
}