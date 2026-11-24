class Solution {
public:

    vector<int> dp;

    bool solve(vector<int>& nums, int index) {

        if (index >= nums.size() - 1) {
            return true;
        }

        if (dp[index] != -1) {
            return dp[index];
        }

        int maxJump = nums[index];

        for (int jump = 1; jump <= maxJump; jump++) {

            if (solve(nums, index + jump)) {
                return dp[index] = 1;
            }
        }

        return dp[index] = 0;
    }

    bool canJump(vector<int>& nums) {

        dp.assign(nums.size(), -1);

        return solve(nums, 0);
        
    }
};
