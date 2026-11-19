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

class Solution {
public:
    unordered_map<int, int> inorderMap;
    int preorderIndex = 0;

    TreeNode* build(
        vector<int>& preorder,
        int left,
        int right
    ) {

        // No elements in this subtree
        if (left > right) {
            return nullptr;
        }

        // First available preorder element is the root
        int rootValue = preorder[preorderIndex++];

        TreeNode* root = new TreeNode(rootValue);

        // Find root position in inorder
        int mid = inorderMap[rootValue];

        // Construct left subtree
        root->left = build(preorder, left, mid - 1);

        // Construct right subtree
        root->right = build(preorder, mid + 1, right);

        return root;
    }

    TreeNode* buildTree(
        vector<int>& preorder,
        vector<int>& inorder
    ) {

        preorderIndex = 0;
        inorderMap.clear();

        // Store inorder values and their indices
        for (int i = 0; i < inorder.size(); i++) {
            inorderMap[inorder[i]] = i;
        }

        return build(preorder, 0, inorder.size() - 1);
        
    }
};
