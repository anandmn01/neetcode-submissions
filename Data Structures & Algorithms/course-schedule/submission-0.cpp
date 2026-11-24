class Solution {
public:
    bool dfs(int node, vector<vector<int>>& adj,
             vector<int>& vis) {

        // Node is already in the current DFS path
        // Therefore, a cycle exists
        if (vis[node] == 1) {
            return true;
        }

        // Node has already been completely processed
        if (vis[node] == 2) {
            return false;
        }

        // Mark as currently visiting
        vis[node] = 1;

        // Explore all neighbors
        for (int neighbor : adj[node]) {

            if (dfs(neighbor, adj, vis)) {
                return true;
            }
        }

        // Mark as completely processed
        vis[node] = 2;

        return false;
    }

    bool canFinish(int numCourses,
                   vector<vector<int>>& prerequisites) {

        vector<vector<int>> adj(numCourses);

        // Build directed graph
        for (auto& p : prerequisites) {

            int a = p[0];
            int b = p[1];

            adj[b].push_back(a);
        }

        vector<int> vis(numCourses, 0);

        // Check every disconnected component
        for (int i = 0; i < numCourses; i++) {

            if (vis[i] == 0) {

                if (dfs(i, adj, vis)) {
                    return false;
                }
            }
        }

        return true;
        
    }
};
