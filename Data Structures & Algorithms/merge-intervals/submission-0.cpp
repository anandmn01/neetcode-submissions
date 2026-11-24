class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        // Sort intervals by starting position
        sort(intervals.begin(), intervals.end());

        vector<vector<int>> result;

        for (auto& interval : intervals) {

            // First interval or no overlap
            if (result.empty() ||
                result.back()[1] < interval[0]) {

                result.push_back(interval);
            }

            // Overlapping intervals
            else {

                result.back()[1] =
                    max(result.back()[1], interval[1]);
            }
        }

        return result;
    }
};
