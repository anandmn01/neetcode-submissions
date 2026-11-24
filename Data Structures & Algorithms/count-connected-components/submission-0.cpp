class Solution {
public:
    void dfs(int node,
             vector<vector<int>>& adj,
             vector<bool>& vis) {

        vis[node] = true;

        for (int neighbor : adj[node]) {

            if (!vis[neighbor]) {
                dfs(neighbor, adj, vis);
            }
        }
    }

    int countComponents(int n, vector<vector<int>>& edges) {

        // Build adjacency list
        vector<vector<int>> adj(n);

        for (auto& edge : edges) {

            int u = edge[0];
            int v = edge[1];

            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        vector<bool> vis(n, false);

        int count = 0;

        // Visit every node
        for (int i = 0; i < n; i++) {

            if (!vis[i]) {

                // Found a new component
                count++;

                dfs(i, adj, vis);
            }
        }

        return count;

    }
};
