#include <cstring>

class Solution {
public:


    int dp[101][101];

    bool solve(string& s, int index, int balance) {

        if (balance < 0) {
            return false;
        }

        if (index == s.size()) {
            return balance == 0;
        }

        if (dp[index][balance] != -1) {
            return dp[index][balance];
        }

        bool result = false;

        if (s[index] == '(') {

            result = solve(s, index + 1, balance + 1);

        }
        else if (s[index] == ')') {

            result = solve(s, index + 1, balance - 1);

        }
        else {

            result =
                solve(s, index + 1, balance + 1) ||
                solve(s, index + 1, balance - 1) ||
                solve(s, index + 1, balance);
        }

        return dp[index][balance] = result;
    }

    bool checkValidString(string s) {

        memset(dp, -1, sizeof(dp));

        return solve(s, 0, 0);
        
    }
};
