#include<cstring>
class Solution {
public:
    int dp[101][101];

    bool solve(
        int i,
        int j,
        string& s1,
        string& s2,
        string& s3
    ) {

        if (i == s1.size() && j == s2.size()) {
            return true;
        }

        if (dp[i][j] != -1) {
            return dp[i][j];
        }

        int k = i + j;

        bool takeS1 = false;
        bool takeS2 = false;

        if (i < s1.size() && s1[i] == s3[k]) {
            takeS1 = solve(i + 1, j, s1, s2, s3);
        }

        if (j < s2.size() && s2[j] == s3[k]) {
            takeS2 = solve(i, j + 1, s1, s2, s3);
        }

        return dp[i][j] = takeS1 || takeS2;
    }

    bool isInterleave(string s1, string s2, string s3) {

        if (s1.size() + s2.size() != s3.size()) {
            return false;
        }

        memset(dp, -1, sizeof(dp));

        return solve(0, 0, s1, s2, s3);
        
    }
};
