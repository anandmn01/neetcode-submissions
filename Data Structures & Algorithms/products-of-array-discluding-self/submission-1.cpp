class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
     int n = nums.size();
        vector<int> prefix(n, 1);
        vector<int>suffix(n,1);

        vector<int>ans(n,1);

        int pre=1;
        for(int i=0;i<n;i++){
            prefix[i]=pre;
            pre=pre*nums[i];
        }

        int suff=1;
        for(int i=n-1;i>=0;i--){
            suffix[i]=suff;
            suff=suff*nums[i];
        }

        for(int i=0;i<n;i++){
          ans[i]=prefix[i]*suffix[i];
        }
return ans;


    }
};
