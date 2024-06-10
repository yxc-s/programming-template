template<typename T, typename U, typename V>
T fastPower(T a, U b, const V& mod) {
    assert(b >= 0);
	T res = 1; 
    for (; b > 0; a = a * a % mod, b >>= 1){
        if (b & 1){
            res = res * a % mod; 
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
class ModInt{
public: 
    using Type = typename std::decay<decltype(T::value)>::type;
    ModInt(): value_(0){}
    
    ModInt(const long long& value): value_(normalize(value)){}

    ModInt& operator += (const ModInt& other)                   { return value_ = normalize(value_ + other.value_), *this; }
    ModInt& operator -= (const ModInt& other)                   { return value_ = normalize(value_ - other.value_), *this; }
    ModInt& operator ++ ()                                      { return normalize(++ value_), *this; }
    ModInt& operator -- ()                                      { return normalize(-- value_), *this; }
    ModInt  operator ++ (int)                                   { ModInt res{*this}; return *this += 1, res; }
    ModInt  operator -- (int)                                   { ModInt res(*this); return *this -= 1, res; }
    template<typename U> ModInt& operator += (const U& other)   { return *this += ModInt(other); }
    template<typename U> ModInt& operator -= (const U& other)   { return *this -= ModInt(other); }

    template <typename U = T>
    typename std::enable_if<std::is_same<typename ModInt<U>::Type, int>::value, ModInt>::type& operator *= (const ModInt& other) {
        value_ = normalize(static_cast<long long>(value_) * static_cast<long long>(other.value_));
        return *this;
    }

    template <typename U = T>
    typename std::enable_if<std::is_same<typename ModInt<U>::Type, long long>::value, ModInt>::type& operator *= (const ModInt& other) {
        value_ = fastPower(value_, other.value_, getModValue());
        return *this;
    }

    ModInt& operator /= (const ModInt& other){
        //return *this *= ModInt(fermatInverse(other.value_, getModValue()));/*Fermat Inverse requires a prime Mod value!!*/
        return *this *= ModInt(extendEuclidInverse(other.value_, getModValue()));
    }

    template<typename U>
    friend operator == (const ModInt<U>& lhs, const ModInt<U>& rhs);

    template <typename U>
    friend bool operator < (const ModInt<U>& lhs, const ModInt<U>& rhs); 

    template<typename U>
    friend std::ostream& operator << (std::ostream& os, const ModInt<U>& number){
        os << number.value_;
        return os;
    }
    
    template<typename U>
    friend std::istream& operator >> (std::istream& is, ModInt<U>& number){
        typename std::common_type<typename ModInt<U>::Type, long long>::type value;
        is >> value;
        number.value_ = normalize(value);
        return is;
    }

    template<typename U>
    static Type normalize(const U& value){
        Type res = static_cast<Type> (value % getModValue());
        res = (res < 0 ? res + getModValue() : res > getModValue() ? res - getModValue() : res);
        return res;
    }

    Type operator () () const { return this->value_; }
private:
    Type value_;
    constexpr static Type getModValue() { return T::value; }

};

template <typename T>             bool operator == (const ModInt<T>& lhs, const ModInt<T>& rhs)        { return lhs.value_ == rhs.value_; }
template <typename T, typename U> bool operator == (const ModInt<T>& lhs, U rhs)                       { return lhs == ModInt<T>(rhs); }
template <typename T, typename U> bool operator == (U lhs, const ModInt<T>& rhs)                       { return ModInt<T>(lhs) == rhs; }

template <typename T>             bool operator != (const ModInt<T>& lhs, const ModInt<T>& rhs)        { return !(lhs == rhs); }
template <typename T, typename U> bool operator != (const ModInt<T>& lhs, U rhs)                       { return !(lhs == rhs); }
template <typename T, typename U> bool operator != (U lhs, const ModInt<T>& rhs)                       { return !(lhs == rhs); }
 
template <typename T> ModInt<T>             operator + (const ModInt<T>& lhs, const ModInt<T>& rhs)    { return ModInt<T>(lhs) += rhs; }
template <typename T, typename U> ModInt<T> operator + (const ModInt<T>& lhs, U rhs)                   { return ModInt<T>(lhs) += rhs; }
template <typename T, typename U> ModInt<T> operator + (U lhs, const ModInt<T>& rhs)                   { return ModInt<T>(lhs) += rhs; }
  
template <typename T>             ModInt<T> operator - (const ModInt<T>& lhs, const ModInt<T>& rhs)    { return ModInt<T>(lhs) -= rhs; }
template <typename T, typename U> ModInt<T> operator - (const ModInt<T>& lhs, U rhs)                   { return ModInt<T>(lhs) -= rhs; }
template <typename T, typename U> ModInt<T> operator - (U lhs, const ModInt<T>& rhs)                   { return ModInt<T>(lhs) -= rhs; }
 
template <typename T>             ModInt<T> operator * (const ModInt<T>& lhs, const ModInt<T>& rhs)    { return ModInt<T>(lhs) *= rhs; }
template <typename T, typename U> ModInt<T> operator * (const ModInt<T>& lhs, U rhs)                   { return ModInt<T>(lhs) *= rhs; }
template <typename T, typename U> ModInt<T> operator * (U lhs, const ModInt<T>& rhs)                   { return ModInt<T>(lhs) *= rhs; }
 
template <typename T>             ModInt<T> operator / (const ModInt<T>& lhs, const ModInt<T>& rhs)    { return ModInt<T>(lhs) /= rhs; }
template <typename T, typename U> ModInt<T> operator / (const ModInt<T>& lhs, U rhs)                   { return ModInt<T>(lhs) /= rhs; }
template <typename T, typename U> ModInt<T> operator / (U lhs, const ModInt<T>& rhs)                   { return ModInt<T>(lhs) /= rhs; }

template <typename T> bool operator < (const ModInt<T>& lhs, const ModInt<T>& rhs)                     { return lhs.value_ < rhs.value_; }

constexpr int mod = (int)(1e9 + 7);
using MInt = ModInt<std::integral_constant<std::decay<decltype(mod)>::type, mod>>;

