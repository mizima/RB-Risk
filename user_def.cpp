#include "user_def.h"

Myeng eng;

string my_errors(int identificator)
{
        switch (identificator)
        {
                case 1 :		return "error - wrong path to input file";
                case 2 :		return "error - too few resources";
                case 3 :		return "error - wrong dependencies";
        }
        return "error - unknown error";
}

string get_word_from_string(string buffer, unsigned * pointer)
{
        string word;
        while(*pointer<buffer.size() && !(buffer[*pointer]==char(',')))
        {
                word+=buffer[*pointer];
                (*pointer)++;
        }
        (*pointer)++;
        return word;
}

