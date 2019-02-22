//
// Created by noobcoder on 22/02/2019.
//

#ifndef JSON_PARSER_CPP_ANY_H
#define JSON_PARSER_CPP_ANY_H

#include <string>
#include <exception>

using namespace std;

class Any {
private:
    string type;
    long pointer;
public:
    template <class T>
    void set(const T &value) {
        free((T*)pointer);
        type = typeid(T).name();

        T * tpointer = new T;
        *tpointer = value;
        pointer = (long)(tpointer);
    }

    template <class T>
    T get() {
        string tname = typeid(T).name();
        if (type != tname) throw bad_alloc();
        return *((T*)pointer);
    }
};


#endif //JSON_PARSER_CPP_ANY_H
