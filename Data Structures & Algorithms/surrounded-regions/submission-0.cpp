class Solution {
public:
    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};

    void dfs(int r, int c, vector<vector<char>>& board) {

        int m = board.size();
        int n = board[0].size();

        // Mark this boundary-connected cell as safe
        board[r][c] = '#';

        // Explore four directions
        for (int k = 0; k < 4; k++) {

            int nr = r + dx[k];
            int nc = c + dy[k];

            // Boundary check
            if (nr < 0 || nc < 0 ||
                nr >= m || nc >= n) {
                continue;
            }

            // Visit only unmarked O cells
            if (board[nr][nc] == 'O') {
                dfs(nr, nc, board);
            }
        }
    }

    void solve(vector<vector<char>>& board) {

        int m = board.size();
        int n = board[0].size();

        // Step 1: DFS from first and last columns
        for (int i = 0; i < m; i++) {

            if (board[i][0] == 'O') {
                dfs(i, 0, board);
            }

            if (board[i][n - 1] == 'O') {
                dfs(i, n - 1, board);
            }
        }

        // Step 2: DFS from first and last rows
        for (int j = 0; j < n; j++) {

            if (board[0][j] == 'O') {
                dfs(0, j, board);
            }

            if (board[m - 1][j] == 'O') {
                dfs(m - 1, j, board);
            }
        }

        // Step 3: Capture surrounded regions
        for (int i = 0; i < m; i++) {

            for (int j = 0; j < n; j++) {

                if (board[i][j] == 'O') {
                    board[i][j] = 'X';
                }

                else if (board[i][j] == '#') {
                    board[i][j] = 'O';
                }
            }
        }
        
    }
};
