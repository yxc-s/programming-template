/* 原问题链接 : https://codeforces.com/contest/1986/problem/F */

#include <bits/stdc++.h>


template<const bool USE_DSU_WEIGHT, const bool USE_DSU_SET_ELEMENT = false>
class DisjointSet {
    friend void unionWeights(DisjointSet& dsu, int x, int y, int px, int py, long long value);
    friend void compressWeights(DisjointSet& dsu, int x, int y);
public:
    constexpr explicit DisjointSet(unsigned int sz) :
        sz_(sz),
        num_sets_(sz)
    {
        fa_.resize(sz_);
        std::iota(fa_.begin(), fa_.end(), 0);
        set_size_.assign(sz_, 1);

        if constexpr (USE_DSU_WEIGHT){
            weight_.resize(sz_);
        }

        if constexpr (USE_DSU_SET_ELEMENT) {
            elements_.resize(sz_);
            for (int i = 0; i < sz_; ++i) {
                elements_[i].emplace_back(i);
            }
        }

    }

    /* 获取集合代表元素。*/
    int findSet(int x) {
        if (fa_[x] == x){
            return x;
        }
        int par = fa_[x];
        fa_[x] = findSet(fa_[x]);
        if constexpr (USE_DSU_WEIGHT) {
            compressWeights(*this, x, par);
        }
        return fa_[x];
    }

    /* 获取元素所在集合大小。*/
    int getSetSize(int x) {
        return set_size_[findSet(x)];
    }

    /* 获取当前元素权重。*/
    long long getWeight(int x) {
        findSet(x);
        return weight_[x];
    }

    /* 获取集合数量。*/
    int countSets()  {
        return num_sets_;
    }

    /* 查询两元素是否同一集合。*/
    bool isSameSet(int x, int y) {
        return findSet(x) == findSet(y);
    }
    
    /* 合并两个集合，缺省参数是合并时的权值（如果是带权并查集）。*/
    bool unionSet(int x, int y, long long value = 0) {
        int px = findSet(x);
        int py = findSet(y);
        if (px == py) {
            return false;
        }
        fa_[px] = py;
        num_sets_--;
        if constexpr(USE_DSU_WEIGHT) {
            unionWeights(*this, x, y, px, py, value);
        }
        set_size_[py] += set_size_[px];

        if constexpr (USE_DSU_SET_ELEMENT) {
            elements_[y].insert(elements_[y].end(), elements_[x].begin(), elements_[x].end());
            elements_[x].clear();
        }
        
        return true;
    }

    /* 获取集合所有的元素，可能会TLE */
    std::vector<int> getSetElements(int x) {
        return elements_[findSet(x)];
    }


private:
    int                             sz_;
    int                             num_sets_;
    std::vector<int>                fa_;
    std::vector<int>                set_size_;
    std::vector<long long>          weight_;
    std::vector<std::vector<int>>   elements_;

};

/* x和y是操作时的集合节点，要把x所在集合合并到y。px和py是前两者的集合代表元素。
  value是一个缺省值，代表指定x->y的权值（一般是输入数据），默认为0。*/
inline void unionWeights(DisjointSet<true>& dsu, int x, int y, int px, int py, long long value = 0){

}

/* 路径压缩时的更新权重操作，y是x压缩前的直接父亲节点。*/
inline void compressWeights(DisjointSet<true>& dsu, int x, int y){

}


/* 如果是带权并查集，将false改为true，并将上面的两个友元函数实现 */
using Dsu  = DisjointSet<false>;


template<const unsigned int START_INDEX> 
class RobertTarjan{
public:
    RobertTarjan() = default;

    
    /* 获取所有的连通分量（必须是有向图,无向图直接搜索或者dsu) */
    static std::vector<std::vector<int>> getScc(std::vector<std::vector<int>>& al){
        int timer = 0;
        int n     = (int)(al.size()) - 1;
        std::stack<int>                 stk;
        std::vector<int>                low(n + 1, -1);
        std::vector<int>                dfn(n + 1, -1);
        std::vector<bool>               in_stack(n + 1, false);
        std::vector<std::vector<int>>   components;
        std::function<void(int)> dfs = [&](int u){
            low[u] = dfn[u] = (++ timer);
            in_stack[u] = true;
            stk.push(u);
            for (const auto& v : al[u]){
                if (dfn[v] == -1){
                    dfs(v);
                    low[u] = std::min(low[u], low[v]);
                }
                else if (in_stack[v]){
                    low[u] = std::min(low[u], low[v]);
                }
            }
            if (low[u] == dfn[u]){
                components.emplace_back();
                while (!stk.empty()){
                    int cur = stk.top();
                    stk.pop();
                    components.back().emplace_back(cur);
                    in_stack[cur] = false;
                    if (cur == u){
                        break;
                    }
                }
            }
        };
        for (int i = START_INDEX; i <= n; ++i){
            if (dfn[i] == -1){
                dfs(i);
            }
        }
        return components;
    }

    /* 获取图中所有的割点 */
    static std::vector<int> getCutVertices(const std::vector<std::vector<int>>& al){
        int timer = 0;
        int n     = (int)(al.size()) - 1;
        std::vector<int>                low(n + 1, -1);
        std::vector<int>                dfn(n + 1, -1);
        std::vector<int>                cut_points;
        std::function<void(int, int)> dfs = [&](int u, int p){
            dfn[u] = low[u] = (++ timer);
            int nums_child = 0;
            for (const auto& v : al[u]){
                if (v != p){
                    if (dfn[v] != -1){
                        low[u] = std::min(low[u], dfn[v]);
                    }
                    else{
                        dfs(v, u);
                        if (low[v] >= dfn[u] && p != -1){
                            cut_points.push_back(u);
                        }
                        low[u] = std::min(low[u], dfn[v]);
                        ++ nums_child;
                    }
                }
            }
            if (p == -1 && nums_child > 1){
                cut_points.push_back(u);
            }
        };
        for (int i = START_INDEX; i <= n; ++i){
            if (dfn[i] == -1){
                dfs(i, -1);
            }
        }
        return cut_points;  
    }

    /* 获取图中所有的割边 */
    static std::vector<std::pair<int, int>> getCutEdges(const std::vector<std::vector<int>>& al){
        int timer = 0;
        int n     = (int)(al.size()) - 1;
        std::vector<int>                  low(n + 1, -1);
        std::vector<int>                  dfn(n + 1, -1);
        std::vector<std::pair<int, int>>  cut_edges;
        std::function<void(int, int)> dfs = [&](int u, int p){
            dfn[u] = low[u] = (++ timer);
            for (const auto& v : al[u]){
                if (v != p){
                    if (dfn[v] != -1){
                        low[u] = std::min(low[u], low[v]);
                    }
                    else{
                        dfs(v, u);
                        if (low[v] > dfn[u]){
                            cut_edges.emplace_back(u, v);
                        }
                        low[u] = std::min(low[u], low[v]);
                    }
                }
            }
        };
        for (int i = START_INDEX; i <= n; ++i){
            if (dfn[i] == -1){
                dfs(i, -1);
            }
        }
        return cut_edges;  
    }
    

};

/* TODO: 相同的割点是否被多次存储?*/

/* 1代表图中起始节点下标，也可以设置为0 */
using Tarjan = RobertTarjan<1>;



using namespace std;
void preProcess() {

}


void solve(){
    int n, m;
    cin >> n >> m;

    Dsu dsu(n + 1);
    vector<vector<int>> al(n + 1);
    vector<vector<int>> tree(n + 1);
    for (int i = 1; i <= m; ++i) {
        int u, v;
        cin >> u >> v;
        al[u].push_back(v);
        al[v].push_back(u);
        if (dsu.isSameSet(u, v) == false) {
            dsu.unionSet(u, v);
            tree[u].push_back(v);
            tree[v].push_back(u);
        } 
    }

    auto cut_edges = Tarjan::getCutEdges(al);
    
    vector<int> sz(n + 1);
    vector<int> parent(n + 1);
    auto dfs = [&](auto&& self, int u, int p)->void {
        parent[u] = p;
        for (const auto& v : tree[u]) {
            if (v != p) {
                self(self, v, u);
                sz[u] += sz[v];
            }
        }
        sz[u] += 1;
    };

    dfs(dfs, 1, 0);

    auto cal = [](int x)->long long{
        return 1ll * x * (x - 1) / 2;
    };

    long long ans = cal(n);
    for (auto& [u, v] : cut_edges) {
        if (parent[u] == v){
            swap(u, v);
        }
        ans = min(ans, cal(sz[v]) + cal(sz[1] - sz[v]));
    }

    cout << ans << '\n';
}


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int tc = 1;

    preProcess();

    if constexpr (true){
        cin >> tc;
    }

    while (tc--) {
        solve();
    }
    return 0;
}
