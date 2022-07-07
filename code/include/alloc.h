#include "inst.h"
#include "window.h"
#include "workshop.h"
#include "graph.h"
#include "algorithm"
#include "unordered_map"
#include "unordered_set"
#include<iostream>
#include<set>
using namespace std;

typedef pair<int,int> PII;

class Alloc
{
private:
    static const Workshop shop;
    static const Inst inst;
    static const Graph graph;
    static const Window window;

    static bool legal;
    static int lastCost;
    static int maxCircle;
    static int firstTypeWindow;

    static set<int> coreInsts;
    static vector<int> flattenWindows;
    static vector<unordered_map<int,int>> instFwRegion;
    static unordered_map<int,PII> windowCountTime;
    
    static int threshold;
    static vector<vector<int>> prevMax;
    static vector<vector<int>> nextMin;
    static vector<int> currAlloc,retAlloc;

    static int k;
    static set<int> allCoreInsts;

    void init(){
        int n=inst.instNum;
        retAlloc.resize(n);
        instFwRegion.resize(n);
        prevMax.resize(n);
        nextMin.resize(n);
        currAlloc.resize(n);

        getCoreInsts();
        getFlatternWindows();
        getInstFwRegion();
    }
    void clear(){
        int n=inst.instNum;
        prevMax.clear();
        nextMin.clear();
        currAlloc.clear();

        prevMax.resize(n);
        nextMin.resize(n);
        currAlloc.resize(n);
        
        decltype(windowCountTime)().swap(windowCountTime);
    }
 
    static void getCoreInsts(){
        auto ks=graph.k;
        int maxKvalue=0;
        int maxKid=0;
        for(int i=0;i<ks.size();++i){
            if(ks[i]>maxKvalue){
                maxKvalue=ks[i];
                maxKid=i;
            }
        }
        for(auto &&elt:graph.coreLine[maxKid]){
            coreInsts.insert(elt);
        }

        for(auto &&core:graph.coreLine){
            for(auto &&elt:core){
                allCoreInsts.insert(elt);
            }
        }
    }
    
    static bool isCoreInst(int instId){
        return allCoreInsts.count(instId);
    }

    static void getFlatternWindows(){
        for(int i=0;i<maxCircle;++i){
            for(int j=0;j<firstTypeWindow;++j){
                flattenWindows.push_back(j);
            }
        }

        for(int j=firstTypeWindow;j<window.windowNum;++j){
            if(windowList[j].canSelfRepeat){
                for(int i=0;i<maxCircle;++i){
                    flattenWindows.push_back(j);
                }
            }
            else{
                flattenWindows.push_back(j);
            }
        }

    }
    
    static void getInstFwRegion(){
        for(int i=0;i<inst.instNum;++i){
            for (int j=0;j<flattenWindows.size();++j)
            {
                int wId=flattenWindows[j];
                auto &window=windowList[wId];
                if(isCoreInst(i)&&!window.canPre(i)){
                    continue;
                }

                int shopId=windowList[wId].workshopId;
                int minCost=INT32_MAX;
                int minR=-1;
                for(auto r:shop.regionsInShop[shopId]){
                    int fee=instList[i].cost[shop.getEnergyType(r)];
                    if(fee&&fee<minCost){
                        minCost=fee;
                        minR=r;
                    }
                }
                if(minR>=0){
                    instFwRegion[i][j]=minR;
                }
            }
            
        }
    }

    static int getEnergyType(int fwId,int instId){
        int region=instFwRegion[instId][fwId];
        return shop.getEnergyType(region);
    }
    
    static int getRegionCost(int fwId,int instId){
        int region=instFwRegion[instId][fwId];
        int energyType=shop.getEnergyType(region);
        return instList[instId].cost[energyType];
    }
    static int getTotalCost(int fwId,int instId){
        int cost=getRegionCost(fwId,instId);
        if(isCoreInst(instId)){
            int window=flattenWindows[fwId];
            int coef=windowList[window].coef;
            if(!windowCountTime.count(window)){
                cost+=inst.timeCost(getEnergyType(fwId,instId))*(k+coef);
                return cost;
            }
            if(graph.redPrev[instId]!=-1){
                int prev=graph.redPrev[instId];
                int prevWin=flattenWindows[currAlloc[prev]];
                if(prevWin==window){
                    return cost;

                }
            }
            int time=windowCountTime[window].second;
            time=max(time,inst.timeCost(getEnergyType(fwId,instId)));
            cost+=time*k;
        }
        return cost;
    }
    static void setAlloc(int fwId,int instId){
        currAlloc[instId]=fwId;
        if(coreInsts.count(instId)){
            int window=flattenWindows[fwId];
            int time=inst.timeCost(getEnergyType(fwId,instId));
            auto &tmp=windowCountTime[window];
            ++tmp.first;
            if(graph.redPrev[instId]!=-1){
                int prev=graph.redPrev[instId];
                int prevWin=flattenWindows[currAlloc[prev]];
                if(prevWin==window){
                    --tmp.first;
                }
            }
            tmp.second=tmp.first==1?time:max(tmp.second,time);
        }
    }
    static int searchForwardIndex(int maxPrev,int instId){
        vector<int> possIds;
        for(int i=maxPrev+1;i<flattenWindows.size();++i){
            int window=flattenWindows[i];
            if(instFwRegion[instId].count(i)){
                possIds.push_back(i);
                if(possIds.size()>=threshold) break;
            }
        }
        int bestIndex;
        int minCost=INT32_MAX;
        if(possIds.empty()) legal=false;

        for(auto i:possIds){
            int currCost=getTotalCost(i,instId);
            if(currCost<minCost){
                minCost=currCost;
                bestIndex=i;
            }
        }
        setAlloc(bestIndex,instId);
        return bestIndex;
    }   
    static int searchBackIndex(int maxPrev,int minNext,int instId){
        vector<int> possIds;
        for(int i=minNext-1;i>maxPrev;--i){
            int window=flattenWindows[i];
            if(instFwRegion[instId].count(i)){
                possIds.push_back(i);
                if(possIds.size()>=threshold) break;
            }
        }
        int bestIndex;
        int minCost=INT32_MAX;
        for(auto i:possIds){
            int currCost=getTotalCost(i,instId);//在窗口处的最小消耗
            if(currCost<minCost){
                minCost=currCost;
                bestIndex=i;
            }
        }
        setAlloc(bestIndex,instId);
        return bestIndex;
    }

    static void allocate(){
        auto instNum=graph.instNum;
        auto &G=graph.G;
        auto indegree=graph.indegree;

        queue<int> q;
        for(int i=0;i<instNum;++i){
            if(indegree[i]==0){
                q.push(i);
            }
        }

        while(!q.empty()){
            auto front=q.front();
            auto &tmpVec=prevMax[front];

            int maxPrev=tmpVec.empty()?-1:*max_element(tmpVec.begin(),tmpVec.end());
            int maxCurr=searchForwardIndex(maxPrev,front);

            q.pop();

            for(auto p:G[front]){
                auto adj=p.first;
                auto edge=p.second;
                if(edge){
                    prevMax[adj].push_back(maxCurr-1);
                }
                else{
                    prevMax[adj].push_back(maxCurr);
                }
                if(!--indegree[adj]){
                    q.push(adj);
                }
            }
        }
    }
    
    static void backOpt(){
        auto instNum=graph.instNum;
        auto &G=graph.G;
        auto outdegree=graph.outdegree;

        queue<int> q;
        for(int i=0;i<instNum;++i){
            if(outdegree[i]==0){
                q.push(i);
            }
        }

        while(!q.empty()){
            auto front=q.front();
            auto &vecPrev=prevMax[front];
            auto &vecNext=nextMin[front];

            int maxPrev=vecPrev.empty()?-1:*max_element(vecPrev.begin(),vecPrev.end());
            int minNext=vecNext.empty()?flattenWindows.size():*min_element(vecNext.begin(),vecNext.end());

            int bestCurr=searchBackIndex(maxPrev,minNext,front);//flatten数组的下标
            q.pop();

            for(auto p:G[front]){
                auto adj=p.first;
                auto edge=p.second;
                if(edge){
                    nextMin[adj].push_back(bestCurr+1);
                }
                else{
                    nextMin[adj].push_back(bestCurr);
                }
                if(!--outdegree[adj]){
                    q.push(adj);
                }
            }
        }
    }

    static void refreshAlloc(){
        int currCost=0;
        for(int instId=0;instId<currAlloc.size();++instId){
            int fwId=currAlloc[instId];
            currCost+=getRegionCost(fwId,instId);
        }
        for(auto &&elt:windowCountTime){
            int window=elt.first;
            int coef=windowList[window].coef;
            int cnt=elt.second.first;
            int time=elt.second.second;
            currCost+=time*(cnt*k+coef);
        }
        if(currCost<lastCost) retAlloc=currAlloc;
    }

public:
    static void getCircleNum(){cin>>maxCircle;++maxCircle;}
    static void getWindowNum(){cin>>firstTypeWindow;}
    
    void strategy(){
        init();
        for(threshold=1;threshold<9;++threshold){
            allocate();
            backOpt();
            if(legal){//主要控制的部分
                refreshAlloc();
                retAlloc=currAlloc;
                clear();
            }
            else {
                break;
            }
        }
    }

    static void print(){
        int n=inst.instNum;
        cout<<n<<endl;

        int fWId,window,region;
        for(int i=0;i<n;++i){
            fWId=retAlloc[i];
            region=instFwRegion[i][fWId];
            cout<<region;
            cout<<(i<n-1?" ":"\n");
        }
        
        int coreNum=graph.coreNum;
        cout<<coreNum<<endl;
        for(int i=0;i<n;++i){
            auto const &tmp=graph.coreLine[i];
            vector<int> coreLine(tmp.begin(),tmp.end());
            int m=coreLine.size();
            cout<<m<<" ";
            for(int i=0;i<m;++i){
                fWId=retAlloc[coreLine[i]];
                window=flattenWindows[fWId];
                cout<<window;
                cout<<(i<m-1?" ":"\n");
            }
        }
    }

    Alloc(/* args */) {}
    ~Alloc() {}
};
int Alloc::maxCircle=0;
int Alloc::firstTypeWindow=0;
vector<int> Alloc::currAlloc={},Alloc::retAlloc={};
vector<vector<int>> Alloc::prevMax={};
vector<vector<int>> Alloc::nextMin={};

set<int> Alloc::coreInsts={};
set<int> Alloc::allCoreInsts={};
int Alloc::k=0;

int Alloc::threshold=8;
vector<int> Alloc::flattenWindows={};
vector<unordered_map<int,int>> Alloc::instFwRegion={};
unordered_map<int,PII> Alloc::windowCountTime={};
bool Alloc::legal=true;
int Alloc::lastCost=INT32_MAX;
int Alloc::k=0;

