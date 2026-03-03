#pragma once 

#include <CCCommon.h>
#include <random>
#include <string>
#include <algorithm>
#include <vector>

namespace CitrusCore
{
    class CITRUS_CORE_API StringUtil
    {
    public:
        static constexpr std::string ToLower(const std::string& str) {
            std::string lowerStr = str;
            std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                [](unsigned char c) { return std::tolower(c); });
            return lowerStr;
        }

        static std::string GenerateRandomHexString(int length)
        {       
            std::vector<char> alphanum = 
                {'0','1','2','3','4',
                '5','6','7','8','9',
                'a','b','c','d','e','f',
                'g','h','i','j','k',
                'l','m','n','o','p',
                'q','r','s','t','u',
                'v','w','x','y','z'
            }; 

            // https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution.html
            static std::mt19937 gen([]{
                std::random_device rd;
                return std::mt19937(rd());
            }());
            
            std::uniform_int_distribution<> distrib(0, alphanum.size() - 1);

            std::string str(length,0);
            std::generate_n( str.begin(), length, [&]() { 
                return alphanum[distrib(gen)];
            } );
            return str;
        }
    };
}
