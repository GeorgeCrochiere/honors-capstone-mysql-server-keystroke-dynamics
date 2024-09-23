#ifndef KDRUN_H
#define KDRUN_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "../keystroke/KeystrokeDynamicsObj.h"
#include "../keystroke/KDRegStore.h"
#include "../keystroke/KDDataStore.h"
#include "../logRawData/logKeystroke.h"

// returns index of vector of found value
int inVector(std::vector<std::string> list, std::string searchChar)
{
    if (list.size() == 0)
        return -1;
    for (int i = 0; i < list.size(); i++)
    {
        if (list.at(i) == searchChar)
        {
            return i;
        }
    }
    return -1;
};

// Takes in string query, and returns vector containing the words that are utilized by KD
KDDataStore getKDDataNeeded(std::string inputQuery)
{
    KeystrokeDynamics kd = KeystrokeDynamics();
    KDDataStore kdds = KDDataStore();

    size_t index = 0;
    std::string delimSpace = " ";
    int useElem[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // 11 elemets, 1 per word

    while (index < std::string::npos)
    {
        index = inputQuery.find(delimSpace);
        int useWordIndex = kd.checkWordInList(inputQuery.substr(0, index));
        if (useWordIndex != -1)
        {
            useElem[useWordIndex] = 1;
        }
        inputQuery.erase(0, index + delimSpace.length());
    }

    for (int i = 0; i < 11; i++)
    {
        if (useElem[i] == 1)
        {
            std::string word = kd.getTestedWords(i);
            for (int j = 0; j < word.length(); j++)
            {
                if (j != word.length() - 1)
                {
                    kdds.addEntryUseChar(word[j], true, word[j + 1]);
                }
                else
                {
                    kdds.addEntryUseChar(word[j], false, NULL);
                }
            }
        }
    }

    return kdds;
}

// Takes in file that tracked raw KD Data, and writes to a file to have clearer data
void processDataRawToCompiled(std::string inputLine)
{
    try
    {
        // Standard start components
        std::string row;
        std::vector<std::string> lettersTemp;
        std::vector<char> upDownTemp;
        std::vector<unsigned long> timeTemp;
        std::vector<std::string>::iterator vecIndex;

        const std::string delimeter = " - ";

        // Open files
        std::ifstream inputFile;
        std::ofstream outputFile;

        inputFile.open(KeystrokeDynamics::RAW_DATA_FP);
        outputFile.open(KeystrokeDynamics::MEASURED_DATA_FP);

        while (getline(inputFile, row))
        {
            if (row.length() < 3)
            {
                continue; // Safety Measure
            }
            else
            {
                // Process line
                LineProcessRaw lpr = LineProcessRaw(row, delimeter);

                // Account for the press is a new downkey (add to storage)
                if (lpr.action == 'D')
                {
                    // new
                    lettersTemp.push_back(lpr.keystroke);
                    upDownTemp.push_back(lpr.action);
                    timeTemp.push_back(lpr.longTime);
                }
                else
                {
                    // upstroke, full keypress found
                    int index = inVector(lettersTemp, lpr.keystroke);
                    // -1, ignore, shouldn't happen but safety
                    if (index != -1)
                    {
                        outputFile << lpr.keystroke << " - " << (timeTemp.at(index)) << " - " << lpr.longTime << " - " << ((lpr.longTime) - (timeTemp.at(index))) << std::endl;
                        lettersTemp.erase(lettersTemp.begin() + index);
                        timeTemp.erase(timeTemp.begin() + index);
                        upDownTemp.erase(upDownTemp.begin() + index);
                    }
                }
            }
        }

        // data parsed, clear any remaining components if necessary, and close files
        outputFile.flush();
        inputFile.close();
        outputFile.close();

        lettersTemp.clear();
        upDownTemp.clear();
        timeTemp.clear();

        return;
    }
    catch (const std::exception &e)
    {
        return;
    }
}

KDDataStore processDataCompiledToObj(KDDataStore neededData, bool registeringNew)
{
    // Start reading file that contains compiled data
    std::ifstream inputFile;
    std::ofstream outputFile;
    inputFile.open(KeystrokeDynamics::MEASURED_DATA_FP);

    std::string line;
    char lastChar = '\0';
    long dTime;
    long uTime;

    std::string delimeter = " - ";
    std::string resetButtons[] = {"(ENTER-1)", "(ENTER-2)", "(ESCAPE)", "(DELETE)", "(TAB)", "(Space)"};

    while (getline(inputFile, line))
    {
        if (line.length() > 0)
        {
            // Get line from compiled file (key, downTime, upTime, etc.)
            LineProcessCompiled lpc = LineProcessCompiled(line, delimeter);
            int addUse = neededData.addEntryHold(lpc.letter, lpc.timeHold);
            if (lastChar != '\0' && addUse != -1)
            {
                neededData.addEntryBetween(lastChar, lpc.letter, (lpc.downTime - dTime), (lpc.downTime - uTime));
            }

            lastChar = lpc.letter;
            dTime = lpc.downTime;
            uTime = lpc.upTime;

            if (!lpc.isLetter)
            {
                if (registeringNew)
                {
                    neededData.isNull = true;
                    return neededData;
                }
                for (int k = 0; k < resetButtons->length(); k++)
                {
                    if (line.substr(0, line.find(" - ")) == resetButtons[k])
                    {
                        lastChar = '\0';
                        dTime = 0;
                        uTime = 0;
                    }
                }
            }
        }

        // Return obj
        return neededData;
    }
}

std::string dataObjToString(KDDataStore data)
{
    return data.toString();
}

KDRegStore newUserRegistration()
{
    // Reset files to allow new data to be gathered
    KeystrokeDynamics kd = KeystrokeDynamics();
    kd.resetFileDataStore();
    kd.resetFileCompileStore();

    KDRegStore newUserRegistrationData = KDRegStore();

    std::string expectedValsTemp = "";
    for (int i = 0; i < kd.getNumTestedWords(); i++)
    {
        expectedValsTemp = expectedValsTemp + kd.getTestedWords(i) + " ";
    }

    std::cout << "\nDetected new user registration. Please have the new user complete the following:\n";
    std::cout << "Please enter the following words in all lowercase without making any mistakes, and without using unneeded keystrokes.\n";
    std::cout << "This will register your keystroke dynamic biometrics. Errors will ask for a reentry.\n\n";

    std::string regEntry;

    for (int i = 0; i < 10; i++)
    {
        KDDataStore kdN = getKDDataNeeded(expectedValsTemp);

        int wordCounter = 0;
        while (wordCounter < kd.getNumTestedWords())
        {
            std::cout << kd.getTestedWords(wordCounter) << ": ";
            std::cin >> regEntry;

            // Test if entered word is accurate
            if (regEntry != kd.getTestedWords(wordCounter))
            {
                // reenter
                std::cout << "Error, try again.\n";
            }
            else
            {
                // process key data to compiled,
                processDataRawToCompiled(kd.getTestedWords(wordCounter));
                KDDataStore afterData = processDataCompiledToObj(kdN, true);
                if (kdN.isNull == true)
                {
                    std::cout << "Error, try again.\n";
                    continue;
                }
                else
                {
                    kdN = afterData;
                    wordCounter++;
                }
            }
        }

        newUserRegistrationData.addEntry(kdN);
    }

    return newUserRegistrationData;
};

#endif