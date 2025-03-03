#ifndef KDDATASTORE_H
#define KDDATASTORE_H

#include <cmath>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "LetterData.h"

class KDDataStore {
 private:
  std::map<char, LetterData> kdLetterData;

 public:
  int useWordsTEMP[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  bool isNull = false;
  KDDataStore(std::string input = "") {
    // Reset
    for (int i = 97; i <= 122; i++) {
      LetterData aLettersData = LetterData();
      kdLetterData.insert(std::pair<char, LetterData>((char)i, aLettersData));
    }
    if (input == "") {
      return;
    }

    std::string delimeter = "\n";
    for (int i = 0; i < 26; i++) {
      std::string line = input.substr(0, input.find(delimeter));
      char mapChar = line[0];
      line.erase(0, 2);
      LetterData ld = LetterData(line);
      this->kdLetterData[mapChar] = ld;
      input.erase(0, input.find(delimeter) + delimeter.length());
    }
  }

  int addEntryUseChar(char baseLetter, bool isLetterAfter, char letterAfter) {
    if (kdLetterData[baseLetter].useLetter == false) {
      kdLetterData[baseLetter].useLetter = true;
    }
    if (isLetterAfter == true) {
      kdLetterData[baseLetter].useSecondLetter[(int)letterAfter - 97] = true;
    }
    return 0;
  }

  bool testIfAfter(char baseLetter, char letterAfter) {
    return ((kdLetterData[baseLetter].useLetter == true) &&
            (kdLetterData[baseLetter].useSecondLetter[(int)letterAfter - 97] ==
             true));
  }

  int addEntryHold(char baseLetter, long DUTime) {
    if (kdLetterData[baseLetter].useLetter == true) {
      kdLetterData[baseLetter].countHold++;
      kdLetterData[baseLetter].DUSummation += std::abs(DUTime);
      return 0;
    }
    return -1;
  }

  int addEntryBetween(char baseLetter, char letterAfter, long DDTime,
                      long UDTime) {
    if (testIfAfter(baseLetter, letterAfter)) {
      kdLetterData[baseLetter].countSecond[(int)letterAfter - 97]++;
      kdLetterData[baseLetter].DDSummation[(int)letterAfter - 97] +=
          std::abs(DDTime);
      kdLetterData[baseLetter].UDSummation[(int)letterAfter - 97] +=
          std::abs(UDTime);
      return 0;
    }
    return -1;
  }

  void printAllPresent() {
    for (int i = 97; i <= 122; i++) {
      std::cout << (char)i << ": " << kdLetterData[(char)i].useLetter << " - ";
      for (int j = 0; j < 26; j++) {
        std::cout << "(" << kdLetterData[(char)i].useSecondLetter[j] << ","
                  << kdLetterData[(char)i].countSecond[j] << ")";
      }
      std::cout << "\n";
    }
  }

  // Get data
  LetterData getKDData(char indexCharacter) {
    return this->kdLetterData[indexCharacter];
  }

  std::string toString() {
    std::ostringstream stringBuilder;
    for (int i = 97; i <= 122; i++) {
      stringBuilder << (char)i << ":" << kdLetterData[(char)i].toString();
    }
    return stringBuilder.str();
  }

  double getTestData(std::string type, char firstLetter, char secondLetter) {
    if (type == "DU") {
      return ((double)(this->kdLetterData[firstLetter].DUSummation)) /
             ((double)(this->kdLetterData[firstLetter].countHold));
    } else if (type == "DD") {
      return ((double)(this->kdLetterData[firstLetter]
                           .DDSummation[((int)secondLetter) - 97])) /
             ((double)(this->kdLetterData[firstLetter]
                           .countSecond[((int)secondLetter) - 97]));
    } else if (type == "UD") {
      return ((double)(this->kdLetterData[firstLetter]
                           .UDSummation[((int)secondLetter) - 97])) /
             ((double)(this->kdLetterData[firstLetter]
                           .countSecond[((int)secondLetter) - 97]));
    } else {
      return -1;
    }
  }

  void addKDData(KDDataStore &newData) {
    for (int i = 0; i < 26; i++) {
      char currentLetter = (char)(i + 97);
      // std::cout << "Char: " << currentLetter << ": \n"
      //           << this->kdLetterData[currentLetter].toString() << "\n";
      this->kdLetterData[currentLetter].addLetterData(
          newData.kdLetterData[currentLetter]);
    }
  }

  double getRegDataCount(std::string type, char firstLetter,
                         char secondLetter) {
    if (type == "DU") {
      return ((double)(this->kdLetterData[firstLetter].countHold));
    } else if (type == "DD") {
      return ((double)(this->kdLetterData[firstLetter]
                           .countSecond[((int)secondLetter) - 97]));
    } else if (type == "UD") {
      return ((double)(this->kdLetterData[firstLetter]
                           .countSecond[((int)secondLetter) - 97]));
    } else {
      return -1;
    }
  }

  double getRegDataSum(std::string type, char firstLetter, char secondLetter) {
    if (type == "DU") {
      return ((double)(this->kdLetterData[firstLetter].DUSummation));
    } else if (type == "DD") {
      return ((double)(this->kdLetterData[firstLetter]
                           .DDSummation[((int)secondLetter) - 97]));
    } else if (type == "UD") {
      return ((double)(this->kdLetterData[firstLetter]
                           .UDSummation[((int)secondLetter) - 97]));
    } else {
      return -1;
    }
  }

  // For registration, test if word and associated features are full
  bool getWordDataFull(std::string word) {
    char prevChar = '\0';
    for (int i = 0; i < (int)word.length(); i++) {
      char currentChar = word.substr(i, 1).c_str()[0];
      // test Hold
      if (this->getRegDataCount("DU", currentChar, '\0') == 0) {
        return false;
      }

      // test after characters
      if (prevChar != '\0') {
        if (this->getRegDataCount("DD", prevChar, currentChar) == 0 ||
            this->getRegDataCount("UD", prevChar, currentChar) == 0) {
          return false;
        }
      }
      prevChar = currentChar;
    }
    return true;
  }
};

#endif