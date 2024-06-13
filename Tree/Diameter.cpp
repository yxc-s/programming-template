
/*
 * Diameter(树的直径)
 * gitHub(仓库地址): https://github.com/yxc-s/programming-template.git
 */











/*
  深搜
  两次深搜
  可以得到各种信息
  不能处理负权值
*/
void dfsDiameter(const std::vector<std::vector<int>>& al){
    int n = (int)al.size();
    std::vector<int> dist(n + 1);
    std::vector<int> parent(n + 1);
    std::function<void(int, int, int, bool)> dfs = [&](int u, int p, int d, bool should_record){
        dist[u] = d;
        if (should_record){
            parent[`u] = p;
        }
        for (const auto& v : al[u]){
            if (v != p){
                dfs(v, u, d + 1, should_record);
            }
        }
    };

    dfs(1, 0, 0, false);
    int edge_point_a = std::max_element(dist.begin(), dist.end()) - dist.begin();
    dfs(edge_point_a, 0, 0, true);
    int edge_point_b = std::max_element(dist.begin(), dist.end()) - dist.begin();

    std::vector<int> path;
    for (int u = edge_point_b; u; u = parent[u]){
        path.push_back(u);
    }
}



/*
  动态规划
  可以有负数权值
  只能得到直径长度
*/
void dpDiameter(const std::vector<std::vector<pair<int, int>>>& al){
    int n = (int)al.size();
    std::vector<int> dp(n + 1);
    int diameter_length = 0;
    std::function<void(int, int)> dfs = [&](int u, int p){
        for (const auto& [v, w] : al[u]){
            if (v != p){
                dfs(v, p);
                diameter_length = std::max(diameter_length, dp[u] + dp[v] + w);
                dp[u] = std::max(dp[u], dp[v] + w);
            }
        }
    };

    dfs(1, 0);
}