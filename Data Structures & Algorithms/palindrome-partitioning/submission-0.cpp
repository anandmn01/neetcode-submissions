class Solution {
private:

    // Check whether a substring is a palindrome.
        bool isPalindrome(string& s, int left, int right) {

                while (left < right) {

                            if (s[left] != s[right]) {
                                            return false;
                                                        }

                                                                    left++;
                                                                                right--;
                                                                                        }

                                                                                                return true;
                                                                                                    }

                                                                                                        void backtrack(
                                                                                                                int start,
                                                                                                                        string& s,
                                                                                                                                vector<string>& current,
                                                                                                                                        vector<vector<string>>& result
                                                                                                                                            ) {

                                                                                                                                                    // Entire string has been partitioned.
                                                                                                                                                            if (start == s.size()) {
                                                                                                                                                                        result.push_back(current);
                                                                                                                                                                                    return;
                                                                                                                                                                                            }

                                                                                                                                                                                                    // Try every possible ending position.
                                                                                                                                                                                                            for (int end = start; end < s.size(); end++) {

                                                                                                                                                                                                                        // Consider only palindrome substrings.
                                                                                                                                                                                                                                    if (isPalindrome(s, start, end)) {

                                                                                                                                                                                                                                                    // Choose the palindrome.
                                                                                                                                                                                                                                                                    current.push_back(s.substr(start, end - start + 1));

                                                                                                                                                                                                                                                                                    // Explore the remaining string.
                                                                                                                                                                                                                                                                                                    backtrack(end + 1, s, current, result);

                                                                                                                                                                                                                                                                                                                    // Backtrack: remove the selected substring.
                                                                                                                                                                                                                                                                                                                                    current.pop_back();
                                                                                                                                                                                                                                                                                                                                                }
                                                                                                                                                                                                                                                                                                                                                        }
                                                                                                                                                                                                                                                                                                                                                            }

                                                                                                                                                                                                                                                                                                                                                            public:

                                                                                                                                                                                                                                                                                                                                                                vector<vector<string>> partition(string s) {

                                                                                                                                                                                                                                                                                                                                                                        vector<vector<string>> result;
                                                                                                                                                                                                                                                                                                                                                                                vector<string> current;

                                                                                                                                                                                                                                                                                                                                                                                        backtrack(0, s, current, result);

                                                                                                                                                                                                                                                                                                                                                                                                return result;
    }
};
