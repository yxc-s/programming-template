
/*
 * FenwickTree(树状数组)
 * 设计思想：基于二进制表示的下标的快速变换来实现前缀和的计算，适用于差分后的区间修改单点查询，单点修改区间查询。
 * 基于面向对象的编程思想，本方法尽可能多的隐藏了内部实现的细节，并且将必要的编程接口暴露在外部，并需要对这些接口进行直接的修改。
 * 在该设计中，基础的方法都已实现，如有需要用户可以自行在该结构上进行改进。
 *
 * gitHub(仓库地址): https://github.com/yxc-s/programming-template.git
 */










/*
  该树总是约定区间左端点从1开始(避免lowbit(0)的情况)。
  如果已有数组，建议使用已有的数组实例化对象，时间复杂度更低。
  注意初始化数组时容器为int和longlong的区别。
*/

class FenwickTree {
public:
    /* 按区间最大右端点下标构造。*/
    constexpr explicit FenwickTree(unsigned int n) : n_(n) {
        ft_.resize(n_);
    }

    /* 基于已有的数组构造。*/
    FenwickTree(const std::vector<long long>& f) {
        n_ = static_cast<int>(f.size());
        ft_.assign(n_, 0);
        for (int i = 1; i < n_; ++i) {
            ft_[i] += f[i];
            if (i + lowbit(i) < n_) {
                ft_[i + lowbit(i)] += ft_[i];
            }
        }
    }

    /* 按数组元素出现频次构造*/
    FenwickTree(const std::vector<int>& s) {
        n_ = *std::max_element(s.begin() + 1, s.end()) + 1;
        ft_.resize(n_);
        for (size_t i = 1; i < s.size(); ++i) {
            ft_[s[i]]++;
            if (s[i] + lowbit(s[i]) < n_) {
                ft_[s[i] + lowbit(s[i])] += ft_[s[i]];
            }
        }
    }

    /* 单点更新。 */
    void update(int pos, long long value) {
        while (pos < n_) {
            ft_[pos] += value;
            pos += lowbit(pos);
        }
    }

    /* 区间更新。*/
    inline void update(int l, int r, long long value) {
        assert(l > 0 && r >= l);
        update(l, value);
        update(r + 1, -value);
    }

    /* 单点查询。*/
    long long query(int p) {
        assert(p < n_);
        long long res = 0;
        while (p > 0) {
            res += ft_[p];
            p -= lowbit(p);
        }
        return res;
    }

    /* 区间查询。*/
    inline long long query(int l, int r) {
        assert(l <= r);
        return query(r) - query(l - 1);
    }


private:
    unsigned int n_;

    std::vector<long long> ft_;


private:
    inline int lowbit(int x) { return (x & (-x)); }

};
