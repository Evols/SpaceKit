// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#pragma once

#include "CoreMinimal.h"

#include <sstream>

inline bool IsFloat(FString Str)
{
	std::string myString = TCHAR_TO_ANSI(*Str);
    std::istringstream iss(myString);
    float f;
    iss >> std::noskipws >> f;
    return iss.eof() && !iss.fail(); 
}
