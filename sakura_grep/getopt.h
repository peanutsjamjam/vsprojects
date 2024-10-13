/*
 * getopt - POSIX like getopt for Windows console Application
 *
 * takamin Ç≥ÇÒÇÃ win-c ÇÃ getopt.h Ç… peanutsjamjam Ç™ïœçXÇâ¡Ç¶ÇΩÇ‡ÇÃÅB
 *
 * https://github.com/takamin/win-c
 */

/*
 * getopt - POSIX like getopt for Windows console Application
 *
 * win-c - Windows Console Library
 * Copyright (c) 2015 Koji Takami
 * Released under the MIT license
 * https://github.com/takamin/win-c/blob/master/LICENSE
 */
#ifndef _GETOPT_H_
#define _GETOPT_H_

#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int getopt(int argc, WCHAR* const argv[], const WCHAR* optstring);
extern WCHAR *optarg;
extern int optind, opterr, optopt;

#define no_argument 0
#define required_argument 1
#define optional_argument 2

struct option {
	const WCHAR *name;
	int has_arg;
	int* flag;
	int val;
};

int getopt_long(int argc, WCHAR* const argv[], const WCHAR* optstring, const struct option* longopts, int* longindex);

/****************************************************************************
    int getopt_long_only(int argc, WCHAR* const argv[],
            const WCHAR* optstring,
            const struct option* longopts, int* longindex);
****************************************************************************/
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _GETOPT_H_
