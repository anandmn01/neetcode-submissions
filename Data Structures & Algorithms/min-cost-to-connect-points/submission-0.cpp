class Solution {
public:
    int minCostConnectPoints(vector<vector<int>>& points) {
         int n = points.size();

        vector<int> minDist(n, INT_MAX);

        vector<bool> visited(n, false);

        minDist[0] = 0;

        int totalCost = 0;

        // Include all points in the MST
        for (int count = 0; count < n; count++) {

            int minCost = INT_MAX;
            int u = -1;

            // Find the minimum-cost unvisited point
            for (int i = 0; i < n; i++) {

                if (!visited[i] && minDist[i] < minCost) {

                    minCost = minDist[i];

                    u = i;
                }
            }

            // Include this point in MST
            visited[u] = true;

            totalCost += minCost;

            // Update minimum connection costs
            for (int v = 0; v < n; v++) {

                if (!visited[v]) {

                    int distance =
                        abs(points[u][0] - points[v][0]) +
                        abs(points[u][1] - points[v][1]);

                    minDist[v] = min(minDist[v], distance);
                }
            }
        }

        return totalCost;
    }
};
