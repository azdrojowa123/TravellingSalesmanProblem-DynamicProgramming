#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <sstream>
#include <string>
using namespace std;

#define INT_MAX 999999


void tsp(int &cost,vector<vector<int>> &previous,vector<vector<int>> &graph, int &visited_all, int &n, vector<vector<int>> &dp, vector<int> &path ){

  for (int i = 0; i < ((1 << (n-1))); i++) {
    dp.push_back(vector<int>());
    for (int j = 0; j < n-1; j++) {
      dp[i].push_back(INT_MAX);
    }
  }

  for(int maska = 1 ; maska < (1 << n-1) ; maska++ ){
    for(int i=0 ; i<n ; i++){
      if ((maska & (1 << i))){ // jeżeli taki wierzchołek nie ma na trasie
        for(int temp =0 ; temp < n-1;temp++){ //wierzchołek końcowy
          if(!(maska & (1<<temp))){ // jeżeli takeigo końcowego nie ma na trasie
            dp[maska|(1<<temp)][temp] = std:: min(dp[maska][i] + graph[i+1][temp+1] , dp[maska|(1<<temp)][temp]);
            if(dp[maska|(1<<temp)][temp] == (dp[maska][i] + graph[i+1][temp+1])){
              previous[maska|(1<<temp)][temp] = i;
            }

          }
        }
      }
    }
  }


  //wyliczenie która ścieżka finalnie da najlepszy result
  unsigned result = INT_MAX;
  int last;
  for (unsigned v = 0; v < (n-1); ++v) {
    unsigned act = dp[(1 << (n-1)) - 1][v] + graph[v+1][0]; //koszt "powrotu" z wierzchołka v do wierzchołka 0
    if (result > act) {
      result = act;
      last = v;

    }
  }
  cout<<"WYNIK"<<result<<endl;

  //odnajdywanie ścieżki
  path.push_back(0);
  int previousMask = (1 << (n-1))-1;
  while(previousMask>0){
    cout<<last;
    path.push_back(last);
    int temp = previousMask;
    previousMask -= (1<<last);
    last = previous[temp][last];

  }
  for(int i=1; i<path.size();i++){
    path[i]+=1;
  }
  path.push_back(0);

  cout<<"PATH"<<result<<endl<<endl;
    for(int i=path.size()-1 ; i>=0 ;i--){
      cout<<path[i]<<" ";
    }
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
  for (int i = 0; i < ((1 << (n-1))) ; i++) {
    previous.push_back(vector<int>());
    for (int j = 0; j <  n-1; j++) {
      previous[i].push_back(-1);
    }
  }
  for (int i = 0; i < ((1 << (n-1))); i++) {
    dp.push_back(vector<int>());
    for (int j = 0; j < n-1; j++) {
      dp[i].push_back(INT_MAX);
    }
  }
  for(int i=0;i<n-1;i++){
    dp[1<<i][i] = graph[0][i+1];
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
        tsp(cost, previous,graph,visited_all,n, dp,result);
        /*csvFile.open(csvName,  std::ios::out |  std::ios::app);
        csvFile<<"Spodziewane wyniki dla pliku: "<<tsp(1, 0, cost, previous,graph,visited_all,n,dp)<<"\n";
        csvFile.close();
        result = getPath(previous);
        //cout<<"GETPATH SIZE"<<result.size();*/

      }
    }

  }return 0;
}


