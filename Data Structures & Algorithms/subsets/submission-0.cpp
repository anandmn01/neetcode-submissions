class Solution {
void backtrack(
        int index,
        vector<int>& nums,
        vector<int>& current,
        vector<vector<int>>& result
    ) {

        // All elements have been considered.
        if (index == nums.size()) {
            result.push_back(current);
            return;
        }

        // Choice 1: Include the current element.
        current.push_back(nums[index]);

        backtrack(index + 1, nums, current, result);

        // Backtrack: Remove the last element.
        current.pop_back();

        // Choice 2: Skip the current element.
        backtrack(index + 1, nums, current, result);
    }

public:

    vector<vector<int>> subsets(vector<int>& nums) {

        vector<vector<int>> result;
        vector<int> current;

        backtrack(0, nums, current, result);

        return result;
        
    }
};
