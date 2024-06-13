
/*
 * MultiSegmentTree(多重线段树)
 * 设计思想：多重线段树主要用于在遍历过程中要对线段树进行合并求解的情况。
 * 基于面向对象的编程思想，本方法尽可能多的隐藏了内部实现的细节，并且将必要的编程接口暴露在外部，并需要对这些接口进行直接的修改。
 * 在该设计中，需要修改的接口有：
 *                             UpdateNode：该结构体存储了区间上的操作类型：需要自定义结构体变量成员及构造函数；
 *                             MultiSegmentTreeNode: 需要在该结构体中写出要维护的区间参数，初始化函数；
 *                                                   该结构体重载了+来实现左右孩子区间合并：在重载函数中实现细节；
 *                                                   该结构体重载了|来实现两颗树合并叶子节点时的逻辑：在重载函数中实现细节；
 *                                                   实现区间上更新操作的方法：applyUpdate（）；
 *
 *                             多重线段树的主体所有必备的方法都已实现，无需修改。
 *
 * gitHub(仓库地址): https://github.com/yxc-s/programming-template.git
 */










/*
  该节点定义在区间上的操作，可以根据输入的类型重写该结构体。
*/
struct UpdateNode {
    /*
      自定义区间要执行的操作变量。
    */
    int value;


    /*
      自定义初始化构造函数。
    */
    explicit UpdateNode(int value_ = 0): value(value_) {

    }


    /*
      懒人标记向下传递时调用，涉及区间操作必须实现。
    */
    inline void mergeLazyMarks(const UpdateNode& parent_node, int segment_length) {

    }


    /*
      清除懒人标记，涉及区间操作必须实现。
    */
    inline void clear() {

    }
};





/*
  线段树的节点
  重载了合并区间符号 '+'
  实现了区间上更新数值的模板函数
*/
struct MultiSegmentTreeNode {
    int cnt = 0;
    long long sum = 0;


    MultiSegmentTreeNode() {}

    /*
      在当前树中左右孩子区间合并逻辑，必须实现。
    */
    friend MultiSegmentTreeNode operator + (const MultiSegmentTreeNode& a, const MultiSegmentTreeNode& b) {
        MultiSegmentTreeNode res{a};

        return res;
    }

    /*
      在a树和b树的叶子节点中进行合并，必须实现。
    */
    friend MultiSegmentTreeNode operator | (const MultiSegmentTreeNode& a, const MultiSegmentTreeNode& b){
        MultiSegmentTreeNode res{};

        return res;
    }


    /*
      区间上操作的逻辑，必须实现。
    */
    void applyUpdate(UpdateNode value, int segment_length) {
      
    }


};





/*
  多重线段树，初始化必须指定是否使用懒人标记（模板参数）。
  初始化列表参数必须指定线段树的数量。
  如果涉及区间操作，必须设置模板参数为true。
*/
template<const bool USE_LAZY_FLAG>
class MultiSegmentTree {
    using LAZY_TYPE      =      UpdateNode;
    using NODE_TYPE      =      MultiSegmentTreeNode;


public:
    explicit MultiSegmentTree(unsigned int n) : n_(n) {
        st_.resize(1);
        lchild_.resize(1);
        rchild_.resize(1);
        root_.resize(n_);
        if (USE_LAZY_FLAG){
            lazy_.resize(1);
            has_lazy_.resize(1);
        }
    }

    /*
      更新指定线段树。
    */
    inline void update(int tree_id, int pos, LAZY_TYPE value) {
        checkNodeIndex(root_[tree_id]);
        update(root_[tree_id], 1, n_, pos, value);
    }

    inline void update(int tree_id, int i, int j, LAZY_TYPE value){
        assert(USE_LAZY_FLAG == true);
        checkNodeIndex(root_[tree_id]);
        update(root_[tree_id], 1, n_, i, j, value);
    }

    /*
      查询树的指定点。
    */
    inline NODE_TYPE query(int tree_id, int pos){
        return query(root_[tree_id], 1, n_, pos, pos);
    }

    /*
      查询树的指定区间。
    */
    inline NODE_TYPE query(int tree_id, int i, int j){
        return query(root_[tree_id], 1, n_, i, j);
    }

    /*
      合并两棵线段树。
    */
    void merge(int u, int v) {
        assert(u && v && std::max(u, v) <= n_);
        root_[u] = merge(root_[u], root_[v], 1, n_);
    }


private:
    int n_;
    std::vector<NODE_TYPE>                st_;
    std::vector<int>                      lchild_;
    std::vector<int>                      rchild_;
    std::vector<int>                      root_;
    std::vector<LAZY_TYPE>                lazy_;
    std::vector<bool>                     has_lazy_;

    /*
      区间更新。
    */
    void update(int p, int l, int r, int i, int j, LAZY_TYPE value) {
        propagate(p, l, r);
        if (i > j) {
            return;
        }
        checkNodeIndex(lchild_[p]);
        checkNodeIndex(rchild_[p]);
        if (l >= i && r <= j){
            lazy_[p] = value;
            has_lazy_[p] = true;
            propagate(p, l, r);
            return;
        }
        int mid = (l + r) >> 1;
        update(lchild_[p], l, mid, i, std::min(mid, j), value);
        update(rchild_[p], mid + 1, r, std::max(mid + 1, i), j, value);
        st_[p] = st_[lchild_[p]] + st_[rchild_[p]];
    };
 
    /*
      线段树单点更新。
    */
    void update(int p, int l, int r, int pos, LAZY_TYPE value) {
        if (l == r) {
            st_[p].applyUpdate(value, pos);
            return;
        }
        int mid = (l + r) >> 1;
        if (pos <= mid) {
            checkNodeIndex(lchild_[p]);
            update(lchild_[p], l, mid, pos, value);
        }
        else {
            checkNodeIndex(rchild_[p]);
            update(rchild_[p], mid + 1, r, pos, value);
        }
        st_[p] = st_[lchild_[p]] + st_[rchild_[p]];
    }
 

    /*
      区间查询。
    */
    NODE_TYPE query(int p, int l, int r, int i, int j){
        if (USE_LAZY_FLAG) {
            propagate(p, l, r);
        }
        if (l >= i && r <= j){
            return st_[p];
        }
        int mid = (l + r) >> 1;
        if (j <= mid){
            return query(lchild_[p], l, mid, i, j);
        }
        else if (i > mid){
            return query(rchild_[p], mid + 1, r, i, j);
        }
        else{
            return query(lchild_[p], l, mid, i, mid) + query(rchild_[p], mid + 1, r, mid + 1, j);
        }
    }

    /*
      合并两颗线段树。
    */
    int merge(int p, int q, int l, int r) {
        if (USE_LAZY_FLAG){
            propagate(p, lchild_[p], rchild_[p]);
        }
        if (!p || !q) {
            return p + q;
        }
        if (l == r) {
            st_[p] = st_[p] | st_[q];
            return p;
        }
        int mid = (l + r) >> 1;
        lchild_[p] = merge(lchild_[p], lchild_[q], l, mid);
        rchild_[p] = merge(rchild_[p], rchild_[q], mid + 1, r);
        st_[p] = st_[lchild_[p]] + st_[rchild_[p]];
        return p;
    }

    /*
      使用该函数完全动态分配空间，一般空间比直接分配内存节省百分之50。
    */
    inline void checkNodeIndex(int& index) {
        if (index == 0) {
            index = static_cast<int> (st_.size());
            st_.emplace_back();
            lchild_.emplace_back(0);
            rchild_.emplace_back(0);
            if (USE_LAZY_FLAG){
                lazy_.emplace_back();
                has_lazy_.emplace_back(false);
            }
        }
    }

    /*
      懒人标记向下传播。
    */
    inline void propagate(int p, int l, int r) {
        if (has_lazy_[p] == true) {
            st_[p].applyUpdate(lazy_[p], r - l + 1);
            if (l != r) {
                lazy_[lchild_[p]].mergeLazyMarks(lazy_[p], r - l + 1);
                lazy_[rchild_[p]].mergeLazyMarks(lazy_[p], r - l + 1);
                has_lazy_[lchild_[p]] = has_lazy_[rchild_[p]] = true;
            }
            has_lazy_[p] = false;
            lazy_[p].clear();
        }
    }



};

/*
  ToDoList: 
*/
