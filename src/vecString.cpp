#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct minterm{
    int decimal;
    vector <char> binary;
    vector <int> g_int; // group int
    bool used = false;
};

int main(){

  vector<string> str_input;
  str_input.assign(1, "3 6 0 1 2 5 6 7");
  cout << int(str_input[0][0] - 48);
  int Numvariables; // variable개수
  int Numminterms; // minterm 개수
  minterm minterms[Numminterms]; // minterm들을 저장할 배열 선언

  Numvariables = int(str_input[0][0] - 48);

  cout << Numvariables << endl;
  cout << str_input[0].charAt(4) << endl;
  //cout << Numminterms << endl;
  /*
  for(int i=0; i<Numminterms; i++){ // 일단 입력받은 minterm들을 저장 완료
    minterms[i].decimal = int(str_input[0][4+(2*i)] - 48);
    minterms[i].g_int.push_back(minterms[i].decimal);
  }*/

/*
  for(int i=0; i<Numminterms; i++){
    cout << minterms[i].decimal << " ";
  }
*/
}
