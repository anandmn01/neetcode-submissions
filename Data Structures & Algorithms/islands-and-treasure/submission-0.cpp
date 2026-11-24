class Solution {
public:
    void islandsAndTreasure(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();

        queue<pair<int, int>> q;

        // Step 1: Add all treasure chests to the queue
        for (int i = 0; i < m; i++) {

            for (int j = 0; j < n; j++) {

                if (grid[i][j] == 0) {
                    q.push({i, j});
                }
            }
        }

        // Four directions: up, down, left, right
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        // Step 2: Multi-source BFS
        while (!q.empty()) {

            auto [x, y] = q.front();
            q.pop();

            // Explore four neighboring cells
            for (int k = 0; k < 4; k++) {

                int nx = x + dx[k];
                int ny = y + dy[k];

                // Boundary check
                if (nx < 0 || ny < 0 ||
                    nx >= m || ny >= n) {
                    continue;
                }

                // Skip water and already visited cells
                if (grid[nx][ny] != INT_MAX) {
                    continue;
                }

                // Update shortest distance
                grid[nx][ny] = grid[x][y] + 1;

                // Add updated cell to queue
                q.push({nx, ny});
            }
        }
    }
};
