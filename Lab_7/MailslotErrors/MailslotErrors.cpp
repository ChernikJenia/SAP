#include "framework.h"

std::string GetMailslotError(int code)
{
	std::string msgText;

	switch (code)
	{
	case ERROR_ALREADY_EXISTS: msgText = "Cannot create a file when that file already exists\n"; break;
	case ERROR_FILE_NOT_FOUND: msgText = "The system cannot find the file specified\n";		  break;
	case ERROR_INVALID_PARAMETER: msgText = "The parameter is incorrect\n";					  break;
	case ERROR_SEM_TIMEOUT:		 msgText = "Время ожидания истекло\n";						  break;
	default:					 msgText = "Have no connection with Mailslot \n";			  break;
	};

	return msgText;
};

std::string SetMailslotError(std::string msgText, int code)
{
	return msgText + GetMailslotError(code);
};