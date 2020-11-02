#include <iostream>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
using namespace std;

#define INT_MAX 999999


void tsp(int mask,int pos, int &cost,vector<vector<int>> &previous,vector<vector<int>> &graph, int &visited_all, int &n, vector<vector<int>> &dp ){



  for(int maska = 1 ; maska < (1 << n-1) ; maska++ ){
    for(int i=0 ; i<n ; i++){
      if ((maska & (1 << i))){ // jeżeli taki wierzchołek nie ma na trasie
        //cout<<"Taki wierzchołek startowy i "<<i<<"jest na trasie"<<endl;
        for(int temp =0 ; temp < n-1;temp++){ //wierzchołek końcowy
          if(!(maska & (1<<temp))){ // jeżeli takeigo końcowego nie ma na trasie
            //cout<<"Takiego  końcowego nie ma na trasie "<<temp<<endl;
            dp[maska|(1<<temp)][temp] = std:: min(dp[maska][i] + graph[i+1][temp+1] , dp[maska|(1<<temp)][temp]);
            //cout<<dp[maska|(1<<temp)][temp]<<endl;
          }
        }
      }
    }
  }
  for (int i=1 ; i << ((1<<n-1)-1) ; i++){
    for(int j=0;j<n-1;j++){
      cout<<"dp od i "<<i<<"od j"<<j<<" "<<dp[i][j]<<" ";
    }
    cout<<endl;
  }

  unsigned result = 1e9;
  int last;
  for (unsigned v = 0; v < (n-1); ++v) {
    unsigned act = dp[(1 << (n-1)) - 1][v] + graph[v+1][0]; //koszt "powrotu" z wierzchołka v do wierzchołka 0
    if (result > act) {
      result = act;
      last = v;
    }
  }
  int nextmask = (1 << ((n-1)-1) ) - (int)pow(2,last);
  int max = INT_MAX, newIndex;
  for(int i=0; i<n-1 ;i++){
    if(dp[nextmask][i] < max){
      newIndex = i;
      max = dp[nextmask][i];
    }
  }
  cout<<"WYNIK"<<result<<endl<<endl;
  cout<<max;

}

//funkcja generująca ścieżkę z vectora previous
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
  for (int i = 0; i < ((1 << (n-1))); i++) {
    dp.push_back(vector<int>());
    cout<<i<<" ";
    for (int j = 0; j < n-1; j++) {
      dp[i].push_back(INT_MAX);
    }
  }
  for(int i=0;i<n-1;i++){
    dp[1<<i][i] = graph[0][i+1];
    cout<<dp[1<<i][i]<<endl;
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
        tsp(1, 0, cost, previous,graph,visited_all,n, dp);
        /*csvFile.open(csvName,  std::ios::out |  std::ios::app);
        csvFile<<"Spodziewane wyniki dla pliku: "<<tsp(1, 0, cost, previous,graph,visited_all,n,dp)<<"\n";
        csvFile.close();
        result = getPath(previous);
        //cout<<"GETPATH SIZE"<<result.size();*/

      }
      /*cout<<"ŚCIEŻKA"<<endl;
      for (int i = 0; i < result.size(); i++) {
        cout << result[i] << " ";
        cout << endl;*/
      }

  }return 0;
}


