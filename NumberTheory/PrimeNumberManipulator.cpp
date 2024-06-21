/*
 * PrimeNumberManipulator(质数类)
 * 设计思想：基于面向对象的思想的质数管理类。
 * 基于面向对象的编程思想，本方法尽可能多的隐藏了内部实现的细节，并且将必要的编程接口暴露在外部，并需要对这些接口进行直接的修改。
 *                             注意事项：第一个模板参数的数值必须是int型（在竞赛中范围最大是1e8)
 *                                      第二个模板参数是代表是否对第一个参数的范围内每个数进行质数统计（范围大概是1e6不会TLE）。
 *
 * gitHub(仓库地址): https://github.com/yxc-s/programming-template.git
 */












template<typename T, const bool CALCULATE_RANGE_PRIMES = false>
class PrimeNumberManipulator{
public:
    template<typename U = T>
    using EnableIfInt = typename std::enable_if<std::is_same<U, int>::value, U>::type;
    using Type        = typename std::decay<decltype(T::value)>::type;


    PrimeNumberManipulator(EnableIfInt<Type>* = 0){
        sievePrimes();
        if (CALCULATE_RANGE_PRIMES){
            getRangePrimes();
        }
    }

    const std::vector<int>& getPrimeArray() const {
        return prime_values_;
    }

    const std::vector<std::vector<std::pair<int, int>>>& getRangePrimesArray() const {
        return range_prime_values_;
    }

    /* 获取单个数的质因子及出现次数 */
    template <typename U>
    std::vector<std::pair<U, int>> getUniquePrimes(U x){
        std::vector<std::pair<U, int>> res;
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

    /* 统计不同的质数个数 */
    template<typename U>
    int countUniquePrimes(U x){
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

    /* 统计所有的质数 */
    template<typename U>
    int countAllPrimes(U x){
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

    /* 判定质数，当筛选范围为1e8时，最多可判断1e16的质数*/
    template<typename U>
    bool isPrime(U x){
        if (x <= getLimit()){
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
    template<typename U>
    int countDivisors(U x){
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


private:
    constexpr Type getLimit() { return T::value; }
    
    /* 线性筛 */
    void sievePrimes(){
        bs_.set();
        bs_[0] = bs_[1] = 0;
        for (int i = 2; i <= getLimit(); ++i){
            if (bs_[i]){
                prime_values_.emplace_back(i);
            }
            for (const auto& prime : prime_values_){
                if (1ll * i * prime > getLimit()){
                    break;
                }
                bs_[i * prime] = 0;
                if (i % prime == 0){
                    break;
                }
            }
        }
    }

    /* 获取[1, x]范围内每个数的质因子及其出现次数*/
    void getRangePrimes(){
        range_prime_values_.resize(getLimit() + 1);
        for (int i = 2; i <= getLimit(); ++i){
            if (range_prime_values_[i].empty()){
                for (int j = i; j <= getLimit(); j += i){
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
    }


};



constexpr int  PRIME_VALUE_LIMIT      =   (int)1e6;    //最多可以到1e8
constexpr bool CALCULATE_RANGE_PRIMES =   false;        //筛范围内质数1e7大概会TLE
using Primer = PrimeNumberManipulator<std::integral_constant<decltype(PRIME_VALUE_LIMIT), PRIME_VALUE_LIMIT>, 
                                    CALCULATE_RANGE_PRIMES>;

Primer primer;
const std::vector<int>&                              primes = primer.getPrimeArray();
const std::vector<std::vector<std::pair<int, int>>>& range_primes = primer.getRangePrimesArray();

/*
TODO: 第二个模板参数要不要也换成跟第一个模板参数相同的类型，来单独进行筛选？
      将大质数的算法也写进去。
       欧拉函数，莫比乌斯函数。
*/