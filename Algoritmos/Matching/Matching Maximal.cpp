#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Matching{
    int n, m, maxMatching;
    vector<vector<int> > matrixAdj;
    vector<int> rowTag, columnTag;
    vector<int> rowFather, columnFather;

    Matching(){
        readInput();
    }

    static void printStringSquare(string s){
        // tam of 5 chars
        int leftover = 5 - int(s.size());
        int firstPrint = leftover/2;
        cout << string(firstPrint, ' ') << s << string(leftover-firstPrint, ' ');
    }

    static string intToRoman(int a){
        // less than 3999
        string ans;
        string M[] = {"","M","MM","MMM"};
        string C[] = {"","C","CC","CCC","CD","D","DC","DCC","DCCC","CM"};
        string X[] = {"","X","XX","XXX","XL","L","LX","LXX","LXXX","XC"};
        string I[] = {"","I","II","III","IV","V","VI","VII","VIII","IX"};
        ans = M[a/1000]+C[(a%1000)/100]+X[(a%100)/10]+I[(a%10)];
        return ans;
    }

    void print(){
        // n less than 26
        cout << '\n';
        cout << "  Actual Matching = " << maxMatching << '\n';
        for(int i = -1; i < n; i++){
            if(i == -1){
                printStringSquare("");
                for(int j = 1; j <= m; j++)
                    printStringSquare(intToRoman(j));
                cout << '\n';
                continue;
            }

            printStringSquare(string(1, 'A' + i));
            for(int j = 0; j < m; j++){
                string s = to_string(matrixAdj[i][j]);
                if(rowTag[i] == j){
                    s = "[" + s + "]";
                    assert(columnTag[j] == i);
                }
                printStringSquare(s);
            }
            printStringSquare(rowFather[i] == -1 ? "" : (rowFather[i] == -2 ? "s" : intToRoman(rowFather[i]+1)));
            cout << '\n';
        }
        for(int j = -1; j < n; j++){
            if(j == -1) printStringSquare("");
            else printStringSquare(columnFather[j] == -1 ? "" : string(1, 'A' + columnFather[j]));
        }
        cout << '\n';
    }

    void readInput(){
        // n m
        // 1 0 1 ... 0
        //    ...
        // 0 0 1 ... 1
        // Every row and column has one or more 1's
        maxMatching = 0;

        cin >> n >> m;

        matrixAdj.resize(n, vector<int> (m, 0));
        rowTag.resize(n, -1), columnTag.resize(m, -1);
        rowFather.resize(n, -1), columnFather.resize(m, -1);

        for(int i = 0; i < n; i++)
            for(int j = 0; j < m; j++)
                cin >> matrixAdj[i][j];
    }


    void extendMatching(int x, bool isColumn){
        if(isColumn){
            for(int i = 0; i < n; i++){
                if(matrixAdj[i][x] && i != columnTag[x]){
                    maxMatching++;
                    columnTag[x] = i;
                    extendMatching(i, 0);
                    rowTag[i] = x;
                    break;
                }
            }
        }else if(rowTag[x] == -1) return;
        else{
            maxMatching--;
            extendMatching(rowTag[x], 1);
        }
    }

    void stepMaxMatching(){
        queue<pair<int, bool> > q; // (number, isColumn)

        // search the first row
        for(int i = 0; i < n; i++){
            if(rowTag[i] == -1){
                rowFather[i] = -2;
                q.push({i, 0});
            }
        }

        while(!q.empty()){
            // print(); // --> ALL STEPS
            pair<int, bool> y = q.front(); int x = y.first;
            q.pop();
            if(y.second == 0){ // isRow
                for(int j = 0; j < m; j++){
                    if(matrixAdj[x][j] && j != rowTag[x] && columnFather[j] == -1){
                        columnFather[j] = x;
                        q.push({j, 1});
                    }
                }

            }else{ // isColumn
                if(columnTag[x] == -1){ // extend matching
                    extendMatching(x, 1);
                    break;
                }else if(rowFather[columnTag[x]] == -1){
                    rowFather[columnTag[x]] = x;
                    q.push({columnTag[x], 0});
                }
            }
        }

        print();
        rowFather.assign(n, -1), columnFather.assign(m, -1);
    }

    void firstMatching(){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                if(matrixAdj[i][j] && columnTag[j] == -1){
                    maxMatching++;
                    columnTag[j] = i;
                    rowTag[i] = j;
                    break;
                }
            }
        }
    }

    void calcMaxMatching(){
        firstMatching();
        print();

        int antMaxMatching;
        do{
            cout << string(40, '=') << '\n';
            antMaxMatching = maxMatching;
            stepMaxMatching();
        }while(antMaxMatching != maxMatching);
    }
};

int main(){
    Matching *matching = new Matching();
    matching->calcMaxMatching();
    delete matching;
}