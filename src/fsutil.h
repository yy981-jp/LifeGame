#pragma once

#include <string>

#include "def.h"


std::wstring to_wstring(const std::string& u8);
std::string to_u8string(const std::wstring& u16);

std::string openFileDialog();
std::string saveFileDialog();

void writeJsonFile(const json& j, const std::string& path);
json readJsonFile(const std::string& path);
std::vector<std::vector<uint8_t>> readLGSFile(const std::string& path);
