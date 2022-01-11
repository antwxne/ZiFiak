/*
** EPITECH PROJECT, 2024
** Project
** File description:
** Created by antoine,
*/

#include <iostream>
#include "Debug/Debug.hpp"

int main()
{
    try {
        std::cout << "hello world!" << std::endl;
        throw MyException("Ca marche", __func__, __FILE__, __LINE__);
    } catch (const MyException &e) {
        Debug::err(e);
    }
    return 0;
}