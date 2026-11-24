class Solution {
public:
    int solve(int i, int j, string& s, string& t,
              vector<vector<int>>& dp) {
        if (j == t.size()) return 1;
        if (i == s.size()) return 0;

        if (dp[i][j] != -1) return dp[i][j];

        int ways = 0;

        if (s[i] == t[j]) {
            ways += solve(i + 1, j + 1, s, t, dp);
        }

        ways += solve(i + 1, j, s, t, dp);
        return dp[i][j] = ways;
    }

    int numDistinct(string s, string t) {
        int m = s.size(), n = t.size();
        vector<vector<int>> dp(m, vector<int>(n, -1));

        return solve(0, 0, s, t, dp);
    }
};