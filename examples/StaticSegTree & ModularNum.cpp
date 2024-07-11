/* This solution is for the problem: https://atcoder.jp/contests/abc357/tasks/abc357_f */
/* 在涉及到编码的地方，已经增加了中文注释 */

#include <bits/stdc++.h>


template<typename T, typename U, typename V>
inline T fastPower(T a, U b, const V& mod) {
	assert(b >= 0);
	T res = 1;
	for (; b > 0; a = 1ll * a * a % mod, b >>= 1) {
		if (b & 1) {
			res = 1ll * res * a % mod;
		}
	}
	return res;
}

template<typename T, typename U>
inline T fermatInverse(const T& a, const U& mod) { return fastPower(a, mod - 2, mod); }


template <typename T, typename U>
inline T extendEuclidInverse(T a, U mod) {
	T x = 0, y = 1;
	while (a != 0) {
		T q = mod / a;
		mod -= q * a; std::swap(a, mod);
		x -= q * y; std::swap(x, y);
	}
	assert(mod == 1);
	return x;
}

template<typename T>
class ModInt {
public:
	using Type = typename std::decay<decltype(T::value)>::type;
	ModInt() : value_(0) {}

	ModInt(const long long& value) : value_(normalize(value)) {}

	ModInt& operator += (const ModInt& other) { return value_ = normalize(value_ + other.value_), *this; }
	ModInt& operator -= (const ModInt& other) { return value_ = normalize(value_ - other.value_), *this; }
	ModInt& operator ++ () { return normalize(++value_), *this; }
	ModInt& operator -- () { return normalize(--value_), *this; }
	ModInt  operator ++ (int) { ModInt res{ *this }; return *this += 1, res; }
	ModInt  operator -- (int) { ModInt res(*this); return *this -= 1, res; }
	template<typename U> ModInt& operator += (const U& other) { return *this += ModInt(other); }
	template<typename U> ModInt& operator -= (const U& other) { return *this -= ModInt(other); }

	template <typename U = T>
	typename std::enable_if<std::is_same<typename ModInt<U>::Type, int>::value, ModInt>::type& operator *= (const ModInt& other) {
		value_ = normalize(static_cast<long long>(value_) * static_cast<long long>(other.value_));
		return *this;
	}

	template <typename U = T>
	typename std::enable_if<std::is_same<typename ModInt<U>::Type, long long>::value, ModInt>::type& operator *= (const ModInt& other) {
		long long q = static_cast<long long>(static_cast<long double>(value_) * other.value_ / getModValue());
		value_ = normalize(value_ * other.value - q * getModValue());
		return *this;
	}

	ModInt& operator /= (const ModInt& other) {
		//return *this *= ModInt(fermatInverse(other.value_, getModValue()));/*Fermat Inverse requires a prime Mod value!!*/
		return *this *= ModInt(extendEuclidInverse(other.value_, getModValue()));
	}

	template<typename U>
	friend bool operator == (const ModInt<U>& lhs, const ModInt<U>& rhs);

	template <typename U>
	friend bool operator < (const ModInt<U>& lhs, const ModInt<U>& rhs);

	template<typename U>
	friend std::ostream& operator << (std::ostream& os, const ModInt<U>& number) {
		os << number.value_;
		return os;
	}

	template<typename U>
	friend std::istream& operator >> (std::istream& is, ModInt<U>& number) {
		typename std::common_type<typename ModInt<U>::Type, long long>::type value;
		is >> value;
		number.value_ = normalize(value);
		return is;
	}

	template<typename U>
	static Type normalize(const U& value) {
		Type res = static_cast<Type> (value % getModValue());
		res = (res < 0 ? res + getModValue() : res > getModValue() ? res - getModValue() : res);
		return res;
	}

	//Type operator () () const { return this->value_; }

	template<typename U, typename V>
	static U power(U a, V b) {
		assert(b >= 0);
		U res = 1;
		for (; b > 0; a = a * a, b >>= 1) {
			if (b & 1) {
				res = res * a;
			}
		}
		return res;
	}


    ModInt<T> power(long long b){
        ModInt<T> res = ModInt::power(*this, b);
        return res;
    }

private:
	Type value_;
	constexpr static Type getModValue() { return T::value; }

};

template <typename T>             bool operator == (const ModInt<T>& lhs, const ModInt<T>& rhs) { return lhs.value_ == rhs.value_; }
template <typename T, typename U> bool operator == (const ModInt<T>& lhs, U rhs) { return lhs == ModInt<T>(rhs); }
template <typename T, typename U> bool operator == (U lhs, const ModInt<T>& rhs) { return ModInt<T>(lhs) == rhs; }

template <typename T>             bool operator != (const ModInt<T>& lhs, const ModInt<T>& rhs) { return !(lhs == rhs); }
template <typename T, typename U> bool operator != (const ModInt<T>& lhs, U rhs) { return !(lhs == rhs); }
template <typename T, typename U> bool operator != (U lhs, const ModInt<T>& rhs) { return !(lhs == rhs); }

template <typename T> ModInt<T>             operator + (const ModInt<T>& lhs, const ModInt<T>& rhs) { return ModInt<T>(lhs) += rhs; }
template <typename T, typename U> ModInt<T> operator + (const ModInt<T>& lhs, U rhs) { return ModInt<T>(lhs) += rhs; }
template <typename T, typename U> ModInt<T> operator + (U lhs, const ModInt<T>& rhs) { return ModInt<T>(lhs) += rhs; }

template <typename T>             ModInt<T> operator - (const ModInt<T>& lhs, const ModInt<T>& rhs) { return ModInt<T>(lhs) -= rhs; }
template <typename T, typename U> ModInt<T> operator - (const ModInt<T>& lhs, U rhs) { return ModInt<T>(lhs) -= rhs; }
template <typename T, typename U> ModInt<T> operator - (U lhs, const ModInt<T>& rhs) { return ModInt<T>(lhs) -= rhs; }

template <typename T>             ModInt<T> operator * (const ModInt<T>& lhs, const ModInt<T>& rhs) { return ModInt<T>(lhs) *= rhs; }
template <typename T, typename U> ModInt<T> operator * (const ModInt<T>& lhs, U rhs) { return ModInt<T>(lhs) *= rhs; }
template <typename T, typename U> ModInt<T> operator * (U lhs, const ModInt<T>& rhs) { return ModInt<T>(lhs) *= rhs; }

template <typename T>             ModInt<T> operator / (const ModInt<T>& lhs, const ModInt<T>& rhs) { return ModInt<T>(lhs) /= rhs; }
template <typename T, typename U> ModInt<T> operator / (const ModInt<T>& lhs, U rhs) { return ModInt<T>(lhs) /= rhs; }
template <typename T, typename U> ModInt<T> operator / (U lhs, const ModInt<T>& rhs) { return ModInt<T>(lhs) /= rhs; }

template <typename T> bool operator < (const ModInt<T>& lhs, const ModInt<T>& rhs) { return lhs.value_ < rhs.value_; }

constexpr int mod = (int)(998244353);   /* Set the correct mod number */
using MInt = ModInt<std::integral_constant<std::decay<decltype(mod)>::type, mod>>;



/*
  该节点定义在区间上的操作，可以根据输入的类型重写该结构体。
*/
struct UpdateNode {

    /* 自定义区间要执行的操作变量。*/
    /* 定义了两种要执行的操作，增加数组a中的元素数值，增加数组b中的元素数值*/
    MInt add_a;
    MInt add_b;

    /* 自定义初始化构造函数。*/
    /* 养成初始化列表构造的习惯 */
    UpdateNode(MInt aa = 0, MInt bb = 0): add_a(aa), add_b(bb) {

    }


    /*  懒人标记向下传递时调用，涉及区间操作必须实现。*/
    /* 区间向下传递时，将之前的修改传递到子区间 */
    void mergeLazyMarks(const UpdateNode& parent_node, int segment_length) {
        add_a += parent_node.add_a;
        add_b += parent_node.add_b;
    }


    /* 清除懒人标记，涉及区间操作必须实现。 */
    /* 当前区间的修改在线段树上已经被更新，将该节点置空 */
    void clear() {
        add_a = add_b = 0;
    }
};





struct StaticSegmentTreeNode {

    /* 在区间上要维护的变量值，必须实现。*/
    /* 线段树区间上维护三个值，该区间内数组a的和，区间内数组b的和，以及a和b元素相乘后取模的结果sum */
    MInt sum_a;
    MInt sum_b;
    MInt sum;


    /* 构造函数，必须实现。*/
    explicit StaticSegmentTreeNode(MInt value = 0ll): sum(value){}


    /* 重载左右孩子区间合并操作，必须实现。*/
    /* 根据题意，推理出线段树中区间合并的操作 */
    friend StaticSegmentTreeNode operator + (const StaticSegmentTreeNode& a, const StaticSegmentTreeNode& b) {
        StaticSegmentTreeNode res{a.sum + b.sum};
        res.sum_a = a.sum_a + b.sum_a;
        res.sum_b = a.sum_b + b.sum_b;
        return res;
    }


    /* 区间更新方法，必须实现。*/
    /* 当前线段树维护的区间需要更新，将节点中要修改的数值应用到线段树区间维护的参数上 */
    void applyUpdate(const UpdateNode& value, int segment_length) {
        sum += value.add_a * sum_b;
        sum += value.add_b * sum_a;
        sum += value.add_a * value.add_b * segment_length;
        sum_a += value.add_a * segment_length;
        sum_b += value.add_b * segment_length;
    }


};






template<const bool USE_LAZY_FLAG>
class StaticSegmentTree {
    using LAZY_TYPE      =      UpdateNode;
    using NODE_TYPE      =      StaticSegmentTreeNode;


public:
    constexpr explicit StaticSegmentTree(unsigned int n) : n_(n) {
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


using StaticSegTree = StaticSegmentTree<true>;  /* 需要使用区间操作，设置为true */
using StaticSegNode = StaticSegmentTreeNode;


using namespace std;


void solve() {
    int n, q;
    cin >> n >> q;

    vector<StaticSegNode> a(n + 1);
    for (int i = 1; i <= n; ++i){
        cin >> a[i].sum_a;
    }
    for (int i = 1; i <= n; ++i){
        cin >> a[i].sum_b;
        a[i].sum = a[i].sum_a * a[i].sum_b;
    }
    StaticSegTree st(a);

    while (q --){
        int t, l, r;
        cin >> t >> l >> r;
        if (t == 1){
            int x;
            cin >> x;
            st.update(l, r, {x, 0});
        }
        else if (t == 2){
            int x;
            cin >> x;
            st.update(l, r, {0, x});
        }
        else{
            cout << st.query(l, r).sum << '\n';
        }
    }
}



int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

    solve();

    return 0;
}
