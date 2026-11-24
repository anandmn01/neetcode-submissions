class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();

        queue<pair<int, int>> q;

        int fresh = 0;
        int minutes = 0;

        // Step 1: Find all rotten and fresh fruits
        for (int i = 0; i < m; i++) {

            for (int j = 0; j < n; j++) {

                if (grid[i][j] == 2) {
                    q.push({i, j});
                }
                else if (grid[i][j] == 1) {
                    fresh++;
                }
            }
        }

        // Four directions
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        // Step 2: Multi-source BFS
        while (!q.empty() && fresh > 0) {

            int size = q.size();

            // Process one complete minute
            for (int i = 0; i < size; i++) {

                auto [x, y] = q.front();
                q.pop();

                // Explore four neighbors
                for (int k = 0; k < 4; k++) {

                    int nx = x + dx[k];
                    int ny = y + dy[k];

                    // Boundary check
                    if (nx < 0 || ny < 0 ||
                        nx >= m || ny >= n) {
                        continue;
                    }

                    // Only fresh fruits can become rotten
                    if (grid[nx][ny] != 1) {
                        continue;
                    }

                    // Make the fresh fruit rotten
                    grid[nx][ny] = 2;

                    fresh--;

                    q.push({nx, ny});
                }
            }

            // One complete BFS level = one minute
            minutes++;
        }

        // Step 3: Check remaining fresh fruits
        return fresh == 0 ? minutes : -1;
    }
};
