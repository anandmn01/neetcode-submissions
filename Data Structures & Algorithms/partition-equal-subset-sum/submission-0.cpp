class Solution {
public:
    bool solve(int index, int target,
               vector<int>& nums,
               vector<vector<int>>& dp) {

        if (target == 0) {
            return true;
        }

        if (index == nums.size() || target < 0) {
            return false;
        }

        if (dp[index][target] != -1) {
            return dp[index][target];
        }

        // Include current element
        bool take = false;

        if (nums[index] <= target) {

            take = solve(
                index + 1,
                target - nums[index],
                nums,
                dp
            );
        }

        // Exclude current element
        bool notTake = solve(
            index + 1,
            target,
            nums,
            dp
        );

        return dp[index][target] = take || notTake;
    }

    bool canPartition(vector<int>& nums) {

        int total = accumulate(
            nums.begin(),
            nums.end(),
            0
        );

        if (total % 2 != 0) {
            return false;
        }

        int target = total / 2;
        int n = nums.size();

        vector<vector<int>> dp(
            n,
            vector<int>(target + 1, -1)
        );

        return solve(0, target, nums, dp);
        
    }
};
