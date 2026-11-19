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
    int dfs(TreeNode* root, int maxVal) {

        if (root == nullptr) {
            return 0;
        }

        int count = 0;

        // Check whether current node is good
        if (root->val >= maxVal) {
            count = 1;
        }

        // Update maximum value along the path
        maxVal = max(maxVal, root->val);

        // Explore left and right subtrees
        count += dfs(root->left, maxVal);
        count += dfs(root->right, maxVal);

        return count;
    }

    int goodNodes(TreeNode* root) {

        if (root == nullptr) {
            return 0;
        }

        return dfs(root, root->val);
    }
};
