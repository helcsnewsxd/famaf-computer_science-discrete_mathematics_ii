#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Dinic{
    struct Edge{int v, rev; ll f, c; bool na;}; // (vecino, indice, flujo, costo, estaEnNetworkAuxiliar)
    typedef vector<int> NA;
    typedef pair<ll, vector<pair<int, int> > > Step;
    typedef vector<pair<NA, vector<Step> > > Steps;
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
        g[u].push_back({v, (int)g[v].size(), 0, c, 0}); // forward
        g[v].push_back({u, (int)g[u].size() - 1, 0, 0, 0}); // backward (se diferencia por tener capacidad 0)
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

    Step path_search_DFS(int u){
        if(u == 1) return {INF, {{u, -1}}};
        for(int i = 0, sz = g[u].size(); i < sz; i++){
            Edge &ne = g[u][i];
            if(!ne.na) continue; // no esta en el NA
            if((ne.c != 0 && ne.f < ne.c) || (ne.c == 0 && ne.f != 0)){ // forward || backward
                Step act_stp = path_search_DFS(ne.v);
                if(act_stp.second.size() != 0){
                    act_stp.first = min(act_stp.first, ne.c ? ne.c - ne.f : ne.f);
                    act_stp.second.push_back({u, i});
                    return act_stp;
                }
            }
        }
        return {0, {}};
    }

    Step path_search_DFS(){
        return path_search_DFS(0);
    }

    NA path_search_BFS(){
        // Limpio todas las aristas
        for(int u = 0; u < n; u++)
            for(Edge &ne : g[u])
                ne.na = false;

        // Saber distancia de s a t
        vector<int> dist(n);
        queue<int> q;
        dist[0] = 1;
        q.push(0);

        while(!q.empty()){
            int u = q.front();
            q.pop();
            
            if(u == 1) break;

            for(int i = 0, sz = g[u].size(); i < sz; i++){
                Edge &ne = g[u][i];
                if(dist[ne.v] != 0) continue;
                if((ne.c != 0 && ne.f < ne.c) || (ne.c == 0 && ne.f != 0)){ // forward || backward
                    dist[ne.v] = dist[u] + 1;
                    q.push(ne.v);
                }
            }
        }

        if(dist[1] == 0) return {};
        
        // Calcular los nodos del NA
        NA na;

        for(int i = 2; i < n; i++) dist[i] = 0;
        while(!q.empty()) q.pop();
        q.push(0);

        while(!q.empty()){
            int u = q.front();
            q.pop();

            na.push_back(u);
            
            for(int i = 0, sz = g[u].size(); i < sz; i++){
                Edge &ne = g[u][i];
                if((ne.c != 0 && ne.f < ne.c) || (ne.c == 0 && ne.f != 0)){ // forward || backward
                    if(dist[u] == dist[1] - 1){
                        if(ne.v == 1) ne.na = true;
                    }else if(dist[ne.v] != 0){
                        if(dist[ne.v] == dist[u] + 1) ne.na = true;
                    }else{
                        ne.na = true;
                        dist[ne.v] = dist[u] + 1;
                        q.push(ne.v);
                    }
                }
            }
        }

        na.push_back(1);

        return na;
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
            NA na = path_search_BFS();
            if(na.size() == 0) break;

            pair<NA, vector<Step> > stp_add = {na, {}};

            while(true){
                Step act_stp = path_search_DFS();
                if(act_stp.second.size() == 0) break;

                mf += act_stp.first;
                reverse(act_stp.second.begin(), act_stp.second.end());
                path_actualize(act_stp);

                stp_add.second.push_back(act_stp);
            }

            stp.push_back(stp_add);
        }
        min_cut();
        assert(check_MFMC_theorem());
    }

    void print(){
        cout << "El MAX FLOW es de " << mf << " considerando " << cnv.itoc(0) << " como source y " << cnv.itoc(1) << " como sink\n\n";

        cout << "Los pasos a seguir para lograrlo son:\n\n";
        for(pair<NA, vector<Step> > s : stp){
            cout << "   Network Auxiliar con los siguientes nodos:\n      ";
            for(int x : s.first) cout << cnv.itoc(x) << ' ';
            cout << "\n   y con Greedy se resuelve con:\n";

            ll eps_greedy = 0;
            for(Step s2 : s.second){
                cout << "      ";
                for(pair<int, int> x : s2.second) cout << cnv.itoc(x.first) << ' ';
                cout << ": " << s2.first << '\n';
                eps_greedy += s2.first;
            }

            cout << "   sumando " << eps_greedy << " al flujo\n\n";
        }

        cout << "Ademas, el MIN CUT esta dado por:\n   ";
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
    Dinic(){
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
    Dinic();
}