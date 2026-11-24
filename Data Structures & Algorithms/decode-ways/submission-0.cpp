class Solution {
public:
    int solve(string& s, int i, vector<int>& dp) {

        int n = s.length();

        if (i == n) {
            return 1;
        }

        if (s[i] == '0') {
            return 0;
        }

        if (dp[i] != -1) {
            return dp[i];
        }

        // Choice 1: Decode one digit
        int ways = solve(s, i + 1, dp);

        // Choice 2: Decode two digits
        if (i + 1 < n) {

            int num = (s[i] - '0') * 10 + (s[i + 1] - '0');

            if (num >= 10 && num <= 26) {
                ways += solve(s, i + 2, dp);
            }
        }

        return dp[i] = ways;
    }

    int numDecodings(string s) {

        int n = s.length();

        vector<int> dp(n, -1);

        return solve(s, 0, dp);
        
    }
};
