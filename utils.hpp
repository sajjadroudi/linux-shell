#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

using namespace std;

template<typename T>
T* toArray(const vector<T>& input) {
    auto output = new T[input.size()];
    std::copy(input.begin(), input.end(), output);
    return output;
}

char** toCharArrayArray(const vector<string>& input) {
    auto output = new vector<char*>();
    std::transform(input.begin(), input.end(), std::back_inserter(*output), [](const string& s) {
        char* pc = new char[s.size() + 1];
        strcpy(pc, s.c_str());
        return pc;
    });
    return toArray(*output);
}

vector<string>* split(string input, const string& delimiter) {
    auto output = new vector<string>;

    size_t pos = 0;
    string token;
    while ((pos = input.find(delimiter)) != string::npos) {
        token = input.substr(0, pos);
        output->push_back(token);
        input.erase(0, pos + delimiter.length());
    }
    output->push_back(input);

    return output;
}

template<typename T>
T* appendNull(const T array[], int length) {
    int newLength = length + 1;
    T* newArray = new T[newLength];

    for(int i = 0; i < length; i++) {
        newArray[i] = array[i];
    }

    newArray[newLength - 1] = NULL;

    return newArray;
}

static inline string &ltrim(string &s) {
    s.erase(
            s.begin(),
            find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );
    return s;
}

static inline string &rtrim(string &s) {
    s.erase(
            find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
            s.end()
    );
    return s;
}

static inline string &trim(string &s) {
    return ltrim(rtrim(s));
}

string replace(string str, const string& from, const string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == string::npos)
        return str;
    str.replace(start_pos, from.length(), to);
    return str;
}