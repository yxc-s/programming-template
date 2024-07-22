/*
 * StaticSegmentTree(静态线段树)
 * 设计思想：静态线段树主要用于先给出一组输入（一般这种输入确定了树中每个节点都会被访问到，所以直接静态开点），并且给出若干个修改和查询的情况。
 * 基于面向对象的编程思想，本方法尽可能多的隐藏了内部实现的细节，并且将必要的编程接口暴露在外部，并需要对这些接口进行直接的修改。
 * 
 * 在该设计中，需要修改的接口有：
 *                             UpdateNode：该结构体存储了区间上的操作类型：需要自定义结构体变量成员及构造函数；
 *                                         懒人标记的向下传递方法：mergeLazyMarks()；
 *                                         懒人标记被更新后的方法：clear()。
 *                             StaticSegmentTreeNode: 需要在该结构体中写出要维护的区间参数，初始化函数；
 *                                                    该类重载了+来实现左右孩子区间合并：在重载函数中实现细节；
 *                                                    实现区间上更新懒人标记值的方法：applyUpdate（）；
 *                                                    建树必须指定模板参数True或False，代表是否使用懒人标记。
 *                             静态线段树的主体所有必备的方法都已实现，无需修改，有的题目有剪枝操作请自行添加实现。
 *
 * gitHub(仓库地址): https://github.com/yxc-s/programming-template.git
 */










/*
  该节点定义在区间上的操作，可以根据输入的类型重写该结构体。
*/
struct UpdateNode {

    /* 自定义区间要执行的操作变量。*/
    int value;


    /* 自定义初始化构造函数。*/
    UpdateNode(int value_ = 0): value(value_) {

    }


    /*  懒人标记向下传递时调用，涉及区间操作必须实现。*/
    void mergeLazyMarks(const UpdateNode& parent_node, int segment_length) {

    }


    /* 清除懒人标记，涉及区间操作必须实现。 */
    void clear() {

    }
};





struct StaticSegmentTreeNode {

    /* 在区间上要维护的变量值，必须实现。*/
    long long sum;


    /* 构造函数，必须实现。*/
    StaticSegmentTreeNode(long long value = 0ll): sum(value) {}


    /* 重载左右孩子区间合并操作，必须实现。*/
    friend StaticSegmentTreeNode operator + (const StaticSegmentTreeNode& a, const StaticSegmentTreeNode& b) {
        StaticSegmentTreeNode res{};

        return res;
    }


    /* 区间更新方法，必须实现。*/
    void applyUpdate(const UpdateNode& value, int segment_length) {
        
    }


};





/*
  静态线段树，query返回类型必须是NODE_TYPE类型，更新数值类型必须是UpdateNode。
  初始化模板必须指定true或者false。
  该树总是约定区间左端点从1开始(避免(0 << 1)的情况)。
  可通过树节点类型的数组初始化建树（推荐，建树时间复杂度更低）。
  可通过要维护的区间的最大右端点下标来建树。
*/

template<const bool USE_LAZY_FLAG>
class StaticSegmentTree {
    using LAZY_TYPE      =      UpdateNode;
    using NODE_TYPE      =      StaticSegmentTreeNode;


public:
    StaticSegmentTree(unsigned int n) : n_(n) {
        st_.resize(4 * n_);
        if constexpr (USE_LAZY_FLAG){
            lazy_.resize(4 * n_);
            has_lazy_.resize(4 * n_);
        }
    }


    StaticSegmentTree(const std::vector<NODE_TYPE>& s) : n_(static_cast<int> (s.size()) - 1) {
        st_.resize(4 * n_);
        if constexpr (USE_LAZY_FLAG){
            lazy_.resize(4 * n_);
            has_lazy_.resize(4 * n_);
        }
        build(s, 1, 1, n_);
    }

    /* 单点更新。*/
    void update(int i, const LAZY_TYPE& value) {
        update(1, 1, n_, i, i, value);
    }

    /* 更新区间值。*/
    void update(int i, int j, const LAZY_TYPE& value) {
        update(1, 1, n_, i, j, value);
    }

    /* 获取区间节点。*/
    NODE_TYPE query(int i, int j) {
        return query(1, 1, n_, i, j);
    }


private:
    unsigned int n_;


    std::vector<NODE_TYPE>                st_;
    std::vector<LAZY_TYPE>                lazy_;
    std::vector<bool>                     has_lazy_;

    /* 区间更新。*/
    void update(int p, int l, int r, int i, int j, const LAZY_TYPE& value) {
        if constexpr (USE_LAZY_FLAG) { 
          propagate(p, l, r); 
        }
        if (i > j) { 
          return; 
        }
        if (l >= i && r <= j){
            if (USE_LAZY_FLAG == true){
                lazy_[p] = value;
                has_lazy_[p] = true;
                propagate(p, l, r);
                return;
            }
            else if (l == r){
                st_[p].applyUpdate(value, 1);
                return;
            }
        }
        int mid = (l + r) >> 1;
        update(p << 1, l, mid, i, std::min(mid, j), value);
        update(p << 1 | 1, mid + 1, r, std::max(mid + 1, i), j, value);
        st_[p] = st_[p << 1] + st_[p << 1 | 1];
    };

    /* 区间查询。*/
    NODE_TYPE query(int p, int l, int r, int i, int j) {
        if constexpr (USE_LAZY_FLAG) { 
            propagate(p, l, r); 
        }
        if (l >= i && r <= j) { 
            return st_[p]; 
        }
        int mid = (l + r) >> 1;
        if (j <= mid) {
            return query(p << 1, l, mid, i, j);
        }
        else if (i > mid) {
            return query(p << 1 | 1, mid + 1, r, i, j);
        }
        else {
            return (query(p << 1, l, mid, i, mid) + query(p << 1 | 1, mid + 1, r, mid + 1, j));
        }
    }

    /* 初始化构造。*/
    void build(const std::vector<NODE_TYPE>& s, int p, int l, int r) {
        if (l == r) { st_[p] = s[l]; }
        else {
            int mid = (l + r) >> 1;
            build(s, p << 1, l, mid);
            build(s, p << 1 | 1, mid + 1, r);
            st_[p] = st_[p << 1] + st_[p << 1 | 1];
        }
    }

    /* 懒人标记向下传播。*/
    void propagate(int p, int l, int r) {
        if (has_lazy_[p] == true) {
            st_[p].applyUpdate(lazy_[p], r - l + 1);
            if (l != r) {
                lazy_[p << 1].mergeLazyMarks(lazy_[p], r - l + 1);
                lazy_[p << 1 | 1].mergeLazyMarks(lazy_[p], r - l + 1);
                has_lazy_[p << 1] = has_lazy_[p << 1 | 1] = true;
            }
            has_lazy_[p] = false;
            lazy_[p].clear();
        }
    }


};


using StaticSegTree = StaticSegmentTree<true>;
using StaticSegNode = StaticSegmentTreeNode;
/*
  ToDoList:

*/