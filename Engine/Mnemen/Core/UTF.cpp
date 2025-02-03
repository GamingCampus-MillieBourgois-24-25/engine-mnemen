//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 06:20:56
//

#include <Core/UTF.hpp>

#include <codecvt>

String UTF::WideToAscii(const wchar_t* psText)
{
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    return converter.to_bytes(psText);
}

String UTF::WideToAscii(const WideString& sText)
{
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    return converter.to_bytes(sText.data());
}

WideString UTF::AsciiToWide(const char* sText)
{
    String Wrapped(sText);
    return WideString(Wrapped.begin(), Wrapped.end());
}

WideString UTF::AsciiToWide(const String& sText)
{
    return WideString(sText.begin(), sText.end());
}