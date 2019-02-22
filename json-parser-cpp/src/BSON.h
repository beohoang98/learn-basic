//
// Created by noobcoder on 22/02/2019.
//

#ifndef JSON_PARSER_CPP_BSON_H
#define JSON_PARSER_CPP_BSON_H

#include <cwchar>
#include <string>
#include <map>
#include "Any.h"

#define _t const wchar_t
namespace TOKEN {
    _t STRING_QUOTE = L'"';
    _t ESCAPE_SLASH = L'\\';
    _t COLON = L':';
    _t COMMA = L',';
    _t OBJECT_OPEN = L'{';
    _t OBJECT_CLOSE = L'}';
    _t ARRAY_OPEN = L'[';
    _t ARRAY_CLOSE = L']';
    _t NEGATIVE = L'-';
};
#undef _t

using namespace std;

typedef map<wstring, Any*> _BSON;

class BSON {
private:
    _BSON value;

public:
    BSON get(wstring key);
    int getInt(wstring key);
    wstring getString(wstring key);

    template <class T>
    void put(wstring key, const T & value);

    /// PROTOTYPE

    static bool isBlank(const wchar_t&);
    static bool isDigit(const wchar_t&);
    static size_t skipTo(const wchar_t * buffer, size_t length, size_t pos, const wchar_t& c);
    static size_t skipBlank(const wchar_t * buffer, size_t length, size_t pos);
    /**
     *
     * @param buffer source text
     * @param length length of buffer
     * @param pos current position to parse next
     * @param output value parsed
     * @return next position, 0 if cannot parse string
     */
    static size_t parseString(const wchar_t * buffer, size_t length, size_t pos, wstring& output);
    static size_t parseNumber(const wchar_t * buffer, size_t length, size_t pos, int & output);
    static size_t parseJSON(const wchar_t * buffer, size_t length, size_t pos, BSON & output);
};


#endif //JSON_PARSER_CPP_BSON_H
