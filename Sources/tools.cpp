#include <iostream>
#include <vector>
#include <algorithm>

namespace tools
{
    using std::string;
    using std::vector;

    //aのb番目の文字からc文字を取り出す
    string SplitString(string value, const int index = -1, int range = 0)
    {
        if (range == 0)
            range = (value).length();

        string str = "";
        if (index < 0)
            return "";

        for (int i = 0; range >= i + index; i++)
        {
            str += (value).at(index + i);
        }

        return str;
    }

    int GetStringVectorIndex(vector<string> base, const string value)
    {
        vector<string>::iterator iterator = std::find(base.begin(), base.end(), value);

        int index = std::distance(base.begin(), iterator);

        return index;
    }

    //バイトaのb番目(2桁)を取り出す
    uint32_t GetByte(int byte, const int index = -1)
    {
        if (index < 1)
            return -1;

        return (byte >> (8 * (index - 1))) & 0xFF;
    }

    //バイトaのb番目(2桁)をcにする(参照渡し)
    void SetByte(uint32_t &byte, int index = -1, const uint8_t value = 0) {
        if (index < 1)
            return;

        index -= 1;

        byte &= ~(0xFFu << (8 * index));
        byte |= (value << (8 * index));
    }
}