class Solution {
public:
    vector<int> minInterval(vector<vector<int>>& intervals, vector<int>& queries) {
       // Step 1: Sort intervals by starting point
        sort(intervals.begin(), intervals.end());

        // Step 2: Store queries with their original indices
        vector<pair<int, int>> sortedQueries;

        for (int i = 0; i < queries.size(); i++) {

            sortedQueries.push_back({queries[i], i});
        }

        // Sort queries by value
        sort(sortedQueries.begin(), sortedQueries.end());

        // Step 3: Min Heap stores {length, right}
        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>
        > minHeap;

        vector<int> ans(queries.size(), -1);

        int j = 0;
        int n = intervals.size();

        // Step 4: Process queries in sorted order
        for (auto& query : sortedQueries) {

            int q = query.first;
            int originalIndex = query.second;

            // Add all intervals that have started
            while (j < n && intervals[j][0] <= q) {

                int left = intervals[j][0];
                int right = intervals[j][1];

                int length = right - left + 1;

                minHeap.push({length, right});

                j++;
            }

            // Remove intervals that have expired
            while (!minHeap.empty() &&
                   minHeap.top().second < q) {

                minHeap.pop();
            }

            // Get the shortest valid interval
            if (!minHeap.empty()) {

                ans[originalIndex] = minHeap.top().first;
            }
        }

        return ans; 
    }
};
