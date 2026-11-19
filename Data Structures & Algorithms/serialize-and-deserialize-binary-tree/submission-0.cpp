/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {

        if (root == nullptr) {
            return "#,";
        }

        string result = to_string(root->val) + ",";

        result += serialize(root->left);
        result += serialize(root->right);

        return result;
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {

        stringstream ss(data);
        return buildTree(ss);
    }

private:

    TreeNode* buildTree(stringstream& ss) {

        string value;
        getline(ss, value, ',');

        // Null marker means no node exists here.
        if (value == "#") {
            return nullptr;
        }

        // Create the current node.
        TreeNode* root = new TreeNode(stoi(value));

        // Reconstruct left subtree first.
        root->left = buildTree(ss);

        // Then reconstruct right subtree.
        root->right = buildTree(ss);

        return root;
    }
};
