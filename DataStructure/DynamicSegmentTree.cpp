
/*
 * DynamicSegmentTreeNode(动态线段树)
 * 设计思想：动态开点，适用于4 * n静态可能爆栈以及需要维护负数区间的情况。
 * 基于面向对象的编程思想，本方法尽可能多的隐藏了内部实现的细节，并且将必要的编程接口暴露在外部，并需要对这些接口进行直接的修改。
 * 在该设计中，需要修改的接口有：
 *                             UpdateNode：该结构体存储了区间上的操作类型：需要自定义结构体变量成员及构造函数；
 *                                         懒人标记的向下传递方法：mergeLazyMarks()；
 *                                         懒人标记被更新后的方法：clear()。
 *                             DynamicSegmentTreeNode: 需要在该结构体中写出要维护的区间参数，初始化函数；
 *                                                    该类重载了+来实现左右孩子区间合并：在重载函数中实现细节；
 *                                                    实现区间上更新懒人标记值的方法：applyUpdate（）；
 *                                                    建树必须指定模板参数True或False，代表是否使用懒人标记。
 *                             动态线段树的主体所有必备的方法都已实现，无需修改，有的题目有剪枝操作请自行添加实现。
 *
 * gitHub(仓库地址): https://github.com/yxc-s/programming-template.git
 */










/* 该节点定义在区间上的操作，可以根据输入的类型重写该结构体。*/
struct UpdateNode {
    /* 自定义区间要执行的操作变量。*/
    long long value;


    /* 自定义初始化构造函数。*/
    UpdateNode(long long value_ = 0ll): value(value_) {

    }


    /*  懒人标记向下传递时调用，涉及区间操作必须实现。 */
    void mergeLazyMarks(const UpdateNode& parent_node, int segment_length) {

    }


    /* 清除懒人标记，涉及区间操作必须实现。 */
    void clear() {
        
    }
};





struct DynamicSegmentTreeNode {

    /* 在区间上要维护的变量值，必须实现。*/
    long long sum;


    /* 构造函数，必须实现。*/
    DynamicSegmentTreeNode(long long value = 0ll): sum(value) {}


    /* 重载左右孩子区间合并操作，必须实现。*/
    friend DynamicSegmentTreeNode operator + (const DynamicSegmentTreeNode& a, const DynamicSegmentTreeNode& b) {
        DynamicSegmentTreeNode res{};

        return res;
    }


    /* 区间更新方法，必须实现。*/
    void applyUpdate(const UpdateNode& value, int segment_length) {

    }


};










/*
  动态开点线段树，query返回类型必须是NODE_TYPE类型，更新数值类型必须是UpdateNode。
  初始化模板必须指定true或者false。
  初始化形参必须包含要维护区间的左右两个端点值。
*/

template<typename T, typename U, const bool USE_LAZY_FLAG>
class DynamicSegmentTree {
    using LAZY_TYPE      =      UpdateNode;
    using NODE_TYPE      =      DynamicSegmentTreeNode;


public:
    DynamicSegmentTree() {
        st_.resize(2);
        lchild_.resize(2);
        rchild_.resize(2);
        if constexpr (USE_LAZY_FLAG){
            lazy_.resize(2);
            has_lazy_.resize(2);
        }
    }


    /* 单点更新。*/
    void update(int i, const LAZY_TYPE& value) {
        update(1, getLeftMost(), getRightMost(), i, value);
    }

    /* 更新区间值。*/
    void update(int i, int j, const LAZY_TYPE& value) {
        assert(USE_LAZY_FLAG == true);
        update(1, getLeftMost(), getRightMost(), i, j, value);
    }

    /* 获取区间节点。*/
    NODE_TYPE query(int i, int j) {
        return query(1, getLeftMost(), getRightMost(), i, j);
    }


private:
    std::vector<NODE_TYPE>                st_;
    std::vector<LAZY_TYPE>                lazy_;
    std::vector<int>                      lchild_;
    std::vector<int>                      rchild_;
    std::vector<bool>                     has_lazy_;

    constexpr static auto getRightMost() ->decltype(U::value){
        return U::value;
    }   

    constexpr static auto getLeftMost() -> decltype(T::value){
        return T::value;
    }

    /* 区间更新。*/
    void update(int p, int l, int r, int i, int j, const LAZY_TYPE& value) {
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
 
    /* 单点更新 */
    void update(int p, int l, int r, int pos, const LAZY_TYPE& value) {
        if (l == r) {
            st_[p].applyUpdate(value, 1);
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

    /* 区间查询 */
    NODE_TYPE query(int p, int l, int r, int i, int j) {
        if constexpr (USE_LAZY_FLAG) {
            propagate(p, l, r);
        }
        if (l >= i && r <= j){
            return st_[p];
        }
        int mid = (l + r) >> 1;
        if (j <= mid) {
            return query(lchild_[p], l, mid, i, j);
        }
        else if (i > mid) {
            return query(rchild_[p], mid + 1, r, i, j);
        }
        else {
            return (query(lchild_[p], l, mid, i, mid) + query(rchild_[p], mid + 1, r, mid + 1, j));
        }
    }

    /* 懒人标记向下传播。*/
    void propagate(int p, int l, int r) {
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

    /* 使用该函数完全动态分配空间，一般空间比直接分配内存节省百分之50。*/
    void checkNodeIndex(int& index) {
        if (index == 0) {
            index = static_cast<int> (st_.size());
            st_.emplace_back();
            lchild_.emplace_back(0);
            rchild_.emplace_back(0);
            if constexpr(USE_LAZY_FLAG){
                lazy_.emplace_back();
                has_lazy_.emplace_back(false);
            }
        }
    }


};


constexpr const long long LEFT_MOST      =  1;
constexpr const long long RIGHT_MOST     =  1e5 + 10;
constexpr const bool      USE_LAZY_FLAG  =  true;
using DynSegTree = DynamicSegmentTree<std::integral_constant<decltype(LEFT_MOST), LEFT_MOST>, 
    std::integral_constant<decltype(RIGHT_MOST), RIGHT_MOST>, USE_LAZY_FLAG>;
using DynSegNode = DynamicSegmentTreeNode;


/*
  ToDoList: 负数区间的维护，在计算mid时方式是否需要调整？
*/
