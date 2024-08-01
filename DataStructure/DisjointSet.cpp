/*
 * DisjointSet(并查集)
 * 设计思想：保留了基本的合并查询功能，采用了宏定义的方式，可手动指定增加新功能。
 * 基于面向对象的编程思想，本方法尽可能多的隐藏了内部实现的细节，并且将必要的编程接口暴露在外部，并需要对这些接口进行直接的修改。
 * 在该设计中，需要修改的接口有：
 *                             USE_DSU_SET_ELEMENT：是否获取集合中的元素。
 *                                                  如果要保留并查集中每个集合中的具体元素，将该定义设置为true。
 *                             USE_DSU_WEIGHT：是否使用带权并查集。
 *                                             如果使用带权并查集，将该定义设置为true。
 *                                             并且实现两个友元函数mergeWeights和compressWeights。
 *                                             unionWeights：在两个集合合并时调用，需要手动实现初始化权重的细节。
 *                                             compressWeights()：在路径压缩时调用，需要手动实现权重更新的细节。
 *                             并查集的主体所有必备的方法都已实现，无需修改。
 *
 * gitHub(仓库地址): https://github.com/yxc-s/programming-template.git
 */











template<const bool USE_DSU_WEIGHT, const bool USE_DSU_SET_ELEMENT = false>
class DisjointSet {
    using this_dsu_type = DisjointSet<USE_DSU_WEIGHT, USE_DSU_SET_ELEMENT>;
    friend void unionWeights(DisjointSet& dsu, int x, int y, int px, int py, long long value);
    friend void compressWeights(DisjointSet& dsu, int x, int y);
public:
	/* 默认构造函数 */
    DisjointSet(unsigned int sz) :
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

    /* 默认拷贝构造，拷贝赋值，析构函数 */
    ~DisjointSet() = default;
    DisjointSet(const this_dsu_type& other) = default;
    this_dsu_type& operator =(const this_dsu_type& other) = default;

    /* 移动构造函数 */
    DisjointSet(this_dsu_type&& other) :
        sz_(other.sz_),
        num_sets_(other.num_sets_),
        fa_(std::move(other.fa_)),
		set_size_(std::move(other.set_size_)),
        weight_(std::move(other.weight_)),
        elements_(std::move(other.elements_)){}

    /* 移动赋值函数 */
    this_dsu_type& operator =(this_dsu_type&& other) {
        if (this != &other) {
            sz_ = other.sz_;
            num_sets_ = other.num_sets_;
            fa_ = std::move(other.fa_);
            set_size_ = std::move(other.set_size_);
            weight_ = std::move(other.weight_);
            elements_ = std::move(other.elements_);
        }
        return *this;
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
    int countSets() const noexcept {
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
    std::vector<int>& getSetElements(int x) {
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
