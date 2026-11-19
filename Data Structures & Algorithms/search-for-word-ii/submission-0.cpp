class Solution {
private:

    struct Node {
        Node* links[26];
        string word;

        Node() {
            for (int i = 0; i < 26; i++) {
                links[i] = nullptr;
            }

            word = "";
        }
    };

    Node* root;

    // Insert all words into Trie.
    void insert(string& word) {

        Node* node = root;

        for (char ch : word) {

            int index = ch - 'a';

            if (node->links[index] == nullptr) {
                node->links[index] = new Node();
            }

            node = node->links[index];
        }

        // Store the complete word at its ending node.
        node->word = word;
    }

    // DFS + Backtracking.
    void dfs(
        vector<vector<char>>& board,
        int row,
        int col,
        Node* node,
        vector<string>& result
    ) {

        char ch = board[row][col];
        int index = ch - 'a';

        // No matching Trie path.
        if (node->links[index] == nullptr) {
            return;
        }

        Node* nextNode = node->links[index];

        // If a complete word is found.
        if (nextNode->word != "") {

            result.push_back(nextNode->word);

            // Prevent duplicate results.
            nextNode->word = "";
        }

        // Mark current cell as visited.
        board[row][col] = '#';

        // Four possible directions.
        int directions[5] = {-1, 0, 1, 0, -1};

        for (int i = 0; i < 4; i++) {

            int newRow = row + directions[i];
            int newCol = col + directions[i + 1];

            // Check board boundaries and visited cells.
            if (
                newRow >= 0 &&
                newRow < board.size() &&
                newCol >= 0 &&
                newCol < board[0].size() &&
                board[newRow][newCol] != '#'
            ) {

                dfs(
                    board,
                    newRow,
                    newCol,
                    nextNode,
                    result
                );
            }
        }

        // Restore the original character.
        board[row][col] = ch;
    }

public:

    vector<string> findWords(
        vector<vector<char>>& board,
        vector<string>& words
    ) {

        root = new Node();

        // Step 1: Build Trie.
        for (string& word : words) {
            insert(word);
        }

        vector<string> result;

        int rows = board.size();
        int cols = board[0].size();

        // Step 2: Start DFS from every cell.
        for (int i = 0; i < rows; i++) {

            for (int j = 0; j < cols; j++) {

                dfs(board, i, j, root, result);
            }
        }

        return result;
        
    }
};
