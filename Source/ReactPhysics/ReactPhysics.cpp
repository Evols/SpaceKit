// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#include "ReactPhysics/ReactPhysics.h"
#include "Core/Public/Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, ReactPhysics, "ReactPhysics" );

std::string my_to_string(int in)
{
    return std::to_string(in);
}

std::string my_to_string(reactphysics3d::decimal in)
{
    return std::string(TCHAR_TO_UTF8(*in.ToString()));
}
