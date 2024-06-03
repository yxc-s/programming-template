
/*
 * LCA(最近公共祖先)。
 * gitHub(仓库地址): https://github.com/100000000000000000000000000000000/programming-template.git
 */












void lca(){
    int n, m, root;
    std::cin >> n >> m >> root;

    std::vector<std::vector<int>> al(n + 1);
    for (int i = 1; i < n; ++i){
        int u, v;
        cin >> u >> v;
        al[u].push_back(v);
        al[v].push_back(u);
    }

    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(30));
    std::vector<int> depth(n + 1);
    function<void(int, int, int)> dfs = [&](int u, int p, int d){
        dp[u][0] = p;
        depth[u] = d;
        for (int i = 1; i <= 25; ++i){
            dp[u][i] = dp[dp[u][i - 1]][i - 1];
        }
        for (const auto& v : al[u]){
            if (v != p){
                dfs(v, u, d + 1);
            }
        }
    };

    dfs(root, 0, 0);

    while (m --){
        int u, v;
        cin >> u >> v;
        if (depth[u] < depth[v]){
            swap(u, v);
        }
        for (int i = 25; i >= 0; --i){
            if (depth[u] - (1 << i) >= depth[v]){
                u = dp[u][i];
            }
        }
        if (u == v){
            cout << u << '\n';
            continue;
        }
        for (int i = 25; i >= 0; --i){
            if (dp[u][i] != dp[v][i]){
                u = dp[u][i];
                v = dp[v][i];
            }
        }

        cout << dp[u][0] << '\n';
    }
}