
/* DoublingSparseTable */

/* 倍增dp，示例是静态区间最大值查询，可以拓展到最小值，最大公约数等 
  TODO:是否封装（感觉好像没必要，代码量不大，复现很快）？
*/

void solve(){
    /* n number and q querys */
    int n, q;
    cin >> n >> q;

    /* Input n numbers */
    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i){
        cin >> a[i];
    }

    /* Get max index */
    int p;
    for (p = 1; (1 << p) <= n; ++p);
    p --;

    /* Initialize segments of length 1 */
    vector<vector<int>> dp (n + 1, vector<int> (p + 1));
    for (int i = 1; i <= n; ++i){
        dp[i][0] = a[i];
    }

    /* Max function examples: dp[i][j] indicates the maximum value of[i, i + (1 << j) - 1] */
    auto merge = [](const auto& lseg, const auto& rseg) {
        return max(lseg, rseg);
    };

    /* Extend to larger segments */
    for (int k = 1; k <= p; ++k){
        for (int i = 1; i + (1 << k) - 1 <= n; ++i){
            /* Merge left and right segments by function merge */
            dp[i][k] = merge(dp[i][k - 1], dp[i + (1 << (k - 1))][k - 1]);
        }
    }

    while (q --){
        int l, r;
        cin >> l >> r;
        int len = (r - l) + 1;
        for (p = 1; (1 << p) <= len; ++p);
        p --;
        auto res = merge(dp[l][p], dp[r - (1 << p) + 1][p]);
        //deal result..
        cout << res << '\n';
    }
}