//
// Created by noobcoder on 22/02/2019.
//

#include "BSON.h"

size_t BSON::parseString(const wchar_t * buffer, size_t length, size_t pos, wstring &output)
{
    pos = skipBlank(buffer, length, pos);
    wchar_t current = buffer[pos]; // current character
    bool isEscape = false;
    bool isEscapeUnicode = false;

    if (current != TOKEN::STRING_QUOTE) return 0;
    ++pos;
    output = L"";

    while (pos < length) {
        current = buffer[pos];
        if (current == TOKEN::STRING_QUOTE) break;
        if (current == TOKEN::ESCAPE_SLASH) {
            if (isEscape) {
                isEscape = false;
                output.push_back(L'\\');
            } else {
                isEscape = true;
            }
        } else {
            if (isEscape) {
                isEscape = false;
                switch (current) {
                    case L't': output.push_back(L'\t'); break;
                    case L'n': output.push_back(L'\n'); break;
                    case L'r': output.push_back(L'\r'); break;
                    case L'u': isEscapeUnicode = true; break;
                    default:
                        return 0;
                }
            } else {
                if (isEscapeUnicode) {
                    isEscapeUnicode = false;
                    bool canRead = (isDigit(current))
                            && (pos+1 < length && isDigit(buffer[pos + 1]))
                               && (pos+2 < length && isDigit(buffer[pos + 2]))
                                  && (pos+3 < length && isDigit(buffer[pos + 3]));
                    if (canRead) {
                        wchar_t unicodeChar = 0;
                        for (size_t j = 0; j < 4; ++j) {
                            unicodeChar = unicodeChar * 10 + (buffer[pos + j] - L'0');
                        }
                        output.push_back(unicodeChar);
                    } else {
                        return 0;
                    }
                }
                output.push_back(current);
            }
        }

        ++pos;
    }

    return pos + 1;
}

bool BSON::isBlank(const wchar_t & c) {
    switch (c) {
        case L' ':
        case L'\t':
        case L'\r':
        case L'\n':
            return true;
        default:
            return false;
    }
}

size_t BSON::skipTo(const wchar_t *buffer, size_t length, size_t pos, const wchar_t &c) {
    while (pos < length && buffer[pos] != c) {
        ++pos;
    }
    return pos;
}

size_t BSON::skipBlank(const wchar_t *buffer, size_t length, size_t pos) {
    while (pos < length && isBlank(buffer[pos])) {
        ++pos;
    }
    return pos;
}

bool BSON::isDigit(const wchar_t & c) {
    return (c >= L'0' && c <= L'9');
}

size_t BSON::parseNumber(const wchar_t *buffer, size_t length, size_t pos, int &output) {
    pos = skipBlank(buffer, length, pos);
    output = 0;
    bool isNegative = false;
    bool isRead = false;

    if (buffer[pos] == TOKEN::NEGATIVE) {
        ++pos;
        isNegative = true;
    }

    while (pos < length) {
        wchar_t current = buffer[pos];
        ++pos;
        if (isBlank(current)
            || current == TOKEN::COMMA
            || current == TOKEN::OBJECT_CLOSE
            || current == TOKEN::ARRAY_CLOSE) {
            break;
        }
        if (isDigit(current)) {
            output = output * 10 + (current - L'0');
            isRead = true;
        } else {
            return 0;
        }
    }

    if (!isRead) return 0;
    if (isNegative) output *= -1;

    return pos;
}

BSON BSON::get(wstring key) {
    return value[key]->get<BSON>();
}

int BSON::getInt(wstring key) {
    return value[key]->get<int>();
}

wstring BSON::getString(wstring key) {
    return value[key]->get<wstring>();
}

size_t BSON::parseJSON(const wchar_t *buffer, size_t length, size_t pos, BSON &output) {
    pos = skipBlank(buffer, length, pos);

    if (buffer[pos] != TOKEN::OBJECT_OPEN) {
        return 0;
    }
    ++pos;

    while (pos < length) {
        wstring key;
        int valInt;
        wstring valString;
        BSON valBson;

        // parse key : always string
        pos = parseString(buffer, length, pos, key);
        if (pos == 0) return 0;

        // behind key is always ':'
        pos = skipTo(buffer, length, pos, TOKEN::COLON);
        if (pos == 0) return 0;
        ++pos;

        // parse value
        size_t nextPos = 0;
        nextPos = parseString(buffer, length, pos, valString);
        if (nextPos == 0) {
            nextPos = parseNumber(buffer, length, pos, valInt);
            if (nextPos == 0) {
                nextPos = parseJSON(buffer, length, pos, valBson);
                if (nextPos == 0) return 0;
                else {
                    output.put<BSON>(key, valBson);
                }
            } else {
                output.put<int>(key, valInt);
            }
        } else {
            output.put<wstring>(key, valString);
        }

        pos = skipBlank(buffer, length, nextPos);
        if (buffer[pos] != TOKEN::COMMA && buffer[pos] != TOKEN::OBJECT_CLOSE) {
            return 0;
        }
        if (buffer[pos] == TOKEN::OBJECT_CLOSE) break;
        ++pos;
    }

    return pos + 1;
}

template <class T>
void BSON::put(wstring key, const T & value)
{
    if (this->value[key] == nullptr) {
        this->value[key] = new Any();
    }
    this->value[key]->set<T>(value);
}

