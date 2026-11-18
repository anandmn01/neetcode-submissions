class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        // Step 1: Detect the cycle
        int slow = nums[0];
        int fast = nums[0];

        do {
            slow = nums[slow];
            fast = nums[nums[fast]];

        } while (slow != fast);

        // Step 2: Find the entrance of the cycle
        slow = nums[0];

        while (slow != fast) {
            slow = nums[slow];
            fast = nums[fast];
        }

        return slow;
    }
};
