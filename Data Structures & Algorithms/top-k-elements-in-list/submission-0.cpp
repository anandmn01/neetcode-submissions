class Solution {
public:

    vector<int> topKFrequent(vector<int>& nums, int k) {
         vector<int> ans;
        vector<pair<int,int>> pair_ele;

        unordered_map<int,int> mp;

        // Step 1: Calculate frequency
        for(auto &x : nums) {
            mp[x]++;
        }

        // Step 2: Store frequency and number
        for(auto &t : mp) {
            pair_ele.push_back({t.second, t.first});
        }

        // Step 3: Sort by frequency
        sort(pair_ele.begin(), pair_ele.end());

        // Step 4: Take top k elements
        for(int i = pair_ele.size()-1; i >= 0 && ans.size() < k; i--) {
            ans.push_back(pair_ele[i].second);
        }

        return ans;



    }
};
