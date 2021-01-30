// colors based on gruvbox vim dark theme
// https://github.com/morhetz/gruvbox



#ifndef TERMINAL_COLORS
#define TERMINAL_COLORS

#include <sstream>

namespace color {
	constexpr const char* gray =		"\033[38;2;92;83;74m";
	constexpr const char* red =			"\033[38;2;204;36;29m";
	constexpr const char* lred =		"\033[38;2;251;73;52m";
	constexpr const char* green =		"\033[38;2;152;151;26m";
	constexpr const char* lgreen =		"\033[38;2;184;187;38m";
	constexpr const char* yellow =		"\033[38;2;215;153;33m";
	constexpr const char* lyellow =		"\033[38;2;250;189;47m";
	constexpr const char* blue =		"\033[38;2;69;133;136m";
	constexpr const char* lblue =		"\033[38;2;131;165;152m";
	constexpr const char* purple =		"\033[38;2;177;98;134m";
	constexpr const char* lpurple =		"\033[38;2;211;134;155m";
	constexpr const char* aqua =		"\033[38;2;104;157;106m";
	constexpr const char* laqua =		"\033[38;2;142;192;124m";
	constexpr const char* lgray =		"\033[38;2;160;153;132m";
	constexpr const char* orange =		"\033[38;2;214;93;14m";
	constexpr const char* lorange =		"\033[38;2;254;128;25m";
	constexpr const char* fg =			"\033[38;2;237;219;178m";
	constexpr const char* white =		"\033[38;2;220;220;220m";

	constexpr const char* error = red;
	constexpr const char* warning = lred;
	constexpr const char* file = purple;
	constexpr const char* param = aqua;
	constexpr const char* std = white;
	constexpr const char* process = blue;
	constexpr const char* success = green;
	constexpr const char* value = orange;

}

#endif