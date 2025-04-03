#ifndef KDUSERREGDATA_H
#define KDUSERREGDATA_H

#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "KDDataStore.h"
#include "KDRegStore.h"

class KDUserRegData {
 private:
  std::string name;
  static const int NUM_WORDS = 14;
  static const int NUM_REGISTRATION_DATA = 10;
  std::vector<std::string> testedWords;
  // std::string testedWords[NUM_WORDS];
  std::map<std::string, std::vector<int>> dataRegisteredPerWord;

  std::string SAVE_LOCATION = "/usr/local/src/KDData/users";
  std::string ARCHIVE_DATA = "/usr/local/src/KDData/oldData";

 public:
  KDUserRegData(std::string name, bool newUser) {
    this->name = name;
    std::string temp[] = {"select", "update",   "from", "where",
                          "delete", "create",   "use",  "describe",
                          "table",  "database", "and",  "databases",
                          "user",   "users"};

    for (long unsigned int i = 0; i < 14; i++) {
      this->testedWords.push_back(temp[i]);
      // testedWords[i] = temp[i];
      std::vector<int> newDataSrc;
      for (int j = 0; j < NUM_REGISTRATION_DATA; j++) {
        newDataSrc.push_back(0);  // 0 = not full, 1 = full
      }
      dataRegisteredPerWord.insert({temp[i], newDataSrc});
    }

    if (!newUser) {
      std::ifstream input(this->SAVE_LOCATION + "/" + this->name +
                          "/totalData.txt");
      std::string line;
      int counter = 0;
      while (getline(input, line)) {
        if (line.length() == 0) {
          break;
        }
        if (counter == 0) {
          this->name = name;
          counter++;
        } else {
          for (int i = 0; i < NUM_REGISTRATION_DATA; i++) {
            this->dataRegisteredPerWord[testedWords.at(counter - 1)].at(i) =
                stoi(line.substr(i, 1));
          }
          counter++;
        }
      }
    }
  }

  KDRegStore importRegistrationStored() {
    std::string filename =
        SAVE_LOCATION + "/" + this->name + "/summaryData.txt";
    std::ifstream regData(filename);

    std::string res = "";
    std::string line;
    while (getline(regData, line)) {
      res += line;
    }
    regData.close();
    // std::cout << "Start of file: \t" << res << "\n";
    return KDRegStore(res);
  }

  void updateRegTable(KDRegStore *data, int indexUpdated) {
    // get data for update
    KDDataStore current = (*data).getItem(indexUpdated);
    // std::cout << "-----------------------\n";
    // std::cout << current.toString() << "\n";
    // std::cout << "-----------------------\n";
    // std::cout << this->toString() << "\n";
    // std::cout << "-----------------------\n";
    // for each word updated
    for (int i = 0; i < 14; i++) {
      std::string currentWord = this->testedWords.at(i);
      int updatedScore = 1;
      char prevChar = '\0';
      for (int j = 0; j < (int)currentWord.length(); j++) {
        // test current single letter
        char currentLetter = (char)currentWord.substr(j, 1).c_str()[0];
        if (current.getKDData(currentLetter).countHold == 0) {
          updatedScore = 0;
          break;
        }

        // test current letter as second
        if (prevChar != '\0') {
          if (current.getKDData(prevChar)
                  .countSecond[((int)currentLetter) - 97] == 0) {
            updatedScore = 0;
            break;
          }
        }
        prevChar = currentLetter;
      }
      if (this->dataRegisteredPerWord[this->testedWords[i]].at(indexUpdated) ==
          0) {
        this->dataRegisteredPerWord[this->testedWords[i]].at(indexUpdated) =
            updatedScore;
      }
    }
    this->saveUserData(*data);
  }

  int getLastEmptyForWord(int index) {
    std::string word = this->testedWords.at(index);
    for (int i = 0; i < NUM_REGISTRATION_DATA; i++) {
      if (this->dataRegisteredPerWord[word].at(i) == 0) {
        return i;
      }
    }
    return -1;
  }

  bool isWordFull(int i) {
    // std::string word = this->testedWords.at(i);
    return this->getLastEmptyForWord(i) == -1;
  }

  std::string toString() {
    std::ostringstream stringBuilder;
    stringBuilder << this->name << "\n";
    for (int i = 0; i < NUM_WORDS; i++) {
      for (int j = 0; j < NUM_REGISTRATION_DATA; j++) {
        stringBuilder << this->dataRegisteredPerWord[testedWords.at(i)].at(j);
      }
      stringBuilder << "\n";
    }
    return stringBuilder.str();
  }

  void initSaveLocation() {
    system(("if test -d " + SAVE_LOCATION +
            "; then echo test > /dev/null; else mkdir -p " + SAVE_LOCATION +
            "; fi")
               .c_str());
    system(("if test -d " + ARCHIVE_DATA +
            "; then echo test > /dev/null; else mkdir -p " + ARCHIVE_DATA +
            "; fi")
               .c_str());
  }

  void testCurrentUserDEBUG() {
    std::string dir = SAVE_LOCATION + "/" + this->name;
    struct stat sb;

    if (!(stat(dir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))) {
      createUser();
    }
  }

  void createUser() {
    // create new user, no passkey, assume established
    system(("mkdir -p " + SAVE_LOCATION + "/" + this->name).c_str());
    std::ofstream userSaveData(SAVE_LOCATION + "/" + this->name +
                               "/totalData.txt");
    userSaveData << this->toString();
    userSaveData.close();
    std::ofstream userSaveDataAll(SAVE_LOCATION + "/" + this->name +
                                  "/summaryData.txt");
    userSaveDataAll << "";
    userSaveDataAll.close();
  }

  void saveKDDataArchieve() {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    auto ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
    std::ostringstream numToStr;
    numToStr << ms;

    std::string newFileName =
        ARCHIVE_DATA + "/" + this->name + "_" + numToStr.str();
    system(("cp /usr/local/src/rawKeyData.txt " + newFileName + "_RAW.txt")
               .c_str());
    system(("cp /usr/local/src/compiledKeyData.txt " + newFileName +
            "_COMPILED.txt")
               .c_str());
  }

  void saveUserData(KDRegStore &userData) {
    std::ofstream userRegData(SAVE_LOCATION + "/" + this->name +
                              "/summaryData.txt");
    userRegData << userData.toString();
    userRegData.close();

    std::ofstream userSaveData(SAVE_LOCATION + "/" + this->name +
                               "/totalData.txt");
    userSaveData << this->toString();
    userSaveData.close();
  }

  static bool testUserExists(std::string name) {
    struct stat sb;
    std::string saveLoc = "/usr/local/src/KDData/users";
    if (stat((saveLoc + "/" + name).c_str(), &sb) == 0) {
      return true;
    }
    return false;
  }
};

#endif