class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<vector<string>>ans;
        unordered_map<string, vector<string>> mp;

       for(auto & x:strs){
        string temp=x;
        vector<string>t;
        sort(x.begin(),x.end());

        if(mp.find(x)!=mp.end()){
            mp[x].push_back(temp);
        }else{
            t.push_back(temp);
            mp[x]=t;
        }
        x=temp;
       }
       for(auto &y:mp){
        ans.push_back(y.second);
       }
       return ans;
    }
};
