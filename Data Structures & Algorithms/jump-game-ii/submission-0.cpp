class Solution {
public:
    vector<int> dp;

    int solve(vector<int>& nums, int index) {

        int n = nums.size();

        // Destination reached
        if (index >= n - 1) {
            return 0;
        }

        // Already calculated
        if (dp[index] != -1) {
            return dp[index];
        }

        int minJumps = INT_MAX;

        for (int jump = 1; jump <= nums[index]; jump++) {

            int nextIndex = index + jump;

            if (nextIndex < n) {

                int result = solve(nums, nextIndex);

                if (result != INT_MAX) {
                    minJumps = min(minJumps, 1 + result);
                }
            }
        }

        return dp[index] = minJumps;
    }

    int jump(vector<int>& nums) {

        dp.assign(nums.size(), -1);

        return solve(nums, 0);
        
    }
};
