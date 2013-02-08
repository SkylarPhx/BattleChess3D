#include "stdafx.h"

inline std::ostream& greyBlack(std::ostream &s)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    return s;
}

inline std::ostream& whiteGold(std::ostream &s)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 111);
    return s;
}

inline std::ostream& whiteYellow(std::ostream &s)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 231);
    return s;
}

inline std::ostream& blackGold(std::ostream &s)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 96);
    return s;
}

inline std::ostream& blackYellow(std::ostream &s)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 224);
    return s;
}

template <class _Elem, class _Traits>
std::basic_ostream<_Elem, _Traits>&
	operator<<(std::basic_ostream<_Elem, _Traits>& i, int& c)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
    return i;
}
