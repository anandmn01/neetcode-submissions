class Solution {
public:
    void expand(string& s, int left, int right, int& count) {

        while (left >= 0 &&
               right < s.length() &&
               s[left] == s[right]) {

            count++;

            left--;
            right++;
        }
    }

    int countSubstrings(string s) {
        int n = s.length();
        int count = 0;

        for (int i = 0; i < n; i++) {

            // Odd-length palindromes
            expand(s, i, i, count);

            // Even-length palindromes
            expand(s, i, i + 1, count);
        }

        return count;
        
    }
};
