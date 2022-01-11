/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#include "MyException.hpp"

MyException::MyException(const std::string &what, const std::string &function,
    const std::string &file, int line
) : _what("what: " + what), _func("function: " + function),
    _file("file: " + file), _line("line: " + std::to_string(line)),
    _where("In: " + _file + ", in " + _func + ", at " + _line)
{
}

const char *MyException::what() const noexcept
{
    return _what.c_str();
}

const char *MyException::where() const noexcept
{
    return _where.c_str();
}

const char *MyException::whichFunction() const noexcept
{
    return _func.c_str();
}

const char *MyException::whichFile() const noexcept
{
    return _file.c_str();
}

const char *MyException::whichLine() const noexcept
{
    return _line.c_str();
}

std::ostream &operator<<(std::ostream &os, const MyException &exception)
{
    os << exception.what() << ", " << exception.where();
    return os;
}