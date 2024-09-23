#ifndef LINEPROCESS_H
#define LINEPROCESS_H

#include <string>

class LineProcessCompiled
{
public:
    std::string letterQuery;
    char letter = '\0';
    long downTime;
    long upTime;
    long timeHold;
    bool isLetter = false;

    LineProcessCompiled(std::string input, std::string delimeter)
    {
        // key - down time - up time - hold time
        size_t d1 = input.find(delimeter);
        this->letterQuery = input.substr(0, d1);
        input.erase(0, d1 + delimeter.length());

        size_t d2 = input.find(delimeter);
        this->downTime = stol(input.substr(0, d2));
        input.erase(0, d2 + delimeter.length());

        size_t d3 = input.find(delimeter);
        this->upTime = stol(input.substr(0, d3));
        input.erase(0, d3 + delimeter.length());

        this->timeHold = stol(input);

        if (this->letterQuery.length() == 1)
        {
            if (((int)(char)this->letterQuery[0] >= 99) && ((int)(char)this->letterQuery[0] <= 122))
            {
                this->letter = letterQuery[0];
                this->isLetter = true;
            }
        }
    }
};

class LineProcessRaw
{
public:
    std::string keystroke;
    int seconds;
    int microseconds;
    char action;
    long longTime;
    bool isLetter = false;
    char letter = '\0';

    LineProcessRaw(std::string input, std::string delimeter)
    {
        // get seconds
        size_t index = input.find(delimeter);
        this->seconds = stoi(input.substr(0, index));
        input.erase(0, index + delimeter.length());

        // get nanoseconds
        index = input.find(delimeter);
        this->microseconds = stoi(input.substr(0, index));
        input.erase(0, index + delimeter.length());

        // get letter
        index = input.find(delimeter);
        this->keystroke = input.substr(0, index);
        input.erase(0, index + delimeter.length());

        // get up/down
        this->action = input[0];

        this->longTime = ((long)(this->seconds) * (long)1000000) + (long)(this->microseconds);

        if (this->keystroke.length() == 1)
        {
            if (((int)(this->keystroke[0]) >= 99) && ((int)(this->keystroke[0]) <= 122))
            {
                this->isLetter = true;
            }
        }

        if (this->isLetter)
        {
            this->letter = this->keystroke[0];
        }
    }
};

#endif