#include <iostream>
#include <map>
#include <vector>
using namespace std;

#define INT_MAX 999999

const int n=4;
int dist[10][10] = {
    {0,20,42,25},
    {20,0,30,34},
    {42,30,0,10},
    {25,34,10,0}
};
int VISITED_ALL = (1<<n) -1;
//int previous[4][16];
int dp[16][4];


int  tsp(int mask,int pos, int &cost, int **previous){

  if(mask==VISITED_ALL){
    cout<<"Return dist[pos][0]"<<endl;
    return dist[pos][0];
  }
  if(dp[mask][pos]!=-1){
    cout<<"Return dp[mask][pos] mask: "<<mask<<endl;
    return dp[mask][pos];
  }

  int ans = INT_MAX;
  int index =-1;
  for(int city=0;city<n;city++){

    if((mask&(1<<city))==0){

      cout<<"Maska: "<<mask<<endl;

      int newAns = dist[pos][city] + tsp( mask|(1<<city), city,cost,previous);

      cout<<"New Ans"<<newAns<<"Dla pos "<<pos<<"city "<<city<<endl;
      cout<<"Poprzednie ans: "<<ans;

      if(newAns < ans){
        cost = newAns;
        index=city;
      }
      ans = min(ans, newAns);
      cout<<"Ans"<<ans<<" W iteracji dla city: "<<city<<"i pos:"<<pos<<endl;
      cout<<mask<<endl;
    }

  }
  cout<<"dopisuje sie do dp mask:" <<mask<<"pos: "<<pos<<"ans: "<<ans<<endl;
  cout<<"dopisuje sie do do previous mask:" <<mask<<"pos: "<<pos<<"index: "<<index<<endl;
  previous[pos][mask] = index;
  return dp[mask][pos] = ans;
}

vector<int> getPath(int **previous){

  int mask = 1;

  vector<int>resultPath;
  int index = 0;
  while(true){
    resultPath.push_back(index);
    int nextInt = previous[index][mask];
    if(nextInt == -1) {
      break;
    }
    int nextMask = mask | (1 << nextInt);
    mask= nextMask;
    index = nextInt;
  }
  resultPath.push_back(0);
  return resultPath;

}

int main(){
  int **previous;
  vector<int> route;
  vector<int> result;
  int cost;

  previous = new int *[n];
  for(int i=0;i<n;i++){
    previous[i] = new int [1<<n];
  }

  for(int i=0;i<(1<<n);i++){
    for(int j=0;j<n;j++){
      dp[i][j] = -1;
    }
  }

  for(int i=0;i<n;i++){
    for(int j=0;j<(1<<n);j++){
      previous[i][j] = -1;
    }
  }


  cout<<tsp(1,0,cost,previous);

  for(int i=0;i<n;i++){
    cout<<i<<" ";
    for(int j=0;j<(1<<n);j++){
      cout<<previous[i][j]<<" ";
    }
    cout<<endl;
  }
  return 0;
}
