class Solution {
public:

string palindromeStr(string &s){
    string temp="";
    for(int i=0;i<s.size();i++){
        if(s[i]>='a'&& s[i]<='z'){
            temp+=s[i];
        }
        else if (s[i]>='A'&&s[i]<='Z'){
            temp+=tolower(s[i]);
            }
            else if(s[i]>='0'&&s[i]<='9'){
                temp+=s[i];  
            
        }
        
    }
    return temp;
}

    bool isPalindrome(string s) {

      string str =  palindromeStr(s);
        int i=0;
        int j=str.size()-1;
        while(i<j){
            if(str[i]!=str[j]){
                return false;
            }
            i++;
            j--;
        }
        return true;
    }
};
