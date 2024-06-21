#include<iostream>
#include<algorithm>
#include <string>
#include<iomanip>
#include <map>
#include<vector>
#include <cmath>
#include <fstream>
#include<set>
#include<stack>
#include<queue>
#include <list>
#include<cstring>
#include<unordered_set>
#include<unordered_map>
#include<utility>
#include<memory>

using namespace std;

void fcfs(vector<int>& arrTime,vector<int>& burstTime, vector<int>& finishTime, vector<int>& turnTime, vector<int>& waitTime) {
    int n = arrTime.size();
    finishTime.resize(n);
    turnTime.resize(n);
    waitTime.resize(n);

    vector<pair<pair<int,int>,int> > vp;
    for(int i=0;i<n;i++){
        vp.push_back(make_pair(make_pair(arrTime[i],i),burstTime[i]));
    }
    sort(vp.begin(),vp.end());
    map<int,int>mp;
    for(int i=0;i<n;i++){
        arrTime[i]=vp[i].first.first;
        burstTime[i]=vp[i].second;
        mp[i]=vp[i].first.second;
    }
    finishTime[mp[0]] = arrTime[0] + burstTime[0];
    for (int i = 1; i < n; ++i) {
        finishTime[mp[i]] = max(finishTime[mp[i - 1]], arrTime[i]) + burstTime[i];
    }

    for (int i = 0; i < n; ++i) {
        turnTime[mp[i]] = finishTime[mp[i]] - arrTime[i];
        waitTime[mp[i]] = turnTime[mp[i]] - burstTime[i];
    }
}
void SJF(vector<int>& arrTime,vector<int>& burstTime, vector<int>& finishTime, vector<int>& turnTime, vector<int>& waitTime){
    int n = arrTime.size();
    finishTime.resize(n);
    turnTime.resize(n);
    waitTime.resize(n);

    vector<pair<pair<int,int>,int> > vp;
    for(int i=0;i<n;i++){
        vp.push_back(make_pair(make_pair(arrTime[i],burstTime[i]),i));
    }
    sort(vp.begin(),vp.end());

    int currtime=vp[0].first.first;
    int completedprocess=0;
    int ind=0;
    set<pair<int,int> > sp;
    while(completedprocess<n){
        int q=ind;
        for(int i=ind;i<n;i++){
            if(currtime>=vp[i].first.first){
                q=i+1;
                sp.insert(make_pair(vp[i].first.second,vp[i].second));
            }
            else{
                if(sp.size()==0){
                    currtime=vp[i].first.first;
                    sp.insert(make_pair(vp[i].first.second,vp[i].second));
                    q=i+1;
                }
                else{
                    q=i;
                    break;
                }
            }
        }
        ind=q;
        auto it=sp.begin();
        finishTime[it->second]=currtime+it->first;
        currtime+=it->first;
        sp.erase(it);
        completedprocess++;
    }
    for (int i = 0; i < n; ++i) {
        turnTime[i] = finishTime[i] - arrTime[i];
        waitTime[i] = turnTime[i] - burstTime[i];
    }
}

void priority_Scheduling(vector<int>& arrTime,vector<int>& burstTime, vector<int>& priority, vector<int>& finishTime, vector<int>& turnTime, vector<int>& waitTime){
    int n = arrTime.size();
    finishTime.resize(n);
    turnTime.resize(n);
    waitTime.resize(n);

    vector<vector<int> > vv;
    for(int i=0;i<n;i++){
        vector<int> temp;
        temp.push_back(arrTime[i]);
        temp.push_back(i);
        temp.push_back(priority[i]);
        temp.push_back(burstTime[i]);
        vv.push_back(temp);
    }
    sort(vv.begin(),vv.end());

    int currtime=vv[0][0];
    int completedprocess=0;
    int ind=0;
    set<pair<pair<int,int>,int> >spp;
    while(completedprocess<n){
        int q=ind;
        for(int i=ind;i<n;i++){
            if(currtime>=vv[i][0]){
                q=i+1;
                spp.insert(make_pair(make_pair(-vv[i][2],vv[i][3]),vv[i][1]));
            }
            else{
                if(spp.size()==0){
                    currtime=vv[i][0];
                    spp.insert(make_pair(make_pair(-vv[i][2],vv[i][3]),vv[i][1]));
                    q=i+1;
                }
                else{
                    q=i;
                    break;
                }
            }
        }
        ind=q;
        auto it=spp.begin();
        finishTime[it->second]=currtime+it->first.second;
        currtime=currtime+it->first.second;
        spp.erase(it);
        completedprocess++;
    }
    for (int i = 0; i < n; ++i) {
        turnTime[i] = finishTime[i] - arrTime[i];
        waitTime[i] = turnTime[i] - burstTime[i];
    }
}
void RR(vector<int>& arrTime,vector<int>& burstTime,vector<int>& finishTime, vector<int>& turnTime, vector<int>& waitTime,int timeslice ){
    int n = arrTime.size();
    finishTime.resize(n);
    turnTime.resize(n);
    waitTime.resize(n);
    vector<int> remainTime=burstTime;

    vector<pair<pair<int,int>,int> > vp;
    for(int i=0;i<n;i++){
        vp.push_back(make_pair(make_pair(arrTime[i],i),burstTime[i]));
    }
    sort(vp.begin(),vp.end());

    queue<int> readyqueue;
    int currtime=vp[0].first.first,completedprocess=0;
    int ind=0;

    while(completedprocess<n){
        int q=ind;
        for(int i=ind;i<n;i++){
            if(vp[i].first.first<=currtime){
                q=i+1;
                readyqueue.push(vp[i].first.second);
            }
            else{
                if(readyqueue.empty()){
                    currtime=vp[i].first.first;
                    q=i+1;
                }
                else{
                    q=i;
                    break;
                }
            }
        }
        ind=q;
        queue<int> temp;
        while(!readyqueue.empty()){
            int p=readyqueue.front();
            readyqueue.pop();
            currtime+=min(timeslice,remainTime[p]);
            remainTime[p]-=min(timeslice,remainTime[p]);
            if(remainTime[p]==0){
                finishTime[p]=currtime;
                completedprocess++;
            }
            else{
                temp.push(p);
            }
        }
        readyqueue=temp;
    }
    for (int i = 0; i < n; ++i) {
        turnTime[i] = finishTime[i] - arrTime[i];
        waitTime[i] = turnTime[i] - burstTime[i];
    }

}
int main(){
    ifstream inputFile("input.txt"); 
    string line; 
    int p=0;
    int n;
    vector<int> arrTime;
    vector<int> burstTime;
    vector<int> priority;
    int timeslice;
    while (getline(inputFile, line)) { 
        if(p==0){
            n=stoi(line);
        }
        else if(p==4){
            timeslice=stoi(line);
        }
        else{
            int num=0;
            for(int i=0;i<line.length();i++){
                if(line.at(i)==' '){
                    if(p==1){
                        arrTime.push_back(num);
                    }
                    else if(p==2){
                        burstTime.push_back(num);
                    }
                    else if(p==3){
                        priority.push_back(num);
                    }
                    num=0;
                }
                else{
                num*=10;
                num+=(line.at(i)-48);
                }
                if(i==line.length()-1){
                    if(p==1){
                        arrTime.push_back(num);
                    }
                    else if(p==2){
                        burstTime.push_back(num);
                    }
                    else if(p==3){
                        priority.push_back(num);
                    }
                }

            }
        }
        p++;
    } 
    int sf=0,sff=0;
    for(int i=0;i<n;i++){
        sf+=arrTime[i];
    }
    for(int i=0;i<n;i++){
        sff+=burstTime[i];
    }
    double c1=(double)sf/(double)n;
    double c2=(double)sff/(double)n;
    inputFile.close();
    cout<<'\n';
    vector<int> finishTime;
    vector<int> turnTime;
    vector<int> waitTime;
    ofstream outputFile("output.txt");
    fcfs(arrTime,burstTime,finishTime,turnTime,waitTime);
    int s1=0;
    outputFile<<c1;
    outputFile<<'\n';
    outputFile<<c2;
    outputFile<<'\n';
    for(int i=0;i<n;i++){
        s1+=finishTime[i];
    }
    double a1=(double)s1/(double)n;
    outputFile<<a1;
    outputFile<<'\n';
    s1=0;
    for(int i=0;i<n;i++){
        s1+=turnTime[i];
    }
    a1=(double)s1/(double)n;
    outputFile<<a1;
    outputFile<<'\n';
    s1=0;
    for(int i=0;i<n;i++){
        s1+=waitTime[i];
    }
    a1=(double)s1/(double)n;
    outputFile<<a1;
    outputFile<<'\n';
    s1=0;
    SJF(arrTime,burstTime,finishTime,turnTime,waitTime);
    outputFile<<c1;
    outputFile<<'\n';
    outputFile<<c2;
    outputFile<<'\n';
    for(int i=0;i<n;i++){
        s1+=finishTime[i];
    }
    a1=(double)s1/(double)n;
    outputFile<<a1;
    outputFile<<'\n';
    s1=0;
    for(int i=0;i<n;i++){
        s1+=turnTime[i];
    }
    a1=(double)s1/(double)n;
    outputFile<<a1;
    outputFile<<'\n';
    s1=0;
    for(int i=0;i<n;i++){
        s1+=waitTime[i];
    }
    a1=(double)s1/(double)n;
    outputFile<<a1;
    outputFile<<'\n';
    s1=0;
    priority_Scheduling(arrTime,burstTime,priority,finishTime,turnTime,waitTime);
    outputFile<<c1;
    outputFile<<'\n';
    outputFile<<c2;
    outputFile<<'\n';
    for(int i=0;i<n;i++){
        s1+=finishTime[i];
    }
    a1=(double)s1/(double)n;
    outputFile<<a1;
    outputFile<<'\n';
    s1=0;
    for(int i=0;i<n;i++){
        s1+=turnTime[i];
    }
    a1=(double)s1/(double)n;
    outputFile<<a1;
    outputFile<<'\n';
    s1=0;
    for(int i=0;i<n;i++){
        s1+=waitTime[i];
    }
    a1=(double)s1/(double)n;
    outputFile<<a1;
    outputFile<<'\n';
    s1=0;
    RR(arrTime,burstTime,finishTime,turnTime,waitTime,timeslice);
    outputFile<<c1;
    outputFile<<'\n';
    outputFile<<c2;
    outputFile<<'\n';
    for(int i=0;i<n;i++){
        s1+=finishTime[i];
    }
    a1=(double)s1/(double)n;
    outputFile<<a1;
    outputFile<<'\n';
    s1=0;
    for(int i=0;i<n;i++){
        s1+=turnTime[i];
    }
    a1=(double)s1/(double)n;
    outputFile<<a1;
    outputFile<<'\n';
    s1=0;
    for(int i=0;i<n;i++){
        s1+=waitTime[i];
    }
    a1=(double)s1/(double)n;
    outputFile<<a1;
    outputFile<<'\n';
    s1=0;
    system("Desktop/CPU_scheduler_proj/page.html");
}