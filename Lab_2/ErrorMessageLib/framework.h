#pragma once

#include <iostream>
#include "Winsock2.h"

#define WIN32_LEAN_AND_MEAN        

std::string SetErrorMsgText(int code);
std::string SetErrorMsgText(std::string msgText, int code);
