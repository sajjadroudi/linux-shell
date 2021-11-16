//
// Created by sajjad on 11/15/21.
//

#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <iostream>

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

vector<string>* splitOnSpace(string input) {
    char delimiter = ' ';
    auto output = new vector<string>;

    size_t pos = 0;
    string token;
    while ((pos = input.find(delimiter)) != string::npos) {
        token = input.substr(0, pos);
        output->push_back(token);
        input.erase(0, pos + 1);
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