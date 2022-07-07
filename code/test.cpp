#include<bits/stdc++.h>
using namespace std;
int main(){
    vector<pair<int,int>> testVec;
    testVec.push_back({1,1});
    testVec.push_back({1,2});
    sort(testVec.begin(),testVec.end(),less<pair<int,int>>());
    int front=testVec.front().second;
    cout<<front;

}