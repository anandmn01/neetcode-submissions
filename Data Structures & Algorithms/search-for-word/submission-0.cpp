class Solution {
private:

    bool dfs(
            int r,
                    int c,
                            int index,
                                    vector<vector<char>>& board,
                                            string& word
                                                ) {

                                                        // Entire word has been found.
                                                                if (index == word.size()) {
                                                                            return true;
                                                                                    }

                                                                                            // Check boundaries.
                                                                                                    if (
                                                                                                                r < 0 || c < 0 ||
                                                                                                                            r >= board.size() ||
                                                                                                                                        c >= board[0].size()
                                                                                                                                                ) {
                                                                                                                                                            return false;
                                                                                                                                                                    }

                                                                                                                                                                            // Current character does not match.
                                                                                                                                                                                    if (board[r][c] != word[index]) {
                                                                                                                                                                                                return false;
                                                                                                                                                                                                        }

                                                                                                                                                                                                                // Temporarily mark the cell as visited.
                                                                                                                                                                                                                        char temp = board[r][c];
                                                                                                                                                                                                                                board[r][c] = '#';

                                                                                                                                                                                                                                        // Explore four directions.
                                                                                                                                                                                                                                                bool found =
                                                                                                                                                                                                                                                            dfs(r + 1, c, index + 1, board, word) ||
                                                                                                                                                                                                                                                                        dfs(r - 1, c, index + 1, board, word) ||
                                                                                                                                                                                                                                                                                    dfs(r, c + 1, index + 1, board, word) ||
                                                                                                                                                                                                                                                                                                dfs(r, c - 1, index + 1, board, word);

                                                                                                                                                                                                                                                                                                        // Restore the original character.
                                                                                                                                                                                                                                                                                                                board[r][c] = temp;

                                                                                                                                                                                                                                                                                                                        return found;
                                                                                                                                                                                                                                                                                                                            }

                                                                                                                                                                                                                                                                                                                            public:

                                                                                                                                                                                                                                                                                                                                bool exist(vector<vector<char>>& board, string word) {

                                                                                                                                                                                                                                                                                                                                        int rows = board.size();
                                                                                                                                                                                                                                                                                                                                                int cols = board[0].size();

                                                                                                                                                                                                                                                                                                                                                        // Try every cell as a starting point.
                                                                                                                                                                                                                                                                                                                                                                for (int r = 0; r < rows; r++) {

                                                                                                                                                                                                                                                                                                                                                                            for (int c = 0; c < cols; c++) {

                                                                                                                                                                                                                                                                                                                                                                                            if (dfs(r, c, 0, board, word)) {
                                                                                                                                                                                                                                                                                                                                                                                                                return true;
                                                                                                                                                                                                                                                                                                                                                                                                                                }
                                                                                                                                                                                                                                                                                                                                                                                                                                            }
                                                                                                                                                                                                                                                                                                                                                                                                                                                    }

                                                                                                                                                                                                                                                                                                                                                                                                                                                            return false;
        
    }
};
