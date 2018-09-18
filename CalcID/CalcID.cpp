//
//  CalcID.cpp
//
//  Created by yangqi on 2012/12/11.
//
#include <string>

// 通过身份证前 17 位计算最后的校验位
std::string CalcID(const std::string& strID)
{
    const int n[17] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};

    std::string strRet = "-1";

    if (strID.length() == 17)
    {
        int nSum = 0;

        const char* pszID = strID.c_str();
        for (int i = 0; i < 17; ++i)
        {
            int ch = pszID[i] - 0x30;
            ch *= n[i];
            nSum += ch;
        }

        const int nMod = nSum%11;
        const int nn[11] = {1, 0, 10, 9, 8, 7, 6, 5, 4, 3, 2};

        switch (nn[nMod])
        {
            case 10:
                strRet = "X";
                break;
            default:
                strRet.assign(1, nn[nMod] + 0x30);
                break;
        }
    }

    return strRet;
}
