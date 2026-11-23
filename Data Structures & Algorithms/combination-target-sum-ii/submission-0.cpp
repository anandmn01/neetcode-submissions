class Solution {
private:

    void backtrack(
            int start,
                    vector<int>& candidates,
                            int target,
                                    vector<int>& current,
                                            vector<vector<int>>& result
                                                ) {

                                                        // Found a valid combination.
                                                                if (target == 0) {
                                                                            result.push_back(current);
                                                                                        return;
                                                                                                }

                                                                                                        for (int i = start; i < candidates.size(); i++) {

                                                                                                                    // Skip duplicates at the same recursion level.
                                                                                                                                if (i > start && candidates[i] == candidates[i - 1]) {
                                                                                                                                                continue;
                                                                                                                                                            }

                                                                                                                                                                        // Candidates are sorted, so no later value can work.
                                                                                                                                                                                    if (candidates[i] > target) {
                                                                                                                                                                                                    break;
                                                                                                                                                                                                                }

                                                                                                                                                                                                                            // Choose the current candidate.
                                                                                                                                                                                                                                        current.push_back(candidates[i]);

                                                                                                                                                                                                                                                    // Move to i + 1 because each element can be used once.
                                                                                                                                                                                                                                                                backtrack(
                                                                                                                                                                                                                                                                                i + 1,
                                                                                                                                                                                                                                                                                                candidates,
                                                                                                                                                                                                                                                                                                                target - candidates[i],
                                                                                                                                                                                                                                                                                                                                current,
                                                                                                                                                                                                                                                                                                                                                result
                                                                                                                                                                                                                                                                                                                                                            );

                                                                                                                                                                                                                                                                                                                                                                        // Undo the choice.
                                                                                                                                                                                                                                                                                                                                                                                    current.pop_back();
                                                                                                                                                                                                                                                                                                                                                                                            }
                                                                                                                                                                                                                                                                                                                                                                                                }

                                                                                                                                                                                                                                                                                                                                                                                                public:

                                                                                                                                                                                                                                                                                                                                                                                                    vector<vector<int>> combinationSum2(
                                                                                                                                                                                                                                                                                                                                                                                                            vector<int>& candidates,
                                                                                                                                                                                                                                                                                                                                                                                                                    int target
                                                                                                                                                                                                                                                                                                                                                                                                                        ) {

                                                                                                                                                                                                                                                                                                                                                                                                                                vector<vector<int>> result;
                                                                                                                                                                                                                                                                                                                                                                                                                                        vector<int> current;

                                                                                                                                                                                                                                                                                                                                                                                                                                                // Sort to group duplicate values together.
                                                                                                                                                                                                                                                                                                                                                                                                                                                        sort(candidates.begin(), candidates.end());

                                                                                                                                                                                                                                                                                                                                                                                                                                                                backtrack(0, candidates, target, current, result);

                                                                                                                                                                                                                                                                                                                                                                                                                                                                        return result;
        
    }
};
