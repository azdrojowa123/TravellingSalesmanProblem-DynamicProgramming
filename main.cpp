#include <iostream>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>
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
int dp[16][4];



int  tsp(int mask,int pos, int &cost, int **previous,int **graph){

  if(mask==VISITED_ALL){
    cout<<"Return dist[pos][0]"<<endl;
    return graph[pos][0];
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

      int newAns = dist[pos][city] + tsp( mask|(1<<city), city,cost,previous,graph);

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

//czytanie z pliku
void readFromFile(string s, int **graph, int &n){

  int weight;
  ifstream myFile;
  myFile.open(s);
  if(myFile.is_open()){
    myFile>>n;
    for(int i=0;i<n;i++){
        for(int j=0; j < n; j++){
        myFile>>weight;
        graph[i][j] = weight;
      }
    }
  }
  else{
    cout<<"File is not read properly";
  }
  myFile.close();
}



int main() {
  int **previous;
  int **graph;
  int n, cost, repeat, optimumCost,temp2;
  char bracket;
  string line, csvName, dataFile;
  vector<int> route;
  vector<int>solution;
  vector<int> result;

  previous = new int *[n];
  for (int i = 0; i < n; i++) {
    previous[i] = new int[1 << n];
  }

  graph = new int *[n];
  for (int i = 0; i < n; i++) {
    graph[i] = new int[1 << n];
  }

  for (int i = 0; i < (1 << n); i++) {
    for (int j = 0; j < n; j++) {
      dp[i][j] = -1;
    }
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < (1 << n); j++) {
      previous[i][j] = -1;
    }
  }

  ofstream csvFile;

  ifstream myInitFile;
  myInitFile.open("initialiaze.INI");

  // wyłuskanie z ostatniej linii pliku .INI nazwy pliku wyjściowego
  while (getline(myInitFile, line)) {
    csvName = line;
  }

  myInitFile.close();
  myInitFile.open("initialiaze.INI");

  // ponowne otwarcie pliku
  if (myInitFile.is_open()) {
    while (getline(myInitFile, line)) {             // wczytanie kolejnej lini
      if (line.find(".csv") != std::string::npos) { // sprawdzenie czy linia zawiera rozszerzenie .csv
        break;
      }
      istringstream stream(line);
      stream >> dataFile >> repeat >> optimumCost >>bracket; // wczytanie nazwy instancji, ilosci powtórzeń, kosztu optymalnego
      readFromFile(dataFile, graph, n); // wypełnienie zmiennych graph oraz fullgraph danymi
      for (int i = 0; i <= n; i++) { // wczytanie ścieżki z nawiasu kwadratowego
        stream >> temp2;
        solution.push_back(temp2);
      }
      for (int j = 0; j < repeat; j++) {
        cout << tsp(1, 0, cost, previous,graph);
      }

      for (int i = 0; i < n; i++) {
        cout << i << " ";
        for (int j = 0; j < (1 << n); j++) {
          cout << previous[i][j] << " ";
        }
        cout << endl;
      }

    }
  }
  return 0;
}
