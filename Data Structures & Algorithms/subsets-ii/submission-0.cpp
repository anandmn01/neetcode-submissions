class Solution {
private:

    void backtrack(
            int start,
                    vector<int>& nums,
                            vector<int>& current,
                                    vector<vector<int>>& result
                                        ) {

                                                // Every current state is a valid subset.
                                                        result.push_back(current);

                                                                for (int i = start; i < nums.size(); i++) {

                                                                            // Skip duplicates at the same recursion level.
                                                                                        if (i > start && nums[i] == nums[i - 1]) {
                                                                                                        continue;
                                                                                                                    }

                                                                                                                                // Choose the current element.
                                                                                                                                            current.push_back(nums[i]);

                                                                                                                                                        // Move to the next index because each element is used once.
                                                                                                                                                                    backtrack(i + 1, nums, current, result);

                                                                                                                                                                                // Backtrack: remove the selected element.
                                                                                                                                                                                            current.pop_back();
                                                                                                                                                                                                    }
                                                                                                                                                                                                        }

                                                                                                                                                                                                        public:

                                                                                                                                                                                                            vector<vector<int>> subsetsWithDup(vector<int>& nums) {

                                                                                                                                                                                                                    vector<vector<int>> result;
                                                                                                                                                                                                                            vector<int> current;

                                                                                                                                                                                                                                    // Sort to group duplicate values together.
                                                                                                                                                                                                                                            sort(nums.begin(), nums.end());

                                                                                                                                                                                                                                                    backtrack(0, nums, current, result);

                                                                                                                                                                                                                                                            return result;
    }
};
