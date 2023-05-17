#include <bits/stdc++.h>

using namespace std;

class Hungaro{
private:
    int n, maxMatching;
    vector<vector<int> > matrixAdj, origMatrixAdj;
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
                for(int j = 1; j <= n; j++)
                    printStringSquare(intToRoman(j));
                cout << '\n';
                continue;
            }

            printStringSquare(string(1, 'A' + i));
            for(int j = 0; j < n; j++){
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
        // n
        // 13 0 1 ... 1944
        //    ...
        // 485 2 1 ... 1
        // Every row and column has one or more 1's
        // n = m
        // There is a perfect matching
        
        maxMatching = 0;

        cin >> n;

        matrixAdj.resize(n, vector<int> (n, 0));
        tag[0].resize(n, -1), tag[1].resize(n, -1);
        father[0].resize(n, -1), father[1].resize(n, -1);

        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
                cin >> matrixAdj[i][j];
        origMatrixAdj = matrixAdj;
    }

    void firstMatching(){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(matrixAdj[i][j] == 0 && tag[1][j] == -1){
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
                if(matrixAdj[i][x] == 0 && i != tag[1][x] && father[0][i] == -2){
                    maxMatching++;
                    tag[1][x] = i;
                    extendMatching(i, 0);
                    tag[0][i] = x;
                    return;
                }
            }

            for(int i = 0; i < n; i++){
                if(matrixAdj[i][x] == 0 && i != tag[1][x] && father[0][i] != -1){
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

    void matrixChange(queue<pair<int, bool> > &q){
        cout << "\nNO SE PUEDE EXTENDER EL MATCHING PARA LOGRAR UN MATCHING PERFECTO Y SE LLEGA A:\n";
        print();

        set<int> s,gamma;

        // the queue is only tagged rows
        while(!q.empty()) q.pop();
        for(int i = 0; i < n; i++)
            if(father[0][i] != -1){
                q.push({i, 0});
                s.insert(i);
                gamma.insert(father[0][i]);
            }
        
        // sum and substract into the adjacency matrix
        int mini = 1e9;
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
                if(s.count(i) && !gamma.count(j))
                    mini = min(mini, matrixAdj[i][j]);
        
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++){
                if(s.count(i)&&!gamma.count(j)) matrixAdj[i][j] -= mini;
                else if(!s.count(i)&&gamma.count(j)) matrixAdj[i][j] += mini;
            }
        
        cout << "\nTENEMOS QUE S = { ";
        for(int x:s) cout << string(1, 'A' + x) << ' ';
        cout << "} Y Gamma(S) = { ";
        for(int x:gamma) if(x != -2) cout << intToRoman(x+1) << ' ';
        cout << "}\n";

        // print new matrix
        cout << "\nSE HACE UN CAMBIO DE MATRIZ OBTENIENDO LA NUEVA:\n";
        print();
    }

    void stepQueue(queue<pair<int, bool> > &q){
        while(!q.empty()){
            // print(); // --> ALL STEPS
            pair<int, bool> y = q.front(); int x = y.first;
            q.pop();
            if(y.second == 0){ // isRow
                for(int j = 0; j < n; j++){
                    if(matrixAdj[x][j] == 0 && j != tag[0][x] && father[1][j] == -1){
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

        int antMaxMatching;
        while(true){
            antMaxMatching = maxMatching;
            stepQueue(q);
            if(antMaxMatching == maxMatching){ // matrix change
                matrixChange(q);
            }else break;
        }

        print();
        father[0].assign(n, -1), father[1].assign(n, -1);
    }

    int minSum(){
        matrixAdj = origMatrixAdj;
        int sum = 0;
        for(int i = 0; i < n; i++){
            assert(tag[0][i] != -1);
            sum += matrixAdj[i][tag[0][i]];
        }

        cout << string(60, '_') << '\n';
        cout << "           LA RESPUESTA ES LA SIGUIENTE:\n";
        print();
        cout << "           CON UNA SUMA DE " << sum << "\n\n";

        return sum;
    }

    void firstSumAndSubstract(){
        for(int i = 0; i < n; i++){
            int mini = 1e9;
            for(int j = 0; j < n; j++)
                mini = min(mini, matrixAdj[i][j]);
            for(int j = 0; j < n; j++)
                matrixAdj[i][j] -= mini;
        }

        for(int i = 0; i < n; i++){
            int mini = 1e9;
            for(int j = 0; j < n; j++)
                mini = min(mini, matrixAdj[j][i]);
            for(int j = 0; j < n; j++)
                matrixAdj[j][i] -= mini;
        }
    }

public:
    Hungaro(){ readInput(); }

    int calcMinSumMaxMatching(){
        firstSumAndSubstract();
        print();

        cout << string(40, '=') << '\n';

        firstMatching();
        print();

        do{
            cout << '\n' << string(40, '=') << '\n';
            stepMaxMatching();
        }while(n != maxMatching);

        assert(maxMatching == n);
        return minSum();
    }

};

int main(){
    Hungaro *matching = new Hungaro();
    matching->calcMinSumMaxMatching();
    delete matching;

    return 0;
}