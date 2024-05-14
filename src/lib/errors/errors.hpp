#pragma once

#include <iostream>
#include <exception>

class ArgumentException : public std::exception {
private:
    std::string errStr;
public:
    ArgumentException(std::uint64_t errorAddress);
    ~ArgumentException();
    const char* what();
};