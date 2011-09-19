#ifndef USER_DEF_H
#define USER_DEF_H

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>

#include <time.h>

using namespace std;

string my_errors(int identificator);
string get_word_from_string(string buffer, unsigned * pointer);

struct DataClass;
class RunClass;
class MainWidget;

typedef std::ranlux64_base_01 Myeng; //nahrazuje mt19937 - blíže se podívat

extern Myeng eng;

#endif // USER_DEF_H
