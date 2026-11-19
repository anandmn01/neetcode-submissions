class WordDictionary {
private:

    struct Node {
        Node* links[26];
        bool isEnd;

        Node() {
            for (int i = 0; i < 26; i++) {
                links[i] = nullptr;
            }

            isEnd = false;
        }
    };

    Node* root;

    // DFS helper function
    bool dfs(Node* node, string& word, int index) {

        // All characters have been processed.
        if (index == word.size()) {
            return node->isEnd;
        }

        char ch = word[index];

        // Wildcard: explore every available child.
        if (ch == '.') {

            for (int i = 0; i < 26; i++) {

                if (node->links[i] != nullptr) {

                    if (dfs(node->links[i], word, index + 1)) {
                        return true;
                    }
                }
            }

            return false;
        }

        // Normal character: follow only its matching child.
        int position = ch - 'a';

        if (node->links[position] == nullptr) {
            return false;
        }

        return dfs(node->links[position], word, index + 1);
    }

public:

    WordDictionary() {
        root = new Node();
    }

    void addWord(string word) {

        Node* node = root;

        for (char ch : word) {

            int index = ch - 'a';

            if (node->links[index] == nullptr) {
                node->links[index] = new Node();
            }

            node = node->links[index];
        }

        node->isEnd = true;
    }

    bool search(string word) {

        return dfs(root, word, 0);
        
    }
};
