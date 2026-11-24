class Solution {
public:
    int eraseOverlapIntervals(vector<vector<int>>& intervals) {
        sort(intervals.begin(), intervals.end());

        int removals = 0;
        int prevEnd = intervals[0][1];

        for (int i = 1; i < intervals.size(); i++) {

            // Overlapping intervals
            if (intervals[i][0] < prevEnd) {

                removals++;

                // Keep the interval that ends earlier
                prevEnd = min(prevEnd, intervals[i][1]);
            }

            // Non-overlapping interval
            else {
                prevEnd = intervals[i][1];
            }
        }

        return removals;
    }
};
