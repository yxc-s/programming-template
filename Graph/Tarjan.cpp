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
    RobertTarjan(std::vector<std::vector<int>>&& s):
        al_(s),
        n_(al_.size() - 1),
        timer_(0),
        low_(n_ + 1, -1),
        dfn_(n_ + 1, -1),
        in_stack_(n_ + 1, false)
        {}

    
    /* 获取所有的连通分量 */
    const std::vector<std::vector<int>>& getScc(){
        for (int i = START_INDEX; i <= n_; ++i){
            if (dfn_[i] == -1){
                dfsScc(i);
            }
        }
        return components_;
    }


private:
    size_t                           n_;
    size_t                           timer_;
    std::vector<std::vector<int>>    al_;
    std::vector<std::vector<int>>    components_;
    std::vector<int>                 low_;
    std::vector<int>                 dfn_;
    std::vector<bool>                in_stack_;
    std::stack<int>                  stk_;
    

    void dfsScc(int u){
        low_[u] = dfn_[u] = timer_ ++;
        in_stack_[u] = true;
        stk_.push(u);
        for (const auto& v : al_[u]){
            if (dfn_[v] == -1){
                dfsScc(v);
                low_[u] = std::min(low_[u], low_[v]);
            }
            else if (in_stack_[v]){
                low_[u] = std::min(low_[u], low_[v]);
            }
        }
        if (low_[u] == dfn_[u]){
            components_.emplace_back();
            while (!stk_.empty()){
                int cur = stk_.top();
                stk_.pop();
                components_.back().emplace_back(cur);
                in_stack_[cur] = false;
                if (cur == u){
                    break;
                }
            }
        }
    }
};


using Tarjan = RobertTarjan<1>;
