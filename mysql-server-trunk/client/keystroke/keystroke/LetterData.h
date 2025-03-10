#ifndef LETTERDATA_H
#define LETTERDATA_H

#include <sstream>

class LetterData {
 public:
  bool useLetter;
  int countHold;
  long DUSummation;
  bool useSecondLetter[26];
  int countSecond[26];
  long DDSummation[26];
  long UDSummation[26];

  std::string toString() {
    if (!useLetter) {
      return "0;\n";
    }
    std::ostringstream newStringMaker;

    newStringMaker << "1," << countHold << "," << DUSummation << ";";
    for (int i = 0; i < 26; i++) {
      if (this->useSecondLetter[i]) {
        newStringMaker << "1," << countSecond[i] << "," << DDSummation[i] << ","
                       << UDSummation[i] << ";";
      } else {
        newStringMaker << "0;";
      }
    }
    newStringMaker << "\n";

    return newStringMaker.str();
  }

  LetterData(std::string fromJSON = "") {
    this->useLetter = false;
    this->countHold = 0;
    this->DUSummation = 0;
    for (int i = 0; i < 26; i++) {
      this->useSecondLetter[i] = false;  // number of trues total (w/ useLetter)
                                         // = number of items being checked
      this->countSecond[i] =
          0;  // if true and 0, -1 to usage (ex. 9 used/10 wanted)
      this->DDSummation[i] = 0;
      this->UDSummation[i] = 0;
    }

    if (fromJSON == "") {
      return;
    }

    if (fromJSON[0] == '0') {
      return;
    }

    // Base Letter Data
    this->useLetter = true;
    fromJSON.erase(0, fromJSON.find(",") + 1);
    this->countHold = stoi(fromJSON.substr(0, fromJSON.find(",")));
    fromJSON.erase(0, fromJSON.find(",") + 1);
    this->DUSummation = stol(fromJSON.substr(0, fromJSON.find(";")));
    fromJSON.erase(0, fromJSON.find(";") + 1);

    // Individual letter data
    for (int i = 0; i < 26; i++) {
      int id = fromJSON.find(";");
      std::string subset = fromJSON.substr(0, id + 1);
      if (subset.length() > 2) {
        this->useSecondLetter[i] = true;
        subset.erase(0, subset.find(",") + 1);
        this->countSecond[i] = stoi(subset.substr(0, subset.find(",")));
        subset.erase(0, subset.find(",") + 1);
        this->DDSummation[i] = stol(subset.substr(0, subset.find(",")));
        subset.erase(0, subset.find(",") + 1);
        this->UDSummation[i] = stol(subset.substr(0, subset.find(";")));
      }
      fromJSON.erase(0, id + 1);
    }

    // return;
  }

  void addLetterData(LetterData &newData) {
    this->useLetter = this->useLetter || newData.useLetter;
    // std::cout << "Original: " << this->countHold
    //           << " New: " << newData.countHold << "\n";
    this->countHold += abs(newData.countHold);
    this->DUSummation += abs(newData.DUSummation);

    if (this->DUSummation < 0) {
      this->DUSummation = abs(newData.DUSummation);
      this->countHold = abs(newData.countHold);
    }

    for (int i = 0; i < 26; i++) {
      this->useSecondLetter[i] =
          this->useSecondLetter[i] || newData.useSecondLetter[i];
      this->countSecond[i] += abs(newData.countSecond[i]);
      this->DDSummation[i] += abs(newData.DDSummation[i]);
      this->UDSummation[i] += abs(newData.UDSummation[i]);

      if ((this->DDSummation[i] < 0) || (this->UDSummation[i] < 0)) {
        this->countSecond[i] = abs(newData.countSecond[i]);
        this->DDSummation[i] = abs(newData.DDSummation[i]);
        this->UDSummation[i] = abs(newData.UDSummation[i]);
      }
    }
  }
};

#endif