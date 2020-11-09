#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

#define INT_MAX 999999

void incrementPath(vector<int> &path){
    for(int i=1; i<path.size();i++){
        path[i]+=1;
    }
    path.push_back(0);

}


void tspDynamic(int &cost,vector<vector<int>> &previous,vector<vector<int>> &graph, int &n, vector<vector<int>> &dp, vector<int> &path ){

  for(int maska = 1 ; maska < (1 << n-1) ; maska++ ){
    for(int i=0 ; i<n ; i++){
      if (maska & (1 << i) ){ // jeżeli taki wierzchołek jest na trasie
        for(int temp =0 ; temp < n-1;temp++){ //szukamy wierzchołka którego jeszcze nie ma na trasie
          if(!(maska & (1<<temp))){ // jeżeli takeigo końcowego nie ma na trasie
            dp[maska|(1<<temp)][temp] = min(dp[maska][i] + graph[i+1][temp+1] , dp[maska|(1<<temp)][temp]);
            if(dp[maska|(1<<temp)][temp] == (dp[maska][i] + graph[i+1][temp+1])){ //jeżeli znależliśmy lepsze rozwiązanie należy zapisać w tablicy poprzedników
              previous[maska|(1<<temp)][temp] = i;
            }

          }
        }
      }
    }
  }


  //wyliczenie która ścieżka finalnie da najlepszy result
  int result = INT_MAX;
  int last;
  for (int v = 0; v < (n-1); ++v) {
    int temp = dp[(1 << (n-1)) - 1][v] + graph[v+1][0]; //koszt "powrotu" z wierzchołka v do wierzchołka 0
    if (result > temp) {
      result = temp;
      last = v;

    }
  }
  cout<<"WYNIK"<<result<<endl;

  //odnajdywanie ścieżki
  path.push_back(0);
  int previousMask = (1 << (n-1))-1; // maska dla całego cyklu
  while(previousMask>0){
    path.push_back(last); // dodanie do cyklu ostatnio dodanego wierzchołka
    int temp = previousMask;
    previousMask -= (1<<last); // odjęcie od ostaniej maski ostatnio dodany wierzchołek
    last = previous[temp][last]; // znalezeinie przed poprzednika wierzchołka

  }
  //inkrementacja każdej wartości ze ściezki
  incrementPath(path);

  //wyświetlenie ścieżki od końca do początku
    for(int i=path.size()-1 ; i>=0 ;i--){
      cout<<path[i]<<" ";
    }
    cout<<endl;
  }



//czytanie z pliku
void readFromFile(string s, vector<vector<int>> &graph, int &n){

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

}

void createVectors(vector<vector<int>> &previous, vector<vector<int>> &dp, int &n, vector<vector<int>> &graph)
{
    //wypełnienia wektora previous wektorami wraz z wartościami -1
    for (int i = 0; i < ((1 << (n-1))) ; i++) {
        previous.push_back(vector<int>());
        for (int j = 0; j <  n-1; j++) {
            previous[i].push_back(-1);
        }
    }
    //wypełnienia wektora dp wekorami wraz z wartości INT_MAX
    for (int i = 0; i < ((1 << (n-1))); i++) {
        dp.push_back(vector<int>());
        for (int j = 0; j < n-1; j++) {
            dp[i].push_back(INT_MAX);
        }
    }
    //wypełnienie tablicy dp kosztami przejścia od wierzchołka 0 do i
    for(int i=0;i<n-1;i++){
        dp[1<<i][i] = graph[0][i+1];
    }
}


int main() {

  int n, repeat, optimumCost,temp2;
  char bracket;
  string line, csvName, dataFile;
  vector<int> route;
  vector<int>solution;
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
      vector<vector<int>> graph;
      readFromFile(dataFile, graph, n); // wypełnienie zmiennych graph danymi
      for (int i = 0; i <= n; i++) { // wczytanie ścieżki z nawiasu kwadratowego
        stream >> temp2;
        solution.push_back(temp2);
      }
      vector<vector<int>> previous;
      vector<vector<int>> dp;
      for (int j = 0; j < repeat; j++) {
        createVectors(previous,dp,n,graph); //stworzenie wektorów previous oraz dp
        vector<int> result;
        int cost = 0;
        tspDynamic(cost, previous,graph,n, dp,result);
        /*csvFile.open(csvName,  std::ios::out |  std::ios::app);
        csvFile<<"Spodziewane wyniki dla pliku: "<<tsp(1, 0, cost, previous,graph,visited_all,n,dp)<<"\n";
        csvFile.close();
        result = getPath(previous);
        //cout<<"GETPATH SIZE"<<result.size();*/
      }

    }

  }return 0;
}


