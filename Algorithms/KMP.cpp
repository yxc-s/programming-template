
/*
  应该考虑一下应该实现什么功能
*/


class Kmp{
public:
    Kmp(){}


    /*
      获取kmp表。
    */
    static std::vector<int> getKmpTable(const std::string& t){
        int len = 0, j = 1;
        int m = (int)t.size();
        std::vector<int> res;
        res.resize(m);
        while (j < m){
            if (t[j] == t[len]){
                res[j] = len;
                j ++;
                len ++;
            }
            else if (len){
                len = res[len - 1];
            }
            else{
                j ++;
            }
        }
        return res;
    }

    /*
      返回匹配成功的次数。
    */
    static size_t count(const std::string& s, const std::string& t, const std::vector<int>& table){
        size_t m = t.size();
        size_t n = s.size();
        int res = 0;
        for (int i = 0, j = 0; i < n; ++i){
            if (s[i] == t[j]){
                i ++;
                j ++;
                if (j == m){
                    res ++;
                    j = table[j - 1];
                }
            }
            else if (j){
                j = table[j - 1];
            }
            else{
                i ++;
            }
        }
        return res;
    }

    /*
      返回首次匹配成功的位置。
    */
    static size_t find_first_of(const std::string s, const std::string& t, size_t pos, const std::vector<int>& table){
        size_t m = t.size();
        size_t n = s.size();
        for (size_t i = pos, j = 0; i < n; ++i){
            if (s[i] == t[j]){
                i ++;
                j ++;
                if (j == m){
                    return i - j;
                }
            }
            else if (j){
                j = table[j - 1];
            }
            else{
                i ++;
            }
        }
        return s.npos;
    }

    /*
      返回s中每一次与t匹配成功的位置。
    */
    static std::vector<int> getPositionArray(const std::string& s, const std::string& t, const std::vector<int>& table){
        std::vector<int> res;
        size_t n = s.size();
        size_t m = t.size();
        for (int i = 0, j = 0; i < n; ++i){
            if (s[i] == t[j]){
                i ++;
                j ++;
                if (j == m){
                    res.push_back(i - j);
                    j = table[j - 1];
                }
            }
            else if (j){
                j = table[j - 1];
            }
            else{
                i ++;
            }
        }
        return res;
    }


};
