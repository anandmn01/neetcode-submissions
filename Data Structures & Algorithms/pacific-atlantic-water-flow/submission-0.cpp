class Solution {
public:
        int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};

    void dfs(
        int r,
        int c,
        vector<vector<int>>& heights,
        vector<vector<bool>>& visited
    ) {

        visited[r][c] = true;

        int m = heights.size();
        int n = heights[0].size();

        for (int k = 0; k < 4; k++) {

            int nr = r + dx[k];
            int nc = c + dy[k];

            // Boundary check
            if (nr < 0 || nc < 0 ||
                nr >= m || nc >= n) {
                continue;
            }

            // Already visited
            if (visited[nr][nc]) {
                continue;
            }

            // Reverse water-flow condition
            if (heights[nr][nc] >= heights[r][c]) {

                dfs(nr, nc, heights, visited);
            }
        }
    }

    vector<vector<int>> pacificAtlantic(
        vector<vector<int>>& heights
    ) {

        int m = heights.size();
        int n = heights[0].size();

        vector<vector<bool>> pacific(
            m, vector<bool>(n, false)
        );

        vector<vector<bool>> atlantic(
            m, vector<bool>(n, false)
        );

        // Pacific DFS: Top and Left boundaries
        for (int i = 0; i < m; i++) {
            dfs(i, 0, heights, pacific);
        }

        for (int j = 0; j < n; j++) {
            dfs(0, j, heights, pacific);
        }

        // Atlantic DFS: Bottom and Right boundaries
        for (int i = 0; i < m; i++) {
            dfs(i, n - 1, heights, atlantic);
        }

        for (int j = 0; j < n; j++) {
            dfs(m - 1, j, heights, atlantic);
        }

        // Find cells that can reach both oceans
        vector<vector<int>> ans;

        for (int i = 0; i < m; i++) {

            for (int j = 0; j < n; j++) {

                if (pacific[i][j] && atlantic[i][j]) {

                    ans.push_back({i, j});
                }
            }
        }

        return ans;
    }
};
