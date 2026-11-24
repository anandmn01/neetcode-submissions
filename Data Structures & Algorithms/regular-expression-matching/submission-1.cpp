class Solution {
public:
    int solve(int i, int j, string& s, string& p,
              vector<vector<int>>& dp) {

        if (j == p.size()) {
            return i == s.size();
        }

        if (dp[i][j] != -1) {
            return dp[i][j];
        }

        bool match = (i < s.size() &&
                     (s[i] == p[j] || p[j] == '.'));

        bool ans = false;

        if (j + 1 < p.size() && p[j + 1] == '*') {

            // Zero occurrences
            bool skip = solve(i, j + 2, s, p, dp);

            // One or more occurrences
            bool take = match &&
                        solve(i + 1, j, s, p, dp);

            ans = skip || take;
        }
        else {

            ans = match &&
                  solve(i + 1, j + 1, s, p, dp);
        }

        return dp[i][j] = ans;
    }

    bool isMatch(string s, string p) {

        int m = s.size();
        int n = p.size();

        vector<vector<int>> dp(
            m + 1, vector<int>(n + 1, -1)
        );

        return solve(0, 0, s, p, dp);
    }
};