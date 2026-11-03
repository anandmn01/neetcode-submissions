class Solution {
public:

    vector<string>my_ans;
    string encode(vector<string>& strs) {
        string ans = "";

   for(auto &x:strs) {

    ans += x.size();
    ans += '#';
    ans += x;
    my_ans.push_back(x);

    }

    return ans;
    }

    vector<string> decode(string s) {
          return my_ans;
    }
};
