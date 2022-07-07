#ifndef GRAPH
#define GRAPH

#include "para.h"
#include "inst.h"
#include<algorithm>
#include <vector>
#include <list>
#include <queue>
#include <iostream>
#include <set>
using namespace std;
class Graph
{
private:
    static int edgeNum;
    static int lastTopId;

public:
    static int coreNum;
    static vector<pair<int,int>> G[maxInstNum];
    static vector<int> indegree;
    static vector<int> outdegree;
    static int redPrev[maxInstNum];

    static int instNum;
    static vector<vector<int>> instsWithTopId;
    static pair<int,int> edges[maxInstNum];
    static int coreEdgeNum;

    static vector<int> k;
    static vector<set<int>> coreEdge;
    static vector<list<int>> coreLine;

    static vector<vector<int>> allPaths;

    static int topId[maxInstNum];
    static void getInstNum(){
        instNum=Inst().instNum;
    }
    static void getEdgeNum(){
        cin>>edgeNum;
    }
    static void getcoreEdge(){
        cin>>coreNum;
        k.resize(coreNum);
        coreEdge.resize(coreNum);
        coreLine.resize(coreNum);
        for(int j=0;j<coreNum;++j){
            cin>>k[j];
            cin>>coreEdgeNum;
            int inst;
            for(int i=0;i<coreEdgeNum;++i){
                cin>>inst;
                coreEdge[j].insert(inst);
            }
            pair<int,int> firstEdge;
            for(auto id:coreEdge[j]){
                firstEdge=edges[id];
                coreEdge[j].erase(id);
                break;
            }
            coreLine[j].push_back(firstEdge.first);
            coreLine[j].push_back(firstEdge.second);
            while (!coreEdge[j].empty())
            {
                auto tmp=coreEdge[j];
                for(auto id:tmp){
                    if(edges[id].first==coreLine[j].back()){
                        coreLine[j].push_back(edges[id].second);
                        coreEdge[j].erase(id);
                    }
                    if(edges[id].second==coreLine[j].front()){
                        coreLine[j].push_front(edges[id].first);
                        coreEdge[j].erase(id);
                    }
                }
            }
        }

    }
    static void build(){
        indegree.resize(maxInstNum);
        outdegree.resize(maxInstNum);
        for(int i=0;i<edgeNum;++i){
            int edge,st,ed;
            cin>>edge>>st>>ed;
            if(edge){
                redPrev[ed]=st;
            }
            else{
                redPrev[ed]=-1;
            }
            G[st].push_back({ed,edge});
            edges[i]={st,ed};
            ++indegree[ed];
            ++outdegree[st];
        }
    }
    Graph(/* args */) {}
    ~Graph() {}
};
int Graph::instNum=0;
int Graph::edgeNum=0;
vector<pair<int,int>> Graph::G[maxInstNum]={};
vector<vector<int>> Graph::instsWithTopId={};

vector<vector<int>> Graph::allPaths;
int Graph::redPrev[maxInstNum]={};

int Graph::topId[maxInstNum]={};
int Graph::lastTopId=0;
pair<int,int> Graph::edges[maxInstNum]={};

int Graph::coreEdgeNum=0;
vector<set<int>> Graph::coreEdge={};
vector<list<int>> Graph::coreLine={};

vector<int> Graph::indegree={};
vector<int> Graph::outdegree={};
int Graph::coreNum=0;
vector<int> Graph::k={};

#endif