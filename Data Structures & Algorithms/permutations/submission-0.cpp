class Solution {
private:

    void backtrack(
            vector<int>& nums,
                    vector<bool>& visited,
                            vector<int>& current,
                                    vector<vector<int>>& result
                                        ) {

                                                // A complete permutation is formed.
                                                        if (current.size() == nums.size()) {
                                                                    result.push_back(current);
                                                                                return;
                                                                                        }

                                                                                                // Try every available element.
                                                                                                        for (int i = 0; i < nums.size(); i++) {

                                                                                                                    // Skip elements that are already used.
                                                                                                                                if (visited[i]) {
                                                                                                                                                continue;
                                                                                                                                                            }

                                                                                                                                                                        // Choose the current element.
                                                                                                                                                                                    current.push_back(nums[i]);
                                                                                                                                                                                                visited[i] = true;

                                                                                                                                                                                                            // Explore the next position.
                                                                                                                                                                                                                        backtrack(nums, visited, current, result);

                                                                                                                                                                                                                                    // Backtrack: undo the choice.
                                                                                                                                                                                                                                                current.pop_back();
                                                                                                                                                                                                                                                            visited[i] = false;
                                                                                                                                                                                                                                                                    }
                                                                                                                                                                                                                                                                        }

                                                                                                                                                                                                                                                                        public:

                                                                                                                                                                                                                                                                            vector<vector<int>> permute(vector<int>& nums) {

                                                                                                                                                                                                                                                                                    vector<vector<int>> result;
                                                                                                                                                                                                                                                                                            vector<int> current;

                                                                                                                                                                                                                                                                                                    vector<bool> visited(nums.size(), false);

                                                                                                                                                                                                                                                                                                            backtrack(nums, visited, current, result);

                                                                                                                                                                                                                                                                                                                    return result;
    }
};
