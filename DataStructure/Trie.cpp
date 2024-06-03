
/*
 * Trie(字典树)
 * 设计思想：字典树设计的思想是基于动态开点的m叉树。
 * 基于面向对象的编程思想，本方法尽可能多的隐藏了内部实现的细节，并且将必要的编程接口暴露在外部，并需要对这些接口进行直接的修改。
 * 在该设计中，基础的方法都已经实现，建议用户根据自己的需求自己在字典树主体或者节点类型中实现对应的逻辑。
 * 在该设计中：实现了基于字符串的插入删除，查询前缀数量及相同字符串数量。
 *             实现了基于int型变量的二进制插入删除，以及最大异或值的查询。
 *                        
 *
 * gitHub(仓库地址): https://github.com/100000000000000000000000000000000/programming-template.git
 * 
 */










template<const unsigned int ALPHABET_SIZE> class Trie;
/*
  Trie类的节点，每个节点有若干个指针，在实例化类对象时指定。
  每个节点除了指针还有两个变量来记数，分别表示经过当前节点的数据数量pass_count_和以当前节点为结尾的数据数量end_count_。
*/
template<unsigned int ALPHABET_SIZE>
class TrieNode {
    friend class Trie<ALPHABET_SIZE>;
public:
    TrieNode() : pass_count_(0), end_count_(0), next_{} {}

private:
    unsigned int  pass_count_;
    unsigned int  end_count_;

    std::array<int, ALPHABET_SIZE> next_;
};



/*
  字典树，实例化时需要传递一个非负值作为节点的指针数量，初始化必须携带起始字符，如果是int的二进制数据建树初始化列表可以输入0。
  使用示例：Trie<26> trie('a'); Trie<2> trie(0);
  注意：如果将数字作为二进制来存储到字典树中，只支持int型。
*/

template<const unsigned int ALPHABET_SIZE>
class Trie {
public:
    Trie(char start) : start_(start), root_(0) {
        trie_.resize(1);
    }


    /*
      将int型变量以二进制形式插入到字典树中。
    */
    void insert(int x) {
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

    /*
      从字典树中移除二进制形式的int型变量值。
    */
    void erase(int x) {
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

    /*
      获取字典树中异或最大值。
    */

    int getMaxXor(int x) {
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

    /*
      字典树中插入字符串。
    */
    void insert(const std::string& s) {
        int cur = root_;
        for (const auto& x : s) {
            int p = x - start_;
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

    /*
      从字典树中移除字符串。
    */
    void erase(const std::string& s) {
        int cur = root_;
        for (const auto& x : s) {
            int p = x - start_;
            cur = trie_[cur].next_[p];
            assert(trie_[cur].pass_count_ > 0);
            trie_[cur].pass_count_ --;
        }
        assert(trie_[cur].end_count_ > 0);
        trie_[cur].end_count_ --;
    }

    /*
      统计字典树中s的数量。
    */
    template<typename T>
    int countUnique(const T& s) {
        int cur = getLastPointer(s);
        return cur == 0 ? cur : trie_[cur].end_count_;
    }

    /*
      统计字典树中前缀包含s的数量。
    */
    template<typename T>
    int countPrefix(const T& s) {
        int cur = getLastPointer(s);
        return cur == 0 ? cur : trie_[cur].pass_count_;
    }



private:
    char start_;
    int  root_;

    std::vector<TrieNode<ALPHABET_SIZE>> trie_;


    inline int getLastPointer(const int& x) {
        int cur = root_;
        for (int i = 30; i >= 0; --i) {
            int p = (x >> i) & 1;
            if (trie_[cur][p] == 0) {
                return 0;
            }
            cur = trie_[cur][p];
        }
        return cur;
    }

    inline int getLastPointer(const std::string& s) {
        int cur = root_;
        for (const auto& x : s) {
            int p = x - start_;
            if (trie_[cur][p] == 0) {
                return 0;
            }
            cur = trie_[cur][p];
        }
        return cur;
    }
};
