class Solution {
public:
    int solve(int l, int r, vector<int>& nums,
              vector<vector<int>>& dp) {

        // No balloon exists between l and r
        if (l + 1 == r) {
            return 0;
        }

        if (dp[l][r] != -1) {
            return dp[l][r];
        }

        int maxi = 0;

        // Choose the last balloon to burst
        for (int k = l + 1; k < r; k++) {

            int coins = nums[l] * nums[k] * nums[r];

            coins += solve(l, k, nums, dp);
            coins += solve(k, r, nums, dp);

            maxi = max(maxi, coins);
        }

        return dp[l][r] = maxi;
    }

    int maxCoins(vector<int>& nums) {

        int n = nums.size();

        // Add boundary balloons
        nums.insert(nums.begin(), 1);
        nums.push_back(1);

        vector<vector<int>> dp(
            n + 2, vector<int>(n + 2, -1)
        );

        return solve(0, n + 1, nums, dp);
    }
};