/*
 * Tarjan
 * 设计思想：目前只实现强连通分量查询，基于有向图的遍历过程中维护的栈与访问次序来判定连通分量。
 * 基于面向对象的编程思想，本方法尽可能多的隐藏了内部实现的细节，并且将必要的编程接口暴露在外部，并需要对这些接口进行直接的修改。
 *                             使用方法: 将图数据作为模板参数传进来，只支持vector实现的二维邻接表。
 *                                      节点起始下标可以根据模板参数指定0或者1，构建Tarjan时可以使用std::move()。
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
    static std::vector<int> getCutVertices(std::vector<std::vector<int>>& al){
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
                        low[u] = std::min(low[u], dfn[v]);
                        if (low[v] >= dfn[u] && p != -1){
                            cut_points.push_back(u);
                        }
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

    

};

/* 1代表图中起始节点下标，也可以设置为0 */
using Tarjan = RobertTarjan<1>;
