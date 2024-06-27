/*
 * Tarjan
 * 设计思想：Tarjan算法的核心思想是根据dfs的时间戳信息来 寻找有向图中的连通分量、 寻找无向图中的割点和割边。
 * 基于面向对象的编程思想，本方法尽可能多的隐藏了内部实现的细节，并且将必要的编程接口暴露在外部，并需要对这些接口进行直接的修改。
 *                             使用方法: 函数均以静态函数实现，将图的节点信息以基于二维的vector容器进行存储后，直接在调用函数时传递进去即可。
 *                                      节点起始下标可以根据模板参数指定0或者1。
 *
 * gitHub(仓库地址): https://github.com/yxc-s/programming-template.git
 */






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
