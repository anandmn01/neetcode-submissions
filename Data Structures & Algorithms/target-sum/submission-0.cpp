class Solution {
public:
    unordered_map<string, int> memo;

    int solve(
        int index,
        int currentSum,
        vector<int>& nums,
        int target
    ) {

        if (index == nums.size()) {
            return currentSum == target;
        }

        string key =
            to_string(index) + "," + to_string(currentSum);

        if (memo.count(key)) {
            return memo[key];
        }

        int add = solve(
            index + 1,
            currentSum + nums[index],
            nums,
            target
        );

        int subtract = solve(
            index + 1,
            currentSum - nums[index],
            nums,
            target
        );

        return memo[key] = add + subtract;
    }

    int findTargetSumWays(vector<int>& nums, int target) {

        memo.clear();

        return solve(0, 0, nums, target);
        
    }
};
