#include <string>
#include <fstream>
#include <filesystem>

#include <windows.h>
#include <commdlg.h>

#include <boost/locale.hpp>

#include "def.h"

namespace fs = std::filesystem;


inline std::wstring to_wstring(const std::string& u8) {
	return boost::locale::conv::utf_to_utf<wchar_t>(u8);
}

inline std::string to_u8string(const std::wstring& u16) {
	return boost::locale::conv::utf_to_utf<char>(u16);
}


std::string openFileDialog() {
	wchar_t fileName[MAX_PATH] = L"";

	OPENFILENAMEW ofn{};
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = 
        L"LGS(部分構造) Files (*.lgs)\0*.lgs\0"
        L"JSON(全体構造) Files (*.json)\0*.json\0"
        L"JSON(全体構造) / LGS(部分構造) Files (*.json;*.lgs)\0*.json;*.lgs\0\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameW(&ofn)) return to_u8string(fileName);
	return "";
}

std::string saveFileDialog() {
    wchar_t fileName[MAX_PATH] = L"";

    OPENFILENAMEW ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"JSON Files (*.json)\0*.json\0\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrDefExt = L"json";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

    if (GetSaveFileNameW(&ofn))
        return to_u8string(fileName);

    return "";
}

void writeJsonFile(const json& j, const std::string& path) {
	std::ofstream ofs(fs::path(to_wstring(path)));
	if (!ofs) throw std::runtime_error("writeJsonFile(): ofs");
	ofs << j;
}

json readJsonFile(const std::string& path) {
	json j;
	std::ifstream ifs(fs::path(to_wstring(path)));
	if (!ifs) throw std::runtime_error("readJsonFile(): ifs");
	ifs >> j;
	return j;
}

std::vector<std::vector<uint8_t>> readLGSFile(const std::string& path) {
	std::vector<std::vector<uint8_t>> map;
	std::ifstream ifs(fs::path(to_wstring(path)));
	if (!ifs) throw std::runtime_error("readLGSFile(): ifs");
	std::string line;
	int width = 0;
	while (std::getline(ifs,line)) {
		if (line.empty()) continue;
		if (!width) width = line.size();
		std::vector<uint8_t> lineMap(width);
		for (int i = 0; i < width; ++i) {
			lineMap[i] = line[i]-'0';
		}
		map.emplace_back(lineMap);
	}
	return map;
}
