class Solution {
public:
    bool dfs(int node, int parent,
             vector<vector<int>>& adj,
             vector<bool>& vis) {

        vis[node] = true;

        for (int neighbor : adj[node]) {

            // Ignore the edge back to parent
            if (neighbor == parent) {
                continue;
            }

            // Visited neighbor other than parent = cycle
            if (vis[neighbor]) {
                return false;
            }

            if (!dfs(neighbor, node, adj, vis)) {
                return false;
            }
        }

        return true;
    }

    bool validTree(int n, vector<vector<int>>& edges) {

        // A tree must have exactly n - 1 edges
        if (edges.size() != n - 1) {
            return false;
        }

        vector<vector<int>> adj(n);

        // Build undirected graph
        for (auto& edge : edges) {

            int u = edge[0];
            int v = edge[1];

            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        vector<bool> vis(n, false);

        // Check for cycles from node 0
        if (!dfs(0, -1, adj, vis)) {
            return false;
        }

        // Check connectivity
        for (int i = 0; i < n; i++) {

            if (!vis[i]) {
                return false;
            }
        }

        return true;

    }
};
