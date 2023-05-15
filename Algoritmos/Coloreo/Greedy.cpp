#include<bits/stdc++.h>
using namespace std;
#define fore(i,a,b) for(int i=a,c=b;i<c;i++)
#define mset(a,b) memset(a,b,sizeof(a))
#define ALL(a) a.begin(),a.end()
#define SZ(a) int(a.size())
#define fst first
#define snd second
#define pb push_back
#define FIO ios::sync_with_stdio(0);cin.tie(0);cout.tie(0)
typedef long long ll;
typedef long double ld;
typedef pair<int,int> ii;
typedef vector<int> vi;

const int MAXN=1e5+7;
vi g[MAXN];
int ans[MAXN];

int main(){
    FIO;

    // Grafo
    // n m
    // aristas (u v)
    // orden de los n nodos
    int n,m;cin>>n>>m;
    fore(i,0,m){int u,v;cin>>u>>v;--u,--v;g[u].pb(v);g[v].pb(u);}
    vi orden(n);fore(i,0,n){cin>>orden[i];--orden[i];}

    // Greedy
    for(auto u:orden){
        vi cols;
        for(auto v:g[u])if(ans[v])cols.pb(ans[v]);
        sort(ALL(cols));

        int act=1;
        for(auto x:cols){
            if(x>act)break;
            act=x+1;
        }
        ans[u]=act;
    }

    cout<<"El coloreo es:\n";
    fore(i,0,n)cout<<ans[i]<<' ';
    cout<<'\n';

    return 0;
}