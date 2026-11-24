class Solution {
public:
    uint32_t reverseBits(uint32_t n) {
        string binary = "";

        for (int i = 0; i < 32; i++) {

            binary += (n & 1) ? '1' : '0';

            n >>= 1;
        }

        uint32_t result = 0;

        for (int i = 0; i < 32; i++) {

            result = (result << 1) | (binary[i] - '0');
        }

        return result;
    }
};
