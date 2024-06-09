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
 * gitHub(仓库地址): https://github.com/100000000000000000000000000000000/programming-template.git
 */










#define USE_DSU_SET_ELEMENT 0
#define USE_DSU_WEIGHT 1

class DisjointSet {
    friend void unionWeights(DisjointSet& dsu, int x, int y, int px, int py, long long value);
    friend void compressWeights(DisjointSet& dsu, int x, int y);
public:
    DisjointSet(int sz) :
        sz_(sz),
        num_sets_(sz)
    {
        fa_.resize(sz_);
        std::iota(fa_.begin(), fa_.end(), 0);
        set_size_.assign(sz_, 1);

        #if USE_DSU_WEIGHT
            weight_.resize(sz_);
        #endif

        #if USE_DSU_SET_ELEMENT
            elements_.resize(sz_);
            for (int i = 0; i < sz_; ++i) {
                elements_[i].emplace_back(i);
            }
        #endif

    }

    inline int findSet(int x) {
        if (fa_[x] == x){
            return x;
        }
        int par = fa_[x];
        fa_[x] = findSet(fa_[x]);
        #if USE_DSU_WEIGHT
        compressWeights(*this, x, par);
        #endif
        return fa_[x];
    }

    inline int getSetSize(int x) {
        return set_size_[findSet(x)];
    }

    #if USE_DSU_WEIGHT
    long long getWeight(int x) {
        findSet(x);
        return weight_[x];
    }
    #endif

    inline int countSets()  {
        return num_sets_;
    }
    inline bool isSameSet(int x, int y) {
        return findSet(x) == findSet(y);
    }

    bool unionSet(int x, int y, long long value = 0) {
        int px = findSet(x);
        int py = findSet(y);
        if (px == py) {
            return false;
        }
        fa_[px] = py;
        num_sets_--;
        #if USE_DSU_WEIGHT
            unionWeights(*this, x, y, px, py, value);
        #endif
        set_size_[py] += set_size_[px];

        #if USE_DSU_SET_ELEMENT
            elements_[y].insert(elements_[y].end(), elements_[x].begin(), elements_[x].end());
            elements_[x].clear();
        #endif
        
        return true;
    }

    #if USE_DSU_SET_ELEMENT
    inline std::vector<int> getSetElements(int x) {
        return elements_[findSet(x)];
    }
    #endif


private:
    int                             sz_;
    int                             num_sets_;
    std::vector<int>                fa_;
    std::vector<int>                set_size_;
    std::vector<long long>          weight_;


};

/*
  这里x和y是操作时的集合节点，要把x所在集合合并到y。px和py是前两者的集合代表元素。
  value是一个缺省值，代表指定x->y的权值，默认为0。
*/
void unionWeights(DisjointSet& dsu, int x, int y, int px, int py, long long value = 0){

}

/*
  这里是路径压缩时的更新权重操作，y是x压缩前的直接父亲节点。
*/
void compressWeights(DisjointSet& dsu, int x, int y){

}