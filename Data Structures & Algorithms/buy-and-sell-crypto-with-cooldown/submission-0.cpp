class Solution {
public:
    int solve(
        int i,
        int holding,
        vector<int>& prices,
        vector<vector<int>>& dp
    ) {

        int n = prices.size();

        if (i >= n) {
            return holding ? -1000000000 : 0;
        }

        if (dp[i][holding] != -1) {
            return dp[i][holding];
        }

        if (holding == 0) {

            int skip = solve(
                i + 1, 0, prices, dp
            );

            int buy = -prices[i] + solve(
                i + 1, 1, prices, dp
            );

            return dp[i][holding] = max(skip, buy);
        }

        int hold = solve(
            i + 1, 1, prices, dp
        );

        int sell = prices[i] + solve(
            i + 2, 0, prices, dp
        );

        return dp[i][holding] = max(hold, sell);
    }

    int maxProfit(vector<int>& prices) {

        int n = prices.size();

        vector<vector<int>> dp(
            n,
            vector<int>(2, -1)
        );

        return solve(0, 0, prices, dp);
        
    }
};
