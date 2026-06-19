#include <string>
#include <cassert>
#include <algorithm>
#include <cctype>

std::string reverse_words(const std::string &str)
{
    std::string result = str;
    std::size_t i = 0;

    while (i < result.size())
    {
        if (std::isalnum(static_cast<unsigned char>(result[i])))
        {
            std::size_t start = i;

            while (i < result.size() &&
                   std::isalnum(static_cast<unsigned char>(result[i])))
            {
                ++i;
            }

            std::reverse(result.begin() + start, result.begin() + i);
        }
        else
        {
            ++i;
        }
    }

    return result;
}

int main()
{
    // Given test
    assert(reverse_words("String; 2be reversed...") ==
           "gnirtS; eb2 desrever...");

    // Empty string
    assert(reverse_words("") == "");

    // Only punctuation
    assert(reverse_words("...,,!!") == "...,,!!");

    // Single word
    assert(reverse_words("hello") == "olleh");

    // Multiple spaces
    assert(reverse_words("ab  cd") == "ba  dc");

    // Mixed letters and digits
    assert(reverse_words("abc123 def456") ==
           "321cba 654fed");

    // Leading and trailing punctuation
    assert(reverse_words("!hello,world!") ==
           "!olleh,dlrow!");

    return 0;
}