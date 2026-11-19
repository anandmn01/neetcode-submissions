class PrefixTree {
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

public:

    PrefixTree() {
        root = new Node();
    }

    void insert(string word) {

        Node* node = root;

        for (char ch : word) {

            int index = ch - 'a';

            // Create a node if this character path doesn't exist.
            if (node->links[index] == nullptr) {
                node->links[index] = new Node();
            }

            // Move to the next character node.
            node = node->links[index];
        }

        // Mark the end of a complete word.
        node->isEnd = true;
    }

    bool search(string word) {

        Node* node = root;

        for (char ch : word) {

            int index = ch - 'a';

            // Character path doesn't exist.
            if (node->links[index] == nullptr) {
                return false;
            }

            node = node->links[index];
        }

        // The path exists, but is it a complete word?
        return node->isEnd;
    }

    bool startsWith(string prefix) {

        Node* node = root;

        for (char ch : prefix) {

            int index = ch - 'a';

            // Prefix path doesn't exist.
            if (node->links[index] == nullptr) {
                return false;
            }

            node = node->links[index];
        }

        // Every character in the prefix was found.
        return true;
        
    }
};
