#pragma once
#include "pch.h"
//#include "ImGui/imgui.h"
//#include <stdio.h>
//#include <fstream>
class MyFile
{
public:
	static int SaveInFile(ImGuiStyle* style, const char *FileName);
	static int ReadFile(ImGuiStyle* style,const char *Filename ="/style.dat");
};

