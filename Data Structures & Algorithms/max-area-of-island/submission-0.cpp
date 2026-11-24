class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
       int m = grid.size();
        int n = grid[0].size();

        int maxArea = 0;

        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        queue<pair<int, int>> q;

        for (int i = 0; i < m; i++) {

            for (int j = 0; j < n; j++) {

                if (grid[i][j] == 1) {

                    int area = 0;

                    q.push({i, j});
                    grid[i][j] = 0;

                    while (!q.empty()) {

                        auto [x, y] = q.front();
                        q.pop();

                        area++;

                        for (int k = 0; k < 4; k++) {

                            int nx = x + dx[k];
                            int ny = y + dy[k];

                            if (nx >= 0 && ny >= 0 &&
                                nx < m && ny < n &&
                                grid[nx][ny] == 1) {

                                grid[nx][ny] = 0;

                                q.push({nx, ny});
                            }
                        }
                    }

                    maxArea = max(maxArea, area);
                }
            }
        }

        return maxArea; 
    }
};
