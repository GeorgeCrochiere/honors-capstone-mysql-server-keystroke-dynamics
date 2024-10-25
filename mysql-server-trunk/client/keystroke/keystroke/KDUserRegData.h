#ifndef KDUSERREGDATA_H
#define KDUSERREGDATA_H

#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

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
  std::string testedWords[NUM_WORDS];
  std::map<std::string, std::vector<int>> dataRegisteredPerWord;

  std::string SAVE_LOCATION = "/tmp/KDData/users";
  std::string ARCHIVE_DATA = "/tmp/KDData/oldData";

 public:
  KDUserRegData(std::string name, std::string inputFile = "") {
    this->name = name;
    std::string temp[] = {"select", "update",   "from", "where",
                          "delete", "create",   "use",  "describe",
                          "table",  "database", "and",  "databases",
                          "user",   "users"};

    for (long unsigned int i = 0; i < 14; i++) {
      testedWords[(int)i] = temp[i];
      std::vector<int> newDataSrc;
      for (int j = 0; j < NUM_REGISTRATION_DATA; j++) {
        newDataSrc.push_back(0);  // 0 = not full, 1 = full
      }
      dataRegisteredPerWord.insert({temp[i], newDataSrc});
    }

    if (inputFile != "") {
      std::ifstream input(inputFile);
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
            this->dataRegisteredPerWord[testedWords[counter - 1]].at(i) =
                stoi(line.substr(i, 1));
          }
          counter++;
        }
      }
    }
  }

  int getLastEmptyForWord(std::string word) {
    for (int i = 0; i < NUM_REGISTRATION_DATA; i++) {
      if (this->dataRegisteredPerWord[word].at(i) == 0) {
        return i;
      }
    }
    return -1;
  }

  bool isWordFull(std::string word) {
    return this->getLastEmptyForWord(word) == -1;
  }

  std::string toString() {
    std::ostringstream stringBuilder;
    stringBuilder << this->name << "\n";
    for (int i = 0; i < NUM_WORDS; i++) {
      for (int j = 0; j < NUM_REGISTRATION_DATA; j++) {
        stringBuilder << this->dataRegisteredPerWord[testedWords[i]].at(j);
      }
      stringBuilder << "\n";
    }
    return stringBuilder.str();
  }

  void initSaveLocation() {
    system(("mkdir -p " + SAVE_LOCATION).c_str());
    system(("mkdir -p " + ARCHIVE_DATA).c_str());
  }

  void createUser() {
    // create new user, no passkey, assume established
    system(("mkdir -p " + SAVE_LOCATION + "/" + this->name).c_str());
    std::ofstream userSaveData(SAVE_LOCATION + "/totalData.txt");
    userSaveData << this->toString();
    userSaveData.close();
  }

  void saveKDDataArchieve() {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    auto ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
    std::ostringstream numToStr;
    numToStr << ms;

    std::string newFileName =
        ARCHIVE_DATA + "/" + this->name + "_" + numToStr.str();
    system(("mv /tmp/rawKeyData.txt " + newFileName + "_RAW.txt").c_str());
    system(("mv /tmp/compiledKeyData.txt " + newFileName + "_COMPILED.txt")
               .c_str());
  }

  void saveUserData(KDRegStore &userData) {
    std::ofstream userRegData(SAVE_LOCATION + "/" + this->name +
                              "/totalData.txt");
    userRegData << userData.toString();
    userRegData.close();

    std::ofstream userSaveData(SAVE_LOCATION + "/" + this->name +
                               "/summaryData.txt");
    userSaveData << this->toString();
    userSaveData.close();
  }

  static bool testUserExists(std::string name) {
    struct stat sb;
    std::string saveLoc = "/tmp/KDData/users";
    if (stat((saveLoc + "/" + name).c_str(), &sb) == 0) {
      return true;
    }
    return false;
  }
};

#endif