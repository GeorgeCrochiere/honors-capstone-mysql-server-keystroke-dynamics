#ifndef KEYSTROKEDYNAMICSOBJ_H
#define KEYSTROKEDYNAMICSOBJ_H

#include <bits/stdc++.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "KDDataStore.h"
#include "LineProcess.h"

class KeystrokeDynamics {
 private:
  std::vector<std::string> testedWords;
  // std::string testedWords[14];

 public:
  static const std::string RAW_DATA_FP;
  static const std::string MEASURED_DATA_FP;

  // change registration to be passive listening for first so many iterations of
  // specified keywords, potential increased security during process, after all
  // data is collected, then resort to fully KD

  KeystrokeDynamics() {
    std::string temp[] = {"select", "update",   "from", "where",
                          "delete", "create",   "use",  "describe",
                          "table",  "database", "and",  "databases",
                          "user",   "users"};
    for (unsigned long int i = 0; i < 14; i++) {
      this->testedWords.push_back(temp[(int)i]);
    }
  }

  std::string getTestedWords(int index) { return testedWords.at(index); }

  int getNumTestedWords() { return 14; }

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

    for (long unsigned int i = 0; i < testedWords.size(); i++) {
      // std::cout << "[" << testedWords.at(i) << ", " << testWord << "]\n";
      if (testedWords.at(i) == testWord) {
        return i;
      }
    }
    return -1;
  }

  std::string wordToLowercase(std::string word) {
    std::ostringstream stringBuilder;
    // char *wordList = new char[word.length()];
    // std::cout << "Word Length: " << word.length() << "\n";
    for (long unsigned int i = 0; i < word.length(); i++) {
      char temp = word[i];
      if ((temp >= 65) && (temp <= 90)) {
        temp = temp + 32;
      }
      stringBuilder << temp;
    }
    std::string testWord = stringBuilder.str();
    return testWord;
  }
};

const std::string KeystrokeDynamics::RAW_DATA_FP =
    "/usr/local/src/rawKeyData.txt";
const std::string KeystrokeDynamics::MEASURED_DATA_FP =
    "/usr/local/src/compiledKeyData.txt";

#endif