#ifndef KEYSTROKEDYNAMICSOBJ_H
#define KEYSTROKEDYNAMICSOBJ_H

#include <bits/stdc++.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

#include "KDDataStore.h"
#include "LineProcess.h"

class KeystrokeDynamics {
 private:
  std::string testedWords[11];

 public:
  static const std::string RAW_DATA_FP;
  static const std::string MEASURED_DATA_FP;

  KeystrokeDynamics() {
    std::string temp[] = {"select", "update",   "from", "where",
                          "delete", "create",   "use",  "describe",
                          "table",  "database", "and"};
    for (long unsigned int i = 0; i < temp->length(); i++) {
      testedWords[(int)i] = temp[i];
    }
  }

  std::string getTestedWords(int index) { return testedWords[index]; }

  int getNumTestedWords() { return 11; }

  // Clear/Reset data store file
  // 0 - Full Reset Success
  // 1 - Delete Success
  // 2 - Fail
  int resetFileDataStore() {
    try {
      remove(RAW_DATA_FP.c_str());
      try {
        FILE *fp = fopen(RAW_DATA_FP.c_str(), "w");
        fclose(fp);
        return 0;
      } catch (const std::exception &e) {
        return 1;
      }
    } catch (const std::exception &e) {
      return 2;
    }
  }

  // Clear/Reset data store file
  // 0 - Full Reset Success
  // 1 - Delete Success
  // 2 - Fail
  int resetFileCompileStore() {
    try {
      remove(MEASURED_DATA_FP.c_str());
      try {
        FILE *fp = fopen(MEASURED_DATA_FP.c_str(), "w");
        fclose(fp);
        return 0;
      } catch (const std::exception &e) {
        return 1;
      }
    } catch (const std::exception &e) {
      return 2;
    }
  }

  int checkWordInList(std::string word) {
    std::string testWord = wordToLowercase(word);

    for (long unsigned int i = 0; i < testedWords->length(); i++) {
      if (testedWords[(int)i] == testWord) {
        return i;
      }
    }
    return -1;
  }

  std::string wordToLowercase(std::string word) {
    char *wordList = new char[word.length()];
    for (long unsigned int i = 0; i < word.length(); i++) {
      char temp = word[i];
      if ((temp >= 65) && (temp <= 92)) {
        temp = temp + 32;
      }
      wordList[i] = temp;
    }
    std::string testWord = wordList;
    return testWord;
  }
};

const std::string KeystrokeDynamics::RAW_DATA_FP = "/tmp/rawKeyData.txt";
const std::string KeystrokeDynamics::MEASURED_DATA_FP =
    "/tmp/compiledKeyData.txt";

#endif