class Solution {
public:

    string encode(vector<string>& strs) {
 string ans = "";

   for(auto &x:strs) {

    ans += to_string(x.size());
    ans += '#';
    ans += x;

    }

    return ans;
    }

    vector<string> decode(string s) {

     vector<string> ans;

        int i = 0;

        while(i < s.size()) {

            int j = s.find('#', i);

            int len = stoi(s.substr(i, j-i));

            string word = s.substr(j+1, len);

            ans.push_back(word);

            i = j + 1 + len;
        }

        return ans;
    }
};
