#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct EK{
    struct Edge{int v, rev; ll f, c;}; // (vecino, indice, flujo, costo)
    typedef pair<ll, vector<pair<int, int> > > Step;
    typedef vector<Step> Steps;
    typedef ll MFlow;
    typedef vector<int> MCut;
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
    MCut mc; // Min Cut
    Steps stp; // Pasos hechos por el algoritmo
    void add_edge(int u, int v, ll c){
        g[u].push_back({v, (int)g[v].size(), 0, c}); // forward
        g[v].push_back({u, (int)g[u].size() - 1, 0, 0}); // backward (se diferencia por tener capacidad 0)
    }

    void path_actualize(Step act_stp){
        for(pair<int, int> x : act_stp.second){
            if(x.second == -1) break;
            Edge &ne = g[x.first][x.second];
            int type = ne.c ? 1 : -1;
            ne.f += type * act_stp.first;
            g[ne.v][ne.rev].f += type * act_stp.first;
        }
    }

    Step path_search_BFS(){
        vector<pair<int, int> > father(n, {-2, -2});
        queue<int> q;
        father[0] = {-1, -1};
        q.push(0);

        while(!q.empty()){
            int u = q.front();
            q.pop();
            
            if(u == 1) break;

            for(int i = 0, sz = g[u].size(); i < sz; i++){
                Edge &ne = g[u][i];
                if(father[ne.v].first != -2) continue;
                if((ne.c != 0 && ne.f < ne.c) || (ne.c == 0 && ne.f != 0)){ // forward || backward
                    father[ne.v] = {u, i};
                    q.push(ne.v);
                }
            }
        }

        Step act_stp = {1e18, {}};
        if(father[1].first == -2) return act_stp;

        int u = 1; act_stp.second.push_back({u, -1});
        while(u != 0){
            pair<int, int> v = father[u];
            act_stp.second.push_back(v);

            Edge &ne = g[v.first][v.second];
            act_stp.first = min(act_stp.first, ne.c ? ne.c - ne.f : ne.f);
            u = v.first;
        }
        return act_stp;
    }

    void min_cut(){
        vector<bool> vis(n, false);
        queue<int> q;
        q.push(0);

        while(!q.empty()){
            int u = q.front();
            q.pop();
            
            if(u == 1) assert(false);
            if(vis[u]) continue;
            vis[u] = true;

            mc.push_back(u);

            for(int i = 0, sz = g[u].size(); i < sz; i++){
                Edge &ne = g[u][i];
                if(vis[ne.v]) continue;
                if((ne.c != 0 && ne.f < ne.c) || (ne.c == 0 && ne.f != 0)){ // forward || backward
                    q.push(ne.v);
                }
            }
        }
    }

    bool check_MFMC_theorem(){
        MFlow mf_cmp = 0;
        vector<bool> cut(n, false);

        for(int x : mc) cut[x] = true;

        for(int u = 0; u < n; u++){
            if(!cut[u]) continue;
            for(Edge &ne : g[u]){
                if(ne.c == 0 || cut[ne.v]) continue;
                mf_cmp += ne.f;
            }
        }

        return mf == mf_cmp;
    }

    void max_flow(){
        mf = 0, stp = {};
        while(true){
            Step act_stp = path_search_BFS();
            if(act_stp.second.size() == 0) break;

            mf += act_stp.first;
            reverse(act_stp.second.begin(), act_stp.second.end());
            path_actualize(act_stp);

            stp.push_back(act_stp);
        }
        min_cut();
        assert(check_MFMC_theorem());
    }

    void print(){
        cout << "El MAX FLOW es de " << mf << " considerando " << cnv.itoc(0) << " como source y " << cnv.itoc(1) << " como sink\n\n";

        cout << "Los pasos a seguir para lograrlo son:\n";
        for(Step s : stp){
            cout << "   EPS = " << s.first << " con camino  ";
            for(pair<int, int> x : s.second) cout << cnv.itoc(x.first) << ' ';
            cout << "\n";
        }

        cout << "\nAdemas, el MIN CUT esta dado por:\n   ";
        for(int x : mc) cout << cnv.itoc(x) << ' ';

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
    EK(){
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
    EK();
}