/*
 * PrefixTree(字典树)
 * 设计思想：字典树设计的思想是基于动态开点的m叉树。
 * 基于面向对象的编程思想，本方法尽可能多的隐藏了内部实现的细节，并且将必要的编程接口暴露在外部，并需要对这些接口进行直接的修改。
 * 在该设计中，基础的方法都已经实现，建议用户根据自己的需求自己在字典树主体或者节点类型中实现对应的逻辑。
 * 在该设计中：实现了基于字符串的插入删除，查询前缀数量及相同字符串数量。
 *             实现了基于int型变量的二进制插入删除，以及最大异或值的查询。
 *                        
 *
 * gitHub(仓库地址): https://github.com/yxc-s/programming-template.git
 * 
 */










/*
  字典树的树节点结构。
  每个节点除了指针还有两个变量来记数，分别表示经过当前节点的数据数量pass_count_和以当前节点为结尾的数据数量end_count_。
*/
template<unsigned int ALPHABET_SIZE>
class PrefixTreeNode {
public:
    PrefixTreeNode() : pass_count_(0), end_count_(0), next_{} {}
    unsigned int  pass_count_;
    unsigned int  end_count_;
    std::array<int, ALPHABET_SIZE> next_;
};



/*
  字典树，实例化时需要指定起始字符和节点中孩子数量。
  使用示例：Trie trie; vector<Trie> trie(2);
  注意：使用前务必在类的末尾处指定起始字符和数节点中孩子数量。
*/

template<typename START_ALPHABET, const unsigned int ALPHABET_SIZE>
class PrefixTree {
public:
    constexpr PrefixTree() : root_(0) {
        trie_.resize(1);
    }

    using Type = decltype(START_ALPHABET::value);


    /* 将int型变量以二进制形式插入到字典树中。 */
    void insert(const int& x) {
        int cur = root_;
        for (int i = 30; i >= 0; --i) {
            int p = (x >> i) & 1;
            if (trie_[cur].next_[p]== 0) {
                trie_[cur].next_[p] = (int)trie_.size();
                trie_.emplace_back();
            }
            cur = trie_[cur].next_[p];
            trie_[cur].pass_count_ ++;
        }
        trie_[cur].end_count_ ++;
    }

    /* 从字典树中移除二进制形式的int型变量值。 */
    void erase(const int& x) {
        int cur = root_;
        for (int i = 30; i >= 0; --i) {
            int p = (x >> i) & 1;
            cur = trie_[cur].next_[p];
            assert(trie_[cur].pass_count_ > 0);
            trie_[cur].pass_count_ --;
        }
        assert(trie_[cur].end_count_ > 0);
        trie_[cur].end_count_ --;
    }

    /* 获取字典树中异或最大值。 */
    int getMaxXor(const int& x) {
        int cur = root_;
        int res = 0;
        for (int i = 30; i >= 0; --i) {
            int p = (x >> i) & 1;
            if (trie_[cur].next_[!p] && trie_[trie_[cur].next_[!p]].pass_count_) {
                res += (1 << i);
                cur = trie_[cur].next_[!p];
            }
            else if (trie_[cur].next_[p] && trie_[trie_[cur].next_[p]].pass_count_) {
                cur = trie_[cur].next_[p];
            }
            else {
                break;
            }
        }
        return res;
    }

    /* 字典树中插入字符串。 */
    void insert(const std::string& s) {
        int cur = root_;
        for (const auto& x : s) {
            int p = x - getStartAlpha();
            if (trie_[cur].next_[p] == 0) {
                trie_[cur].next_[p] = (int)trie_.size();
                trie_.emplace_back();
            }
            cur = trie_[cur].next_[p];
            trie_[cur].pass_count_ ++;
        }
        assert(trie_[cur].end_count_ > 0);
        trie_[cur].end_count_ ++;
    }

    /* 从字典树中移除字符串。*/
    void erase(const std::string& s) {
        int cur = root_;
        for (const auto& x : s) {
            int p = x - getStartAlpha();
            cur = trie_[cur].next_[p];
            assert(trie_[cur].pass_count_ > 0);
            trie_[cur].pass_count_ --;
        }
        assert(trie_[cur].end_count_ > 0);
        trie_[cur].end_count_ --;
    }

    /* 统计字典树中s的数量。 */
    template<typename T>
    int countUnique(const T& s) {
        int cur = getLastPointer(s);
        return cur == 0 ? cur : trie_[cur].end_count_;
    }

    /* 统计字典树中前缀包含s的数量。 */
    template<typename T>
    int countPrefix(const T& s) {
        int cur = getLastPointer(s);
        return cur == 0 ? cur : trie_[cur].pass_count_;
    }



private:
    int  root_;

    std::vector<PrefixTreeNode<ALPHABET_SIZE>> trie_;
    
    constexpr static Type getStartAlpha() { return  START_ALPHABET::value;}

    int getLastPointer(const int& x) {
        int cur = root_;
        for (int i = 30; i >= 0; --i) {
            int p = (x >> i) & 1;
            if (trie_[cur].next_[p] == 0) {
                return 0;
            }
            cur = trie_[cur].next_[p];
        }
        return cur;
    }

    int getLastPointer(const std::string& s) {
        int cur = root_;
        for (const auto& x : s) {
            int p = x - getStartAlpha();
            if (trie_[cur].next_[p] == 0) {
                return 0;
            }
            cur = trie_[cur].next_[p];
        }
        return cur;
    }
};

// constexpr char START_ALPHABET = 'a';
// constexpr int TRIE_NODE_SIZE = 26;
constexpr int START_ALPHABET = 0;
constexpr int TRIE_NODE_SIZE = 2;
using Trie = PrefixTree<std::integral_constant<decltype(START_ALPHABET), START_ALPHABET>, TRIE_NODE_SIZE>;


/*
TODO:
*/