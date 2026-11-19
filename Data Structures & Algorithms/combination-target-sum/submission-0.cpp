class Solution {
private:

    void backtrack(
        int index,
        vector<int>& nums,
        int target,
        vector<int>& current,
        vector<vector<int>>& result
    ) {

        // A valid combination is found.
        if (target == 0) {
            result.push_back(current);
            return;
        }

        // No candidates left.
        if (index == nums.size()) {
            return;
        }

        // Choice 1: Include the current number.
        if (nums[index] <= target) {

            current.push_back(nums[index]);

            // Stay at the same index because reuse is allowed.
            backtrack(
                index,
                nums,
                target - nums[index],
                current,
                result
            );

            // Undo the choice.
            current.pop_back();
        }

        // Choice 2: Skip the current number.
        backtrack(
            index + 1,
            nums,
            target,
            current,
            result
        );
    }

public:

    vector<vector<int>> combinationSum(
        vector<int>& nums,
        int target
    ) {

        vector<vector<int>> result;
        vector<int> current;

        backtrack(0, nums, target, current, result);

        return result;
    }
};
