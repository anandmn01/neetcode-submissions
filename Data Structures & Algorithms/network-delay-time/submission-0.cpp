class Solution {
public:
    int networkDelayTime(vector<vector<int>>& times,
                         int n,
                         int k) {

        // Step 1: Build adjacency list
        vector<vector<pair<int, int>>> adj(n + 1);

        for (auto &edge : times) {

            int u = edge[0];
            int v = edge[1];
            int wt = edge[2];

            adj[u].push_back({v, wt});
        }

        // Step 2: Initialize distances
        vector<int> dist(n + 1, INT_MAX);

        dist[k] = 0;

        // Step 3: Min Heap
        // {distance, node}
        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>
        > pq;

        pq.push({0, k});

        // Step 4: Dijkstra's Algorithm
        while (!pq.empty()) {

            auto [time, node] = pq.top();
            pq.pop();

            // Ignore outdated entries
            if (time > dist[node]) {
                continue;
            }

            // Explore adjacent nodes
            for (auto &[neighbor, wt] : adj[node]) {

                int newTime = time + wt;

                // Relaxation
                if (newTime < dist[neighbor]) {

                    dist[neighbor] = newTime;

                    pq.push({newTime, neighbor});
                }
            }
        }

        // Step 5: Find maximum shortest distance
        int ans = 0;

        for (int i = 1; i <= n; i++) {

            if (dist[i] == INT_MAX) {
                return -1;
            }

            ans = max(ans, dist[i]);
        }

        return ans;
        
    }
};
