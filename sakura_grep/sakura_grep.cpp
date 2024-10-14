// sakura_grep.cpp

#include <cstdio>
#include <cstdlib>
#include <clocale>
#include <ctime>
#include <Windows.h>
#include "getopt.h"
#include "Sakura.h"

int wmain(int argc, WCHAR** argv)
{
	setlocale(LC_ALL, "Japanese");

	wchar_t tmpdir[MAX_PATH];
	size_t ret;
	_wgetenv_s(&ret, tmpdir, MAX_PATH, L"SGREP_TMPDIR");
	if (ret == 0) {
		_wgetenv_s(&ret, tmpdir, MAX_PATH, L"TEMP");
	}

	Sakura s{ tmpdir, L"*.cpp;*.h" };

	
	int opt = 0;
	while ((opt = getopt(argc, argv, L"f:hinRtv")) != -1) {

		switch (opt) {
		case L'f':
			s.setFiles(optarg);
			break;
		case L'h':
			{
				int v = s.getOpt('h') + 1;
				if (v >= 3) {
					fprintf(stderr, "error\n");
					exit(1);
				}
				s.setOpt('h', v);
			}
			break;
		case L'i':
			s.setOpt('i', 1);
			break;
		case L'n':
			s.setOpt('n', 1);
			break;
		case L'R':
			s.setOpt('R', 1);
			break;
		case L't':
			{
				s.setOpt('t', s.getOpt('t') + 1);
			}
			break;
		case L'v':
			s.setOpt('v', 1);
			break;
		default:
			break;
		}
	}

	if (optind < argc) {
		s.setWord(argv[optind]);
		optind++;
	}
	if (optind < argc) {
		s.setFolder(argv[optind]);
		optind++;
	}

	//s.setTmpdir(L"C:\\Mail\\tmp\\");
	s.mkFileName();
	s.mkCommand();
	s.mkProcess();
	s.printResult();
	s.delTmpFile();

	return 0;
}
