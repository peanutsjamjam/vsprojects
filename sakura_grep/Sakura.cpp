

#include <cstdio>
#include <ctime>
#include <regex>
#include <Windows.h>
#include "Sakura.h"

Sakura::Sakura(const WCHAR* files)
	: m_tmpdir{ L"C:\\Mail\\tmp\\" }
	, m_files{ files }
	, m_folder{ L"." }
{
	WCHAR cwd[MAX_PATH];
	(void)_wgetcwd(cwd, MAX_PATH);
	m_cwd = cwd;
	std::wstring::size_type sz = m_cwd.size();
	if (sz >= 2 && m_cwd[sz - 1] != L'\\') {
		m_cwd.append(L"\\");
	}
}
Sakura::~Sakura()
{
}
void Sakura::setOpt(char k, int v)
{
	m_opt[k] = v;
}
int Sakura::getOpt(char k)
{
	return m_opt[k];
}
void Sakura::setWord(const WCHAR* word)
{
	m_word = word;
}
void Sakura::setFiles(const WCHAR* files)
{
	m_files = files;
}
void Sakura::setFolder(const WCHAR* folder)
{
	m_folder = folder;
}

void Sakura::mkFileName()
{
	time_t _t;
	tm _tm;
	time(&_t);
	localtime_s(&_tm, &_t);

	WCHAR fn[64];
	swprintf_s(fn, L"grep_log_%04d%02d%02d%02d%02d%02d.txt", _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday, _tm.tm_hour, _tm.tm_min, _tm.tm_sec);
	m_tmpfile.clear();
	m_tmpfile.append(m_tmpdir);
	m_tmpfile.append(fn);
}

void Sakura::mkCommand()
{
	m_cmd.clear();
	m_cmd.append(L"cmd /C \"sakura -SX=800 -SY=130 -WX=0 -WY=0 -GREPMODE");
	m_cmd.append(L" -GKEY=\"").append(m_word).append(L"\"");
	m_cmd.append(L" -GFILE=\"").append(m_files).append(L"\"");
	m_cmd.append(L" -GFOLDER=\"").append(m_folder).append(L"\"");
	m_cmd.append(L" -GCODE=99");
	m_cmd.append(L" -GOPT=");
	if (!m_opt['R']) {
		m_cmd.append(L"S");
	}
	if (!m_opt['i']) {
		m_cmd.append(L"L");
	}
	m_cmd.append(L"PRU");
	m_cmd.append(L" > ").append(m_tmpfile).append(L"\"");
}

void Sakura::mkProcess()
{
	WCHAR cmd[1024];
	wcscpy_s(cmd, 1024, m_cmd.c_str());

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process. 
	if (!CreateProcess(NULL,		// No module name (use command line)
		cmd,        // Command line
		NULL,       // Process handle not inheritable
		NULL,       // Thread handle not inheritable
		FALSE,      // Set handle inheritance to FALSE
		0,          // No creation flags
		NULL,       // Use parent's environment block
		NULL,       // Use parent's starting directory 
		&si,        // Pointer to STARTUPINFO structure
		&pi)        // Pointer to PROCESS_INFORMATION structure
		) {
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

void Sakura::chomp(WCHAR* line)
{
	for (int i = 0; line[i]; i++) {
		if (line[i] == L'\n' || line[i] == L'\r') {
			line[i] = L'\0';
			break;
		}
	}
}
void Sakura::printResult()
{
	FILE* stream = nullptr;

	if (_wfopen_s(&stream, m_tmpfile.c_str(), L"r, ccs=UTF-8") != 0) {
		fprintf(stderr, "error\n");
		return;
	}

	WCHAR str[1024];
	if (fgetws(str, 1024, stream) == nullptr) {
		fprintf(stderr, "error\n");
		return;
	}

	int f = 0;
	while (fgetws(str, 1024, stream) != nullptr) {
		chomp(str);
		if (wcslen(str) == 0) {
			f++;
		}
		if (wcsstr(str, L"□検索条件") == str
			|| wcsstr(str, L"検索対象") == str
			|| wcsstr(str, L"フォルダー") == str
		) {
			wprintf(L"%s\n", str);
		}
		if (f >= 2) {
			break;
		}
	}

	wprintf(L"\n");

	//  1                                        2  3     4      5
	//  C:\Mail\vsproj\Solution2\grep\Sakura.cpp(52,12)  [SJIS]:     m_tmpfile.append(m_tmpdir);
	std::wregex re(L"^(.+?)\\((\\d+),(\\d+)\\)\\s{2}\\[(.*)\\]: (.*)");
	std::wregex re_tab(L"\\t");
	std::wstring* wstrarr = new std::wstring[6];
	std::wstring spaces{};
	for (int i = 0; i < m_opt['t']; i++) {
		spaces += L" ";
	}

	while (fgetws(str, 1024, stream) != nullptr) {
		chomp(str);

		std::wcmatch m;
		if (std::regex_search(str, m, re)) {
			for (size_t i = 0; i < m.size(); i++) {
				size_t pos = m[i].first - str;
				size_t cnt = m[i].second - m[i].first;
				wstrarr[i] = { str, pos, cnt };
			}
			if (wstrarr[1].starts_with(m_cwd)) {
				wstrarr[1] = wstrarr[1].substr(m_cwd.size());
			}

			if (m_opt['t'] > 0) {
				wstrarr[5] = std::regex_replace(wstrarr[5], re_tab, spaces);
			}

			if (m_opt['h'] == 0) {
				wprintf(L"%s(%s):%s\n",
					wstrarr[1].c_str(),
					wstrarr[2].c_str(),
					wstrarr[5].c_str()
				);
			} else if (m_opt['h'] == 1) {
				wprintf(L"%s\n",
					wstrarr[5].c_str()
				);
			} else if (m_opt['h'] == 2) {
				wprintf(L"%s:%s\n",
					wstrarr[5].c_str(),
					wstrarr[1].c_str()
				);
			}
		} else {
			wprintf(L"%s\n", str);
		}
	}

	delete[] wstrarr;
	fclose(stream);
}

void Sakura::delTmpFile()
{
	if (m_opt['n'])
		return;
	if (_wunlink(m_tmpfile.c_str()) == -1) {
		printf("_wunlink error\n");
	}
}

