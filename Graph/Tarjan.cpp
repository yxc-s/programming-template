
/*
 * Tarjan()
 * gitHub(仓库地址): https://github.com/yxc-s/programming-template.git
 */









/*
  约定图树节点下标总是从1开始！！
*/
class Tarjan{
public:
    Tarjan(const vector<vector<int>>& s):
        n_((int)s.size() - 1),
        al_(s),
        low_(n_ + 1, -1),
        dfn_(n_ + 1, -1),
        in_stack_(n_ + 1, false),
        timer_(0){}

    //strongly connected component
    std::vector<vector<int>> getScc(){
        for (int i = 1; i <= n_; ++i){
            if (dfn_[i] == -1){
                dfsScc(i);
            }
        }
        return components_;
    }


private:
    int n_;
    std::vector<vector<int>> al_;
    std::vector<vector<int>> components_;
    std::vector<int> low_;
    std::vector<int> dfn_;
    std::vector<bool> in_stack_;
    std::stack<int> stk_;
    int timer_;

    void dfsScc(int u){
        low_[u] = dfn_[u] = timer_ ++;
        in_stack_[u] = true;
        stk_.push(u);
        for (const auto& v : al_[u]){
            if (dfn_[v] == -1){
                dfsScc(v);
                low_[u] = min(low_[u], low_[v]);
            }
            else if (in_stack_[v] == true){
                low_[u] = min(low_[u], low_[v]);
            }
        }
        //head of strongly connected component
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