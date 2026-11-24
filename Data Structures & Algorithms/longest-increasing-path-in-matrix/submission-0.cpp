class Solution {
public:
    int m, n;

    vector<vector<int>> dp;

    int dfs(
        int row,
        int col,
        vector<vector<int>>& matrix
    ) {

        // Return the previously calculated result
        if (dp[row][col] != 0) {
            return dp[row][col];
        }

        // At least the current cell is included
        dp[row][col] = 1;

        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        for (int d = 0; d < 4; d++) {

            int nr = row + dr[d];
            int nc = col + dc[d];

            // Check boundaries and increasing condition
            if (
                nr >= 0 && nr < m &&
                nc >= 0 && nc < n &&
                matrix[nr][nc] > matrix[row][col]
            ) {

                dp[row][col] = max(
                    dp[row][col],
                    1 + dfs(nr, nc, matrix)
                );
            }
        }

        return dp[row][col];
    }

    int longestIncreasingPath(vector<vector<int>>& matrix) {

        m = matrix.size();
        n = matrix[0].size();

        dp.assign(m, vector<int>(n, 0));

        int answer = 0;

        // Start DFS from every cell
        for (int i = 0; i < m; i++) {

            for (int j = 0; j < n; j++) {

                answer = max(
                    answer,
                    dfs(i, j, matrix)
                );
            }
        }

        return answer;
        
    }
};
