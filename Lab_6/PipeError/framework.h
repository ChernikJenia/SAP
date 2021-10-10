#pragma once

#include <iostream>
#include <Windows.h>

#define WIN32_LEAN_AND_MEAN             

std::string GetPipeError(int code);
std::string SetPipeError(std::string msgText, int code);
