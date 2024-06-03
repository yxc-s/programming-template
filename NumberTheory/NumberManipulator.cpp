class NumberManipulator{
public:
    explicit NumberManipulator(unsigned int x, bool sieve = false):
        sz_(x),
        has_sieved_(sieve){
            if (has_sieved_ == true){
                sievePrimes();
            }
        }


    const std::vector<int>& getPrimeArray(){
        if (has_sieved_ == false){
            sievePrimes();
        }
        return prime_values_;
    }

    template <typename T>
    std::vector<std::pair<T, int>> getUniquePrimes(T x){
        std::vector<std::pair<T, int>> res;
        if (has_sieved_){
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
        }
        else{
            int cnt = 0;
            while (x % 2 == 0){
                x /= 2;
                cnt ++;
            }
            if (cnt){
                res.emplace_back(2, cnt);
            }
            for (int i = 3; i <= x / i; i += 2){
                if (x % i == 0){
                    cnt = 0;
                    while (x % i == 0){
                        cnt ++;
                        x /= i;
                    }
                    res.emplace_back(i, cnt);
                }
            }
        }
        if (x > 1){
            res.emplace_back(x, 1);
        }
        return res;
    }

    std::vector<std::vector<std::pair<int, int>>> getRangePrimes(int x){
        std::vector<std::vector<std::pair<int, int>>> res(x + 1);
        for (int i = 2; i <= x; ++i){
            if (res[i].empty()){
                for (int j = i; j <= x; j += i){
                    int cnt = 0;
                    int num = j;
                    while (num % i == 0){
                        cnt ++;
                        num /= i;
                    }
                    res[j].emplace_back(i, cnt);
                }
            }
        }
        return res;
    }

    template<typename T>
    int countUniquePrimes(T x){
        int ans = 0;
        if (has_sieved_){
            for (const auto& prime : prime_values_){
                if (prime > x / prime){
                    break;
                }
                ans ++;
                while (x % prime == 0){
                    x /= prime;
                }
            }
        }
        else{
            if (x % 2 == 0){
                ans ++;
            }
            while (x % 2 == 0){
                x /= 2;
            }
            for (int i = 3; i <= x / i; i += 2){
                if (x % i == 0){
                    ans ++;
                }
                while (x % i == 0){
                    x /= i;
                }
            }
        }
        return ans + (x > 1);
    }

    template<typename T>
    int countAllPrimes(T x){
        int ans = 0;
        if (has_sieved_){
            for (const auto& prime : prime_values_){
                if (prime > x / prime){
                    break;
                }
                while (x % prime == 0){
                    x /= prime;
                    ans ++;
                }
            }
        }
        else{
            while (x % 2 == 0){
                x /= 2;
                ans ++;
            }
            for (int i = 3; i <= x / i; i += 2){
                while (x % i == 0){
                    x /= i;
                    ans ++;
                }
            }
        }
        return ans + (x > 1);
    }


    template<typename T>
    bool isPrime(T x){
        assert(x < sz_);
        if (has_sieved_) {
            if (x <= sz_){
                return bs_[x];
            }
            else{
                for (const auto& prime : prime_values_){
                    if (x % prime == 0){
                        return false;
                    }
                }
                return true;
            }
        }
        else{
            if (x == 2){
                return true;
            }
            if (x < 2 || x % 2 == 0){
                return false;
            }
            for (T i = 3; i <= x / i; i += 2){
                if (x % i == 0){
                    return false;
                }
            }
            return true;
        }
    }

    template<typename T>
    int countDivisors(T x){
        if (has_sieved_){
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
        else{
            int ans = 0;
            for (int i = 1; i <= x / i; ++i){
                if (x % i == 0){
                    ans ++;
                    ans += (x / i != i);
                }
            }
            return ans;
        }
    }

    template<typename T>
    std::vector<T> getDivisors(T x){
        std::vector<T> res;
        for (int i = 1; i <= x / i; ++i){
            if (x % i == 0){
                res.emplace_back(i);
                if (x / i != i){
                    res.emplace_back(x / i);
                }
            }
        }
        std::sort(res.begin(), res.end());
        return res;
    }



private:
    int     sz_;
    bool    has_sieved_;

    std::bitset<10000010>   bs_;
    std::vector<int>        prime_values_;


private:
    void sievePrimes(){
        has_sieved_ = true;
        bs_.set();
        bs_[0] = bs_[1] = 0;
        for (int i = 2; i <= sz_; ++i){
            if (bs_[i]){
                prime_values_.emplace_back(i);
            }
            for (const auto& prime : prime_values_){
                if (1ll * i * prime > sz_){
                    break;
                }
                bs_[i * prime] = 0;
                if (i % prime == 0){
                    break;
                }
            }
        }
    }

};
NumberManipulator num_manip{(int)1e5, true};
