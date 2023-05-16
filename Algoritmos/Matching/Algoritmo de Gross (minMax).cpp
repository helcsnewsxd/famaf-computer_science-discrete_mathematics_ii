#include<bits/stdc++.h>
using namespace std;

class Matching{
private:
    int n, m, maxMatching;
    vector<vector<int> > matrixAdj;
    vector<int> tag[2], father[2];

    void printStringSquare(string s){
        // tam of 5 chars
        int leftover = 5 - int(s.size());
        int firstPrint = leftover/2;
        cout << string(firstPrint, ' ') << s << string(leftover-firstPrint, ' ');
    }

    string intToRoman(int a){
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
                if(tag[0][i] == j){
                    s = "[" + s + "]";
                    assert(tag[1][j] == i);
                }
                printStringSquare(s);
            }
            printStringSquare(father[0][i] == -1 ? "" : (father[0][i] == -2 ? "s" : intToRoman(father[0][i]+1)));
            cout << '\n';
        }
        for(int j = -1; j < n; j++){
            if(j == -1) printStringSquare("");
            else printStringSquare(father[1][j] == -1 ? "" : string(1, 'A' + father[1][j]));
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
        tag[0].resize(n, -1), tag[1].resize(m, -1);
        father[0].resize(n, -1), father[1].resize(m, -1);

        for(int i = 0; i < n; i++)
            for(int j = 0; j < m; j++)
                cin >> matrixAdj[i][j];
    }

    void firstMatching(){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                if(matrixAdj[i][j] && tag[1][j] == -1){
                    maxMatching++;
                    tag[1][j] = i;
                    tag[0][i] = j;
                    break;
                }
            }
        }
    }

    void extendMatching(int x, bool isColumn){
        if(isColumn){
            for(int i = 0; i < n; i++){
                if(matrixAdj[i][x] && i != tag[1][x] && father[0][i] == -2){
                    maxMatching++;
                    tag[1][x] = i;
                    extendMatching(i, 0);
                    tag[0][i] = x;
                    return;
                }
            }

            for(int i = 0; i < n; i++){
                if(matrixAdj[i][x] && i != tag[1][x] && father[0][i] != -1){
                    maxMatching++;
                    tag[1][x] = i;
                    extendMatching(i, 0);
                    tag[0][i] = x;
                    return;
                }
            }
        }else if(tag[0][x] == -1) return;
        else{
            maxMatching--;
            extendMatching(tag[0][x], 1);
        }
    }

    void stepMaxMatching(){
        queue<pair<int, bool> > q; // (number, isColumn)

        // search the first row
        for(int i = 0; i < n; i++){
            if(tag[0][i] == -1){
                father[0][i] = -2;
                q.push({i, 0});
            }
        }

        while(!q.empty()){
            // print(); // --> ALL STEPS
            pair<int, bool> y = q.front(); int x = y.first;
            q.pop();
            if(y.second == 0){ // isRow
                for(int j = 0; j < m; j++){
                    if(matrixAdj[x][j] && j != tag[0][x] && father[1][j] == -1){
                        father[1][j] = x;
                        q.push({j, 1});
                    }
                }

            }else{ // isColumn
                if(tag[1][x] == -1){ // extend matching
                    extendMatching(x, 1);
                    break;
                }else if(father[0][tag[1][x]] == -1){
                    father[0][tag[1][x]] = x;
                    q.push({tag[1][x], 0});
                }
            }
        }

        print();
        father[0].assign(n, -1), father[1].assign(m, -1);
    }

public:
    Matching(){ readInput(); }

    Matching(int N, int M, vector<vector<int> > ADJ){
        assert(int(ADJ.size()) == N && int(ADJ[0].size()) == M);

        maxMatching = 0;
        n = N, m = M;

        matrixAdj = ADJ;
        tag[0].resize(n, -1), tag[1].resize(m, -1);
        father[0].resize(n, -1), father[1].resize(m, -1);
    }

    int calcMaxMatching(){
        firstMatching();
        print();

        int antMaxMatching;
        do{
            cout << string(40, '=') << '\n';
            antMaxMatching = maxMatching;
            stepMaxMatching();
        }while(antMaxMatching != maxMatching);

        return maxMatching;
    }
};

int calcMatching(int n, int limit, const vector<vector<int> > &matrixAdj){
    vector<vector<int> > newMatrixAdj(n, vector<int> (n, 0));
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            newMatrixAdj[i][j] = (matrixAdj[i][j] != 0 && matrixAdj[i][j] <= limit);
    
    Matching *matching = new Matching(n, n, newMatrixAdj);
    int ret = matching->calcMaxMatching();
    delete matching;
    return ret;
}

int Gross(){
    // n
    // 13 0 1 ... 1944
    //    ...
    // 485 2 1 ... 1
    // Every row and column has one or more 1's
    // n = m
    // There is a perfect matching
    
    int n; cin >> n;
    vector<vector<int> > matrixAdj(n, vector<int> (n, 0));
    set<int> setEdgeValues;
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++){
            cin >> matrixAdj[i][j];
            setEdgeValues.insert(matrixAdj[i][j]);
        }
    vector<int> edgeValues;
    for(int x : setEdgeValues) edgeValues.push_back(x);

    int s = -1, e = int(edgeValues.size());
    while(e - s > 1){
        int m = (s+e)/2;

        cout << string(60, '_') << '\n';
        cout << "           CONSIDERO CASO CON m = " << m << " Y edgeValues[m] = " << edgeValues[m] << '\n';

        if(calcMatching(n, edgeValues[m], matrixAdj) == n){ // isPerfectMatching
            cout << "           ES MATCHING PERFECTO\n\n";
            e = m;
        } else{
            cout << "           NO ES MATCHING PERFECTO\n\n";
            s = m;
        }
    }

    cout << string(60, '_') << '\n';
    cout << "           LA RESPUESTA ES " << edgeValues[e] << '\n';
    return e;
}

int main(){
    Gross();

    return 0;
}