#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;

// 가능한 변수의 개수는 최대 6개
struct minterm{
    int decimal;
    vector <char> binary;
    vector <int> g_int; // group int
    bool used = false;
};

vector < vector<minterm> > processing(vector < vector<minterm> > &p_groups, vector <vector<minterm> > final_groups, int Numvariables){
  for(int i=0; i<p_groups.size()-1; i++){ //행
    for(int j=0; j<p_groups[i].size(); j++){ //열
        for(int l=0; l<p_groups[i+1].size(); l++){
          int diff = Numvariables;
          int tmp = -1;
          for(int k=0; k<Numvariables; k++){ // minterm내부 bin
            if(p_groups[i][j].binary[k] == p_groups[i+1][l].binary[k]){diff-=1;}
          }
          if(diff == 1){ // 한자리만 차이나는 경우에
            for(int k=0; k<Numvariables; k++){
              if(p_groups[i][j].binary[k] != p_groups[i+1][l].binary[k]){
                  final_groups[i].push_back(p_groups[i][j]); // p_groups에 원래 있던 행의 minterm을 넣고
                  p_groups[i][j].used = true;
                  p_groups[i+1][l].used = true;
                  tmp = k;
              }
            }
            final_groups[i][final_groups[i].size()-1].binary[tmp] = '-';
            final_groups[i][final_groups[i].size()-1].used = false;
            //중복된 경우 제거
            vector <char> tmpv(final_groups[i][final_groups[i].size()-1].binary);
            int same=0;
            for(int k=0; k<final_groups[i].size(); k++){
              if(final_groups[i][k].binary == tmpv){
                same++;
              }
            }
            if(same==2){
              final_groups[i].pop_back();
            }
            else{
            final_groups[i][final_groups[i].size()-1].g_int.insert(final_groups[i][final_groups[i].size()-1].g_int.end(), p_groups[i+1][l].g_int.begin(), p_groups[i+1][l].g_int.end());
            }
          }
        }
    }
  }
  return final_groups;
}

void appendpi(vector<minterm> &pi, vector<vector<minterm> > p_groups){ // pi배열에 추가
  for(int i=0; i<p_groups.size();i++){
    for(int j=0; j<p_groups[i].size(); j++){
      if(p_groups[i][j].used == false){
        pi.push_back(p_groups[i][j]);
      }
    }
  }
}

void print_table(bool further_processing, vector<vector<minterm> > final_groups){
  if(further_processing){
    for(int i=0; i<final_groups.size(); i++){
      //cout << groups[i].size() << " ";
      for(int j=0; j<final_groups[i].size(); j++){
        //cout << groups[i][j].g_int.size() << " ";

        cout << "final_group" << i << ": (";
        for(int k=0; k<final_groups[i][j].g_int.size(); k++){
          cout << final_groups[i][j].g_int[k] << ", ";
        }
          cout << ")";
          //cout << "group" << i << ": " << groups[i][j].decimal << " ";
        for(int k=0; k<final_groups[i][j].binary.size(); k++){
          cout << final_groups[i][j].binary[k];
        }
        cout << " " << final_groups[i][j].used << " ";
        cout << endl;
      }
    }
    cout << endl;
  }
}

int convert(vector<char> binary, int Numv){
  int result = 0;
  for(int i=0; i<binary.size(); i++){
    switch(binary[i]){
      case '-':
        result += pow(10, Numv-i-1)*2;
        break;
      case '1':
        result += pow(10, Numv-i-1)*1;
        break;
      case '0':
        result +=0 ;
        break;
    }
  }
  return result;
}

bool cmp_minterm(minterm a, minterm b){return a.decimal<b.decimal;}

int main(){

  int Numvariables; // variable개수
  int Numminterms; // minterm 개수

  cin >> Numvariables >> Numminterms;

  minterm minterms[Numminterms]; // minterm들을 저장할 배열 선언

  for(int i=0; i<Numminterms; i++){ // 일단 입력받은 minterm들을 저장 완료
    cin >> minterms[i].decimal;
    minterms[i].g_int.push_back(minterms[i].decimal);
  }

  // 각 minterm들을 2진수로 변환해 구조체 내부 binary배열에 저장
  for(int i=0; i<Numminterms; i++){
    for(int j = Numvariables-1; j>=0; --j){
      minterms[i].binary.push_back( char( ((minterms[i].decimal >> j)&1) + 48) );
    }
  }

  // Grouping을 해서 저장할 배열을 만든다.
  //int group[Numvariables+1][Numminterms];
  bool further_processing1 = false;
  bool further_processing2 = false;
  bool further_processing3 = false;
  bool further_processing4 = false;
  bool further_processing5 = false;
  vector < vector<minterm> > groups(Numvariables+1); // groups[Numvariables+1][];
  vector < vector<minterm> > p_groups(Numvariables); // p_groups[Numvariables][]; 1회 프로세싱 후
  vector < vector<minterm> > final_groups(Numvariables); // final_groups[Numvariables-1][]; 2회 프로세싱 후
  vector < vector<minterm> > final_groups_2nd(Numvariables); // final_groups_2nd[Numvariables-2][]; 3회 프로세싱 후
  vector < vector<minterm> > final_groups_3nd(Numvariables); // final_groups_2nd[Numvariables-2][]; 3회 프로세싱 후
  vector < vector<minterm> > final_groups_4nd(Numvariables); // final_groups_2nd[Numvariables-2][]; 3회 프로세싱 후

  for(int i=0; i<Numminterms; i++){ // minterm들 각각을 접근한다.
    int count = 0;
    for(int j=0; j<Numvariables; j++){ // minterm내부의 binary접근
      if(minterms[i].binary[j] == '1'){count++;} // binary에서 1의 개수를 센다
    }
    groups[count].push_back(minterms[i]); // 1의 개수만큼 groups의 열에 들어간다.
  }

  //groups to p_groups가 필요한지 판별
  for(int i=0; i<groups.size()-1; i++){ //행
    for(int j=0; j<groups[i].size(); j++){ //열
        for(int l=0; l<groups[i+1].size(); l++){
          int diff = Numvariables;
          int tmp = -1;
          for(int k=0; k<Numvariables; k++){ // minterm내부 bin
            if(groups[i][j].binary[k] == groups[i+1][l].binary[k]){diff-=1;}
          }
          if(diff == 1){ // 한자리만 차이나는 경우에
            further_processing1 = true;
            break;
          }
        }
    }
  }

  //groups to p_groups 진행
  if(further_processing1){
    for(int i=0; i<groups.size()-1; i++){ //행
      for(int j=0; j<groups[i].size(); j++){ //열
          for(int l=0; l<groups[i+1].size(); l++){
            int diff = Numvariables;
            int tmp = -1;
            for(int k=0; k<Numvariables; k++){ // minterm내부 bin
              if(groups[i][j].binary[k] == groups[i+1][l].binary[k]){diff-=1;}
            }
            if(diff == 1){ // 한자리만 차이나는 경우에
              for(int k=0; k<Numvariables; k++){
                if(groups[i][j].binary[k] != groups[i+1][l].binary[k]){
                    p_groups[i].push_back(groups[i][j]); // p_groups에 원래 있던 행의 minterm을 넣고
                    //groups[i][j].used = true;
                    //groups[i+1][l].used = true;
                    tmp = k;
                }
              }
              p_groups[i][p_groups[i].size()-1].binary[tmp] = '-';
              p_groups[i][p_groups[i].size()-1].g_int.push_back(groups[i+1][l].decimal);
            }
          }
      }
    }
  }

  // processing 2회차 p_group to final_groups가 필요한지 판별
  for(int i=0; i<p_groups.size()-1; i++){ //행
    for(int j=0; j<p_groups[i].size(); j++){ //열
        for(int l=0; l<p_groups[i+1].size(); l++){
          int diff = Numvariables;
          int tmp = -1;
          for(int k=0; k<Numvariables; k++){ // minterm내부 bin
            if(p_groups[i][j].binary[k] == p_groups[i+1][l].binary[k]){diff-=1;}
          }
          if(diff == 1){ // 한자리만 차이나는 경우에
            further_processing2 = true;
            break;
          }
        }
    }
  }

  // processing 2회차 p_group to final_groups
  if(further_processing2){
    final_groups = processing(p_groups,final_groups,Numvariables);
  }

  // processing 3회차 final_groups to final_groups_2nd 필요한지 확인
  for(int i=0; i<final_groups.size()-1; i++){ //행
    for(int j=0; j<final_groups[i].size(); j++){ //열
        for(int l=0; l<final_groups[i+1].size(); l++){
          int diff = Numvariables;
          int tmp = -1;
          for(int k=0; k<Numvariables; k++){ // minterm내부 bin
            if(final_groups[i][j].binary[k] == final_groups[i+1][l].binary[k]){diff-=1;}
          }
          if(diff == 1){ // 한자리만 차이나는 경우에
            further_processing3 = true;
            break;
          }
        }
    }
  }

  // processing 3회차 final_groups to final_groups_2nd
  if(further_processing3){
    final_groups_2nd = processing(final_groups, final_groups_2nd, Numvariables);
  }

  // processing 4회차 final_groups_2nd to final_groups_3nd 필요한지 확인
  for(int i=0; i<final_groups_2nd.size()-1; i++){ //행
    for(int j=0; j<final_groups_2nd[i].size(); j++){ //열
        for(int l=0; l<final_groups_2nd[i+1].size(); l++){
          int diff = Numvariables;
          int tmp = -1;
          for(int k=0; k<Numvariables; k++){ // minterm내부 bin
            if(final_groups_2nd[i][j].binary[k] == final_groups_2nd[i+1][l].binary[k]){diff-=1;}
          }
          if(diff == 1){ // 한자리만 차이나는 경우에
            further_processing4 = true;
            break;
          }
        }
    }
  }

  // processing 4회차
  if(further_processing4){
    final_groups_3nd = processing(final_groups_2nd, final_groups_3nd, Numvariables);
  }

  // processing 5회차 final_groups_3nd to final_groups_4nd 필요한지 확인
  for(int i=0; i<final_groups_3nd.size()-1; i++){ //행
    for(int j=0; j<final_groups_3nd[i].size(); j++){ //열
        for(int l=0; l<final_groups_3nd[i+1].size(); l++){
          int diff = Numvariables;
          int tmp = -1;
          for(int k=0; k<Numvariables; k++){ // minterm내부 bin
            if(final_groups_3nd[i][j].binary[k] == final_groups_3nd[i+1][l].binary[k]){diff-=1;}
          }
          if(diff == 1){ // 한자리만 차이나는 경우에
            further_processing5 = true;
            break;
          }
        }
    }
  }

  // processing 5회차
  if(further_processing5){
    final_groups_4nd = processing(final_groups_3nd, final_groups_4nd, Numvariables);
  }
  /*
  //groups 내부 저장된것 확인
  print_table(further_processing1, p_groups);
  //final_groups 내부 저장된것 확인
  print_table(further_processing2, final_groups);
  //final_groups_2nd 내부 저장된것 확인
  print_table(further_processing3, final_groups_2nd);
  //final_groups_3nd 내부 저장된것 확인
  print_table(further_processing4, final_groups_3nd);
  //final_groups_4nd 내부 저장된것 확인
  print_table(further_processing5, final_groups_4nd);
  */
  vector<minterm> pi; // prime implicants

  appendpi(pi, p_groups);
  appendpi(pi, final_groups);
  appendpi(pi, final_groups_2nd);

  // NEPI와 EPI 구분
  int check[Numminterms] = {0}; // 각 minterm의 개수를 센다
  int minterm_decimals[Numminterms];
  bool checkepi[pi.size()] = {false}; // pi에서 epi인 index check
  vector <minterm> epi;
  vector <minterm> nepi;

  for(int i=0; i<Numminterms; i++){
     minterm_decimals[i] = minterms[i].decimal;
  }
  sort(minterm_decimals, minterm_decimals+Numminterms);

  for(int i=0; i<pi.size(); i++){
    for(int j=0; j<pi[i].g_int.size(); j++){
      for(int k=0; k<Numminterms; k++){
        if(minterm_decimals[k] == pi[i].g_int[j]) check[k]++;
      }
    }
  }

  for(int i=0; i<Numminterms; i++){
    if(check[i] == 1){
      for(int j=0; j<pi.size(); j++){
        for(int k=0; k<pi[j].g_int.size(); k++){
          if(pi[j].g_int[k] == minterm_decimals[i]){
            checkepi[j] = true;
          }
        }
      }
    }
  }

  for(int i=0; i<pi.size(); i++){
    if(checkepi[i]){
      epi.push_back(pi[i]); // EPI인 minterm저장
    }
  }

  for(int i=0; i<epi.size(); i++){
    epi[i].decimal = convert(epi[i].binary, Numvariables);
  }
/*
  cout << "\nepi-decimals : ";
  for(int i=0; i<epi.size(); i++){
    cout << epi[i].decimal << " ";
  }*/

  for(int i=0; i<pi.size(); i++){
    if(!checkepi[i]){
      nepi.push_back(pi[i]); // NEPI인 minterm저장
    }
  }

  for(int i=0; i<nepi.size(); i++){
    nepi[i].decimal = convert(nepi[i].binary, Numvariables);
  }

  sort(epi.begin(), epi.end(), cmp_minterm); // 결과값 오름차순 정렬
  sort(nepi.begin(), nepi.end(), cmp_minterm); // 결과값 오름차순 정렬

  string ans;

  cout << "EPI,";
  ans = "EPI,";
  for(int i=0; i<epi.size(); i++){
    cout << "\"";

    for(int j=0; j<epi[i].binary.size(); j++){
        cout << epi[i].binary[j];
        ans+=epi[i].binary[j];
      }
    cout << "\",";

  }

  cout << "\"NEPI\",";
  ans.append("NEPI,");

  for(int i=0; i<nepi.size(); i++){
    cout << "\"";

    for(int j=0; j<nepi[i].binary.size(); j++){
      cout << nepi[i].binary[j];
      ans += nepi[i].binary[j];
    }
    if(i==nepi.size()-1){
      cout << "\"";

    }else{
      cout << "\",";
      ans.append(",");
    }
  }
  cout << "\n" << ans << endl;
  return 0;
}
