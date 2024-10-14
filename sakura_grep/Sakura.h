#pragma once

#include <string>
#include <unordered_map>
#include <Windows.h>

class Sakura
{
private:
	std::unordered_map<char, int> m_opt;
	std::wstring m_cwd;
	std::wstring m_word;
	std::wstring m_tmpdir;
	std::wstring m_tmpfile;
	std::wstring m_files;
	std::wstring m_folder;
	std::wstring m_cmd;
protected:
public:
	Sakura(const WCHAR* tmpdir, const WCHAR* files);
	virtual ~Sakura();
	void setOpt(char k, int v);
	int getOpt(char k);
	void setWord(const WCHAR* word);
	void setFiles(const WCHAR* files);
	void setFolder(const WCHAR* folder);
	void mkFileName();
	void mkCommand();
	void mkProcess();
	void printResult();
	void delTmpFile();
	void chomp(WCHAR* line);

};
