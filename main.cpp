#include <iostream>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>
using namespace std;

#define INT_MAX 999999


int  tsp(int mask,int pos, int &cost,vector<vector<int>> &previous,vector<vector<int>> &graph, int &visited_all, int &n, vector<vector<int>> &dp ){

  if(mask==visited_all){
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

      int newAns = graph[pos][city] + tsp(mask|(1<<city), city,cost,previous,graph,visited_all,n,dp);

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

vector<int> getPath(vector<vector<int>> &previous){

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
void readFromFile(string s, vector<vector<int>> &graph, int &n, vector<vector<int>> &previous, vector<vector<int>> &dp){

  int weight;
  ifstream myFile;
  myFile.open(s);
  if(myFile.is_open()){
    myFile>>n;
    for(int i=0;i<n;i++){
        graph.push_back(vector<int>());
        for(int j=0; j < n; j++){
        myFile>>weight;
        graph[i].push_back(weight);
      }
    }
  }
  else{
    cout<<"File is not read properly";
  }
  myFile.close();
  for (int i = 0; i < n; i++) {
    previous.push_back(vector<int>());
    for (int j = 0; j < (1 << n); j++) {
      previous[i].push_back(-1);
    }
  }
  for (int i = 0; i < (1 << n); i++) {
    dp.push_back(vector<int>());
    for (int j = 0; j < n; j++) {
      dp[i].push_back(-1);
    }
  }
}



int main() {
  vector<vector<int>> previous;
  vector<vector<int>> graph;
  vector<vector<int>> dp;
  int n, cost, repeat, optimumCost,temp2;
  char bracket;
  string line, csvName, dataFile;
  vector<int> route;
  vector<int>solution;
  vector<int> result;


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
      readFromFile(dataFile, graph, n, previous, dp); // wypełnienie zmiennych graph oraz fullgraph danymi
      int visited_all = (1<<n) -1;
      for (int i = 0; i <= n; i++) { // wczytanie ścieżki z nawiasu kwadratowego
        stream >> temp2;
        solution.push_back(temp2);
      }
      for (int j = 0; j < repeat; j++) {
        cout << tsp(1, 0, cost, previous,graph,visited_all,n, dp);
        csvFile.open(csvName,  std::ios::out |  std::ios::app);
        csvFile<<"Spodziewane wyniki dla pliku: "<<tsp(1, 0, cost, previous,graph,visited_all,n,dp)<<"\n";
        csvFile.close();
        result = getPath(previous);
        //cout<<"GETPATH SIZE"<<result.size();

      }
      cout<<"ŚCIEŻKA"<<endl;
      for (int i = 0; i < result.size(); i++) {
        cout << result[i] << " ";
        cout << endl;
      }

  }}
  return 0;
}
