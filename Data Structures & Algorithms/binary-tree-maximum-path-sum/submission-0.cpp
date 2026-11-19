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
   
int maxSum = INT_MIN;

    int dfs(TreeNode* root) {

        // Base case
        if (root == nullptr) {
            return 0;
        }

        // Calculate left and right maximum gains
        int leftGain = max(0, dfs(root->left));
        int rightGain = max(0, dfs(root->right));

        // Maximum path passing through current node
        int currentPath = root->val + leftGain + rightGain;

        // Update global maximum
        maxSum = max(maxSum, currentPath);

        // Return one branch to parent
        return root->val + max(leftGain, rightGain);
    }

    int maxPathSum(TreeNode* root) {

        maxSum = INT_MIN;

        dfs(root);

        return maxSum;
    }
};
