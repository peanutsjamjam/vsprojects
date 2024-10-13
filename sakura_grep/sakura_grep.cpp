// sakura_grep.cpp

#include <cstdio>
#include <clocale>
#include <ctime>
#include <Windows.h>
#include "getopt.h"
#include "Sakura.h"

int wmain(int argc, WCHAR** argv)
{
	setlocale(LC_ALL, "Japanese");

	Sakura s{ L"*.cpp;*.h" };

	int opt = 0;
	while ((opt = getopt(argc, argv, L"f:hinRt")) != -1) {

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
		default:
			printf("other\n");
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

	s.mkFileName();
	s.mkCommand();
	s.mkProcess();
	s.printResult();
	s.delTmpFile();

	return 0;
}
