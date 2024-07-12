/* problems : https://codeforces.com/contest/546/problem/D */



/* solution A Time : 2624ms
   solution B Time : 1030ms
*/

/********************** solution A **********************/
#if true
#include <bits/stdc++.h>


template<typename T, typename U>
class PrimeNumberManipulator{
public:
    template<typename V = T>
    using EnableIfInt    =   typename std::enable_if<std::is_same<V, int>::value, V>::type;
    using primeType      =   typename std::decay<decltype(T::value)>::type;
    using rangePrimeType =   typename std::decay<decltype(U::value)>::type;

    PrimeNumberManipulator(EnableIfInt<primeType>* = 0, EnableIfInt<rangePrimeType>* = 0){}

    
    /* 线性筛，再使用其他的涉及到单个数的质因子操作时，需要先调用该接口 */
    void sievePrimes(){
        if (prime_values_.empty()){
            bs_.set();
            bs_[0] = bs_[1] = 0;
            for (int i = 2; i <= getPrimeLimit(); ++i){
                if (bs_[i]){
                    prime_values_.emplace_back(i);
                }
                for (const auto& prime : prime_values_){
                    if (1ll * i * prime > getPrimeLimit()){
                        break;
                    }
                    bs_[i * prime] = 0;
                    if (i % prime == 0){
                        break;
                    }
                }
            }
        }
    }

    /* 获取质数列表 */
    std::vector<int>& getPrimesArray(){
        assert(!prime_values_.empty());
        return prime_values_;
    }

    /* 获取范围内每个数的质因子数量, 不需要先线性筛 */
    std::vector<int>& countRangePrimes(){
        range_prime_nums_.resize(getRangePrimeLimit() + 1);
        for (int i = 2; i <= getRangePrimeLimit(); ++i){
            if (range_prime_nums_[i] == 0){
                for (int j = i; j <= getRangePrimeLimit(); j += i){
                    int num = j; 
                    while (num % i == 0){
                        range_prime_nums_[j] ++;
                        num /= i;
                    }
                }
            }
        }
        return range_prime_nums_;
    }

    /* 获取[1, x]范围内每个数的质因子及其数量, 不需要先线性筛 */
    std::vector<std::vector<std::pair<int, int>>>& getRangePrimes(){
        range_prime_values_.resize(getRangePrimeLimit() + 1);
        for (int i = 2; i <= getRangePrimeLimit(); ++i){
            if (range_prime_values_[i].empty()){
                for (int j = i; j <= getRangePrimeLimit(); j += i){
                    int cnt = 0;
                    int num = j; 
                    while (num % i == 0){
                        cnt ++;
                        num /= i;
                    }
                    range_prime_values_[j].emplace_back(i, cnt);
                }
            }
        }
        return range_prime_values_;
    }

     /* 统计单个数中不相同的质数数量 */
    template<typename V>
    int countUniquePrimes(V x){
        assert(!prime_values_.empty());
        int ans = 0;
        for (const auto& prime : prime_values_){
            if (prime > x / prime){
                break;
            }
            ans ++;
            while (x % prime == 0){
                x /= prime;
            }
         }
        return ans + (x > 1);
    }

    /* 统计单个数中所有的质数数量 */
    template<typename V>
    int countAllPrimes(V x){
        assert(!prime_values_.empty());
        int ans = 0;
        for (const auto& prime : prime_values_){
            if (prime > x / prime){
                break;
            }
            while (x % prime == 0){
                x /= prime;
                ans ++;
            }
        }
        return ans + (x > 1);
    }

    /* 获取单个数中的质因子及出现次数 */
    template <typename V>
    std::vector<std::pair<V, int>> getUniquePrimes(V x){
        assert(!prime_values_.empty());
        std::vector<std::pair<V, int>> res;
        for (const auto& prime : prime_values_){
            if (1ll * prime * prime > x){
                break;
            }
            int cnt = 0;
            while (x % prime == 0){
                x /= prime;
                cnt ++;
            }
            if (cnt){
                res.emplace_back(prime, cnt);
            }
        }
        if (x > 1){
            res.emplace_back(x, 1);
        }
        return res;
    }

    /* 判定质数，当筛选范围为1e8时，最多可判断1e16的质数*/
    template<typename V>
    bool isPrime(V x){
        assert(!prime_values_.empty());
        if (x <= getPrimeLimit()){
            return bs_[x];
        }
        for (const auto& prime : prime_values_){
            if (x % prime == 0){
                return false;
            }
        }
        return true;
    }

    /* 统计因子数量 */
    template<typename V>
    int countDivisors(V x){
        assert(!prime_values_.empty());
        int ans = 1;
        for (const auto& prime : prime_values_){
            if (prime > x / prime){
                break;
            }
            int power = 0;
            while (x % prime == 0){
                x /= prime;
                power ++;
            }
            ans *= power + 1;
        }
        return x > 1 ? ans * 2 : ans;
    }


private:
    std::bitset<100000010>                            bs_;
    std::vector<int>                                  prime_values_;
    std::vector<std::vector<std::pair<int, int>>>     range_prime_values_;
    std::vector<int>                                  range_prime_nums_;


private:
    constexpr primeType      getPrimeLimit()      { return T::value; }
    constexpr rangePrimeType getRangePrimeLimit() { return U::value; }
};


constexpr int  N = (int)5e6;
constexpr int  PRIME_VALUE_LIMIT   =   N + 10;              /*最多可以到1e8*/
constexpr int  RANGE_VALUE_LIMIT   =   N + 10;        /*目测最多到5e6*/
using Primer = PrimeNumberManipulator<std::integral_constant<decltype(PRIME_VALUE_LIMIT), PRIME_VALUE_LIMIT>, 
                                     std::integral_constant<decltype(PRIME_VALUE_LIMIT), RANGE_VALUE_LIMIT>>;

Primer primer;



using namespace std;

array<long long, N + 10> pref = {};

void preProcess() {
    primer.sievePrimes();
    for (int i = 1; i <= N; ++i){
        pref[i] = pref[i - 1] + primer.countAllPrimes(i);
    }
}


void solve(){
    int a, b;
    cin >> a >> b;

    cout << pref[a] - pref[b] << '\n';
}


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int tc = 1;

    preProcess();

    if constexpr (true){
        cin >> tc;
    }

    while (tc--) {
        solve();
    }
    return 0;
}



#else
/********************** solution B **********************/

#include <bits/stdc++.h>


template<typename T, typename U>
class PrimeNumberManipulator{
public:
    template<typename V = T>
    using EnableIfInt    =   typename std::enable_if<std::is_same<V, int>::value, V>::type;
    using primeType      =   typename std::decay<decltype(T::value)>::type;
    using rangePrimeType =   typename std::decay<decltype(U::value)>::type;

    PrimeNumberManipulator(EnableIfInt<primeType>* = 0, EnableIfInt<rangePrimeType>* = 0){}

    
    /* 线性筛，再使用其他的涉及到单个数的质因子操作时，需要先调用该接口 */
    void sievePrimes(){
        if (prime_values_.empty()){
            bs_.set();
            bs_[0] = bs_[1] = 0;
            for (int i = 2; i <= getPrimeLimit(); ++i){
                if (bs_[i]){
                    prime_values_.emplace_back(i);
                }
                for (const auto& prime : prime_values_){
                    if (1ll * i * prime > getPrimeLimit()){
                        break;
                    }
                    bs_[i * prime] = 0;
                    if (i % prime == 0){
                        break;
                    }
                }
            }
        }
    }

    /* 获取质数列表 */
    std::vector<int>& getPrimesArray(){
        assert(!prime_values_.empty());
        return prime_values_;
    }

    /* 获取范围内每个数的质因子数量, 不需要先线性筛 */
    std::vector<int>& countRangePrimes(){
        range_prime_nums_.resize(getRangePrimeLimit() + 1);
        for (int i = 2; i <= getRangePrimeLimit(); ++i){
            if (range_prime_nums_[i] == 0){
                for (int j = i; j <= getRangePrimeLimit(); j += i){
                    int num = j; 
                    while (num % i == 0){
                        range_prime_nums_[j] ++;
                        num /= i;
                    }
                }
            }
        }
        return range_prime_nums_;
    }

    /* 获取[1, x]范围内每个数的质因子及其数量, 不需要先线性筛 */
    std::vector<std::vector<std::pair<int, int>>>& getRangePrimes(){
        range_prime_values_.resize(getRangePrimeLimit() + 1);
        for (int i = 2; i <= getRangePrimeLimit(); ++i){
            if (range_prime_values_[i].empty()){
                for (int j = i; j <= getRangePrimeLimit(); j += i){
                    int cnt = 0;
                    int num = j; 
                    while (num % i == 0){
                        cnt ++;
                        num /= i;
                    }
                    range_prime_values_[j].emplace_back(i, cnt);
                }
            }
        }
        return range_prime_values_;
    }

     /* 统计单个数中不相同的质数数量 */
    template<typename V>
    int countUniquePrimes(V x){
        assert(!prime_values_.empty());
        int ans = 0;
        for (const auto& prime : prime_values_){
            if (prime > x / prime){
                break;
            }
            ans ++;
            while (x % prime == 0){
                x /= prime;
            }
         }
        return ans + (x > 1);
    }

    /* 统计单个数中所有的质数数量 */
    template<typename V>
    int countAllPrimes(V x){
        assert(!prime_values_.empty());
        int ans = 0;
        for (const auto& prime : prime_values_){
            if (prime > x / prime){
                break;
            }
            while (x % prime == 0){
                x /= prime;
                ans ++;
            }
        }
        return ans + (x > 1);
    }

    /* 获取单个数中的质因子及出现次数 */
    template <typename V>
    std::vector<std::pair<V, int>> getUniquePrimes(V x){
        assert(!prime_values_.empty());
        std::vector<std::pair<V, int>> res;
        for (const auto& prime : prime_values_){
            if (1ll * prime * prime > x){
                break;
            }
            int cnt = 0;
            while (x % prime == 0){
                x /= prime;
                cnt ++;
            }
            if (cnt){
                res.emplace_back(prime, cnt);
            }
        }
        if (x > 1){
            res.emplace_back(x, 1);
        }
        return res;
    }

    /* 判定质数，当筛选范围为1e8时，最多可判断1e16的质数*/
    template<typename V>
    bool isPrime(V x){
        assert(!prime_values_.empty());
        if (x <= getPrimeLimit()){
            return bs_[x];
        }
        for (const auto& prime : prime_values_){
            if (x % prime == 0){
                return false;
            }
        }
        return true;
    }

    /* 统计因子数量 */
    template<typename V>
    int countDivisors(V x){
        assert(!prime_values_.empty());
        int ans = 1;
        for (const auto& prime : prime_values_){
            if (prime > x / prime){
                break;
            }
            int power = 0;
            while (x % prime == 0){
                x /= prime;
                power ++;
            }
            ans *= power + 1;
        }
        return x > 1 ? ans * 2 : ans;
    }


private:
    std::bitset<100000010>                            bs_;
    std::vector<int>                                  prime_values_;
    std::vector<std::vector<std::pair<int, int>>>     range_prime_values_;
    std::vector<int>                                  range_prime_nums_;


private:
    constexpr primeType      getPrimeLimit()      { return T::value; }
    constexpr rangePrimeType getRangePrimeLimit() { return U::value; }
};


constexpr int  N = (int)5e6;
constexpr int  PRIME_VALUE_LIMIT   =   N + 10;              /*最多可以到1e8*/
constexpr int  RANGE_VALUE_LIMIT   =   N + 10;        /*目测最多到5e6*/
using Primer = PrimeNumberManipulator<std::integral_constant<decltype(PRIME_VALUE_LIMIT), PRIME_VALUE_LIMIT>, 
                                     std::integral_constant<decltype(PRIME_VALUE_LIMIT), RANGE_VALUE_LIMIT>>;

Primer primer;



using namespace std;

array<long long, N + 10> pref = {};

void preProcess() {
    auto& res = primer.countRangePrimes();
    for (int i = 1; i <= N; ++i){
        pref[i] = pref[i - 1] + res[i];
    }
}


void solve(){
    int a, b;
    cin >> a >> b;

    cout << pref[a] - pref[b] << '\n';
}


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int tc = 1;

    preProcess();

    if constexpr (true){
        cin >> tc;
    }

    while (tc--) {
        solve();
    }
    return 0;
}
#endif