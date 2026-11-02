class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        int totalsum=0;
        vector<int>ans;
        for(int i=0;i<nums.size()-1;i++){
            int take=nums[i];
            for(int j=1;j<nums.size();j++){
               if(nums[i]+nums[j]==target && i!=j){
                ans.push_back(i);
                ans.push_back(j);
                return ans;
               }
            }
        }
    }
};
