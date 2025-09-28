#include <iostream>

namespace tools
{
    using namespace std;

    //(string).substrの代わり
    void SplitString(string *value, const int index = -1, int range = 0);

    int GetStringVectorIndex(const vector<string> base, const string value);

    int GetByte(int value, const int index = -1);

    void SetByte(uint32_t &byte, const int index = -1, const uint8_t value = 0);
}