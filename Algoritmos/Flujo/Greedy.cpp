#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct GFlow{
    struct Edge{int v; ll f, c;}; // (vecino, flujo, costo)
    typedef pair<ll, vector<pair<int, int> > > Step;
    typedef vector<Step> Steps;
    typedef ll MFlow;
    struct Name{
        int cnt = 0;
        map<char, int> ci;
        map<int, char> ic;
        void add(char ch){
            if(ci.find(ch) != ci.end()) return;
            ci[ch] = cnt, ic[cnt] = ch, cnt++;
        }
        int ctoi(char ch){
            if(ci.find(ch) == ci.end()) return -1;
            return ci[ch];
        }
        char itoc(int num){
            if(ic.find(num) == ic.end()) return '\n';
            return ic[num];
        }
    };
    const ll INF = 1e18; // infinito

    int n; vector<vector<Edge>> g; // cantidad de nodos, grafo
    MFlow mf; // Max Flow
    Steps stp; // Pasos hechos por el algoritmo
    void add_edge(int u, int v, ll c){
        g[u].push_back({v, 0, c});
    }

    void path_actualize(Step act_stp){
        for(pair<int, int> x : act_stp.second){
            if(x.second == -1) break;
            g[x.first][x.second].f += act_stp.first;
        }
    }

    Step path_search_DFS(int u){
        if(u == 1) return {INF, {{u, -1}}};
        for(int i = 0, sz = g[u].size(); i < sz; i++){
            Edge &e = g[u][i];
            if(e.f < e.c){
                Step act_stp = path_search_DFS(e.v);
                if(act_stp.second.size() != 0){
                    act_stp.first = min(act_stp.first, e.c - e.f);
                    act_stp.second.push_back({u, i});
                    return act_stp;
                }
            }
        }
        return {0,{}};
    }

    void max_flow(){
        mf = 0, stp = {};
        while(true){
            Step act_stp = path_search_DFS(0);
            if(act_stp.second.size() == 0) break;

            mf += act_stp.first;
            reverse(act_stp.second.begin(), act_stp.second.end());
            path_actualize(act_stp);

            stp.push_back(act_stp);
        }
    }

    void print(){
        cout << "El MAX FLOW es de " << mf << " considerando " << cnv.itoc(0) << " como source y " << cnv.itoc(1) << " como sink\n\n";

        cout << "Los pasos a seguir para lograrlo son:\n";
        for(Step s : stp){
            cout << "   EPS = " << s.first << " con camino  ";
            for(pair<int, int> x : s.second) cout << cnv.itoc(x.first) << ' ';
            cout << "\n";
        }

        cout << "\n\n";
    }

    Name cnv; // conversor
    /**
     * Toma de STDIN las aristas. El formato del input es:
        * st -1 --> s es la source y t la sink (no es arista esto)
        * ab 10 --> Arista entre nodos a y b, siendo 10 el costo
        * ...
        * END --> Final del input
    * Cada nodo es un char
    */
    GFlow(){
        string s; ll cap;
        vector<pair<pair<int, int>, ll> > nw_ed;
        while(cin >> s){
            if(s == "END") break;
            cin >> cap;
            cnv.add(s[0]), cnv.add(s[1]);
            if(cap == -1) continue;
            nw_ed.push_back({{cnv.ctoi(s[0]), cnv.ctoi(s[1])}, cap});
        }
        n = cnv.cnt;
        g.resize(n);

        for(pair<pair<int, int>, ll> x : nw_ed)
            add_edge(x.first.first, x.first.second, x.second);

        max_flow();
        print();
    }
};

int main(){
    GFlow();
}