class Solution {
public:
   vector<int> parent;
    vector<int> size;

    // Find with Path Compression
    int find(int x) {

        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }

        return parent[x];
    }

    // Union by Size
    bool unite(int u, int v) {

        int rootU = find(u);
        int rootV = find(v);

        // Both nodes already belong to the same component
        if (rootU == rootV) {
            return false;
        }

        // Attach smaller component to larger component
        if (size[rootU] < size[rootV]) {
            swap(rootU, rootV);
        }

        parent[rootV] = rootU;

        size[rootU] += size[rootV];

        return true;
    }

    vector<int> findRedundantConnection(
        vector<vector<int>>& edges
    ) {

        int n = edges.size();

        parent.resize(n + 1);
        size.assign(n + 1, 1);

        // Initially, every node is its own parent
        for (int i = 1; i <= n; i++) {
            parent[i] = i;
        }

        // Process every edge
        for (auto& edge : edges) {

            int u = edge[0];
            int v = edge[1];

            // If union fails, this edge creates a cycle
            if (!unite(u, v)) {
                return edge;
            }
        }

        return {};
        
    }
};
