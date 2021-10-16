#pragma once

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
#include <iostream>
#include <Windows.h>

std::string GetMailslotError(int code);
std::string SetMailslotError(std::string msgText, int code);