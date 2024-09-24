#ifndef KDREGSTORE_H
#define KDREGSTORE_H

#include <iostream>
#include <sstream>
#include <vector>

#include "KDDataStore.h"

class KDRegStore {
 private:
  // Data
  std::vector<KDDataStore> registationData;

  // Means
  long meanHold[26];
  long meanUD[26][26];
  long meanDD[26][26];

  // Variances
  double varianceHold[26];
  double varianceUD[26][26];
  double varianceDD[26][26];
  // As variance and covariance are based on both what keystrokes were added and
  // which ones are needed, calc

  static const int DOWN_UP = 1;
  static const int DOWN_DOWN = 2;
  static const int UP_DOWN = 3;

 public:
  KDRegStore(std::string input = "") {
    this->registationData.clear();
    for (int i = 0; i < 26; i++) {
      this->meanHold[i] = 0;
      this->varianceHold[i] = 0;
      for (int j = 0; j < 26; j++) {
        this->meanUD[i][j] = 0;
        this->meanDD[i][j] = 0;

        this->varianceUD[i][j] = 0;
        this->varianceDD[i][j] = 0;
      }
    }

    if (input.length() != 0) {
      int index = input.find(",");
      int numElements = stoi(input.substr(0, index));
      input.erase(0, 1 + input.substr(0, index).length());

      // Find closing bracket - mean Hold
      index = input.find("]") + 1;  // get comma
      setMeanDU(input.substr(0, index));
      input.erase(0, index + 1);

      // Find closing bracket - mean DD
      index = input.find("]") + 1;  // get comma
      setMeanDD(input.substr(0, index));
      input.erase(0, index + 1);

      // Find closing bracket - mean UD
      index = input.find("]") + 1;  // get comma
      setMeanUD(input.substr(0, index));
      input.erase(0, index + 1);

      // Find closing bracket - variance Hold
      index = input.find("]") + 1;  // get comma
      setVarianceDU(input.substr(0, index));
      input.erase(0, index + 1);

      // Find closing bracket - variance DD
      index = input.find("]") + 1;  // get comma
      setVarianceDD(input.substr(0, index));
      input.erase(0, index + 1);

      // Find closing bracket - variance UD
      index = input.find("]") + 1;  // get comma
      setVarianceUD(input.substr(0, index));
      input.erase(0, index + 1);

      // Add each individal component of data, starts/ends w/ {/}
      for (int i = 0; i < numElements; i++) {
        int indexStart = input.find("{");
        int indexEnd = input.find("}");
        std::string dataLine =
            input.substr(indexStart + 1, (indexEnd - indexStart) - 1);
        this->addEntry(KDDataStore(dataLine));
        input.erase(0, indexEnd + 1);
      }
    }
  }

  void addEntry(KDDataStore data) { this->registationData.push_back(data); }

  int numItems() { return this->registationData.size(); }

  KDDataStore getItem(int index) { return this->registationData.at(index); }

  // Get means
  long getMeanDU(char letter) { return this->meanHold[((int)(letter)) - 97]; }
  long getMeanDD(char letter, char letterSecond) {
    return this->meanDD[((int)(letter)) - 97][((int)(letterSecond)) - 97];
  }
  long getMeanUD(char letter, char letterSecond) {
    return this->meanUD[((int)(letter)) - 97][((int)(letterSecond)) - 97];
  }

  // Get variances
  long getVarianceDU(char letter) {
    return this->varianceHold[((int)(letter)) - 97];
  }
  long getVarianceDD(char letter, char letterSecond) {
    return this->varianceDD[((int)(letter)) - 97][((int)(letterSecond)) - 97];
  }
  long getVarianceUD(char letter, char letterSecond) {
    return this->varianceUD[((int)(letter)) - 97][((int)(letterSecond)) - 97];
  }

  void calcMeans() {
    // for each letter
    for (int i = 0; i < 26; i++) {
      long tempHold = 0;
      long tempDD[26];
      long tempUD[26];

      // For each entry - summation
      for (int j = 0; j < (int)this->registationData.size(); j++) {
        if (this->registationData.at(j).getKDData((char)(97 + i)).useLetter ==
            true) {
          tempHold +=
              (this->registationData.at(j)
                   .getKDData((char)(97 + i))
                   .DUSummation) /
              (this->registationData.at(j).getKDData((char)(97 + i)).countHold);
          // For each second letter
          for (int k = 0; k < 26; k++) {
            if (this->registationData.at(j)
                    .getKDData((char)(97 + i))
                    .useSecondLetter[k] == true) {
              tempDD[k] += (this->registationData.at(j)
                                .getKDData((char)(97 + i))
                                .DDSummation[k]) /
                           (this->registationData.at(j)
                                .getKDData((char)(97 + i))
                                .countSecond[k]);
              tempUD[k] += (this->registationData.at(j)
                                .getKDData((char)(97 + i))
                                .UDSummation[k]) /
                           (this->registationData.at(j)
                                .getKDData((char)(97 + i))
                                .countSecond[k]);
            }
          }
        }
      }

      // Division & Storage Portion
      if (tempHold != 0) {
        tempHold = tempHold / 10;
        this->meanHold[i] = tempHold;
      }
      for (int j = 0; j < 26; j++) {
        if (tempDD[j] != 0) {
          tempDD[j] = tempDD[j] / 10;
          this->meanDD[i][j] = tempDD[j];
          tempUD[j] = tempUD[j] / 10;
          this->meanUD[i][j] = tempUD[j];
        }
      }
    }
  }

  void calcVariances() {
    // for each letter
    for (int i = 0; i < 26; i++) {
      double tempHold = 0;
      double tempDD[26];
      double tempUD[26];

      // For each entry - summation
      for (int j = 0; j < (int)this->registationData.size(); j++) {
        if (this->registationData.at(j).getKDData((char)(97 + i)).useLetter ==
            true) {
          tempHold += (((this->registationData.at(j)
                             .getKDData((char)(97 + i))
                             .countHold) /
                        (this->registationData.at(j)
                             .getKDData((char)(97 + i))
                             .countHold)) -
                       (this->meanHold[i])) ^
                      2;
          // For each second letter
          for (int k = 0; k < 26; k++) {
            if (this->registationData.at(j)
                    .getKDData((char)(97 + i))
                    .useSecondLetter[k] == true) {
              tempDD[k] += (((this->registationData.at(j)
                                  .getKDData((char)(97 + i))
                                  .DDSummation[k]) /
                             (this->registationData.at(j)
                                  .getKDData((char)(97 + i))
                                  .countSecond[k])) -
                            (this->meanDD[i][j])) ^
                           2;
              tempUD[k] += (((this->registationData.at(j)
                                  .getKDData((char)(97 + i))
                                  .UDSummation[k]) /
                             (this->registationData.at(j)
                                  .getKDData((char)(97 + i))
                                  .countSecond[k])) -
                            (this->meanUD[i][j])) ^
                           2;
            }
          }
        }
      }

      // Division & Storage Portion
      if (tempHold != 0) {
        tempHold = tempHold / 10;
        this->varianceHold[i] = tempHold;
      }
      for (int j = 0; j < 26; j++) {
        if (tempDD[j] != 0) {
          tempDD[j] = tempDD[j] / 10;
          this->varianceDD[i][j] = tempDD[j];
          tempUD[j] = tempUD[j] / 10;
          this->varianceUD[i][j] = tempUD[j];
        }
      }
    }
  }

  double getCovariance(int type, char letter1, char letter1After = '\0',
                       int type2 = 0, char letter2 = '\0',
                       char letter2After = '\0') {
    if (type != 1 && letter1After == '\0') {
      return -1;
    }
    if (type2 != 1 && letter2After == '\0') {
      return -1;
    }

    double covarianceVal = 0;
    for (int i = 0; i < (int)this->registationData.size(); i++) {
      double val1 = 0;
      double val2 = 0;

      // Val1
      switch (type) {
        case KDRegStore::DOWN_UP:
          val1 =
              (((this->registationData.at(i).getKDData(letter1).DUSummation) /
                (this->registationData.at(i).getKDData(letter1).countHold)) -
               (this->meanHold[i]));
          break;
        case KDRegStore::DOWN_DOWN:
          val1 =
              (((this->registationData.at(i)
                     .getKDData(letter1)
                     .DDSummation[((int)letter1After) - 97]) /
                (this->registationData.at(i)
                     .getKDData(letter1)
                     .countSecond[i])) -
               (this->meanDD[((int)letter1) - 97][((int)letter1After) - 97]));
          break;
        case KDRegStore::UP_DOWN:
          val1 =
              (((this->registationData.at(i)
                     .getKDData(letter1)
                     .UDSummation[((int)letter1After) - 97]) /
                (this->registationData.at(i)
                     .getKDData(letter1)
                     .countSecond[i])) -
               (this->meanUD[((int)letter1) - 97][((int)letter1After) - 97]));
          break;
        default:
          val1 = 0;
          break;
      }

      // Val2
      switch (type2) {
        case KDRegStore::DOWN_UP:
          val2 =
              (((this->registationData.at(i).getKDData(letter2).DUSummation) /
                (this->registationData.at(i).getKDData(letter2).countHold)) -
               (this->meanHold[i]));
          break;
        case KDRegStore::DOWN_DOWN:
          val2 =
              (((this->registationData.at(i)
                     .getKDData(letter2)
                     .DDSummation[((int)letter2After) - 97]) /
                (this->registationData.at(i)
                     .getKDData(letter2)
                     .countSecond[i])) -
               (this->meanDD[((int)letter2) - 97][((int)letter2After) - 97]));
          break;
        case KDRegStore::UP_DOWN:
          val2 =
              (((this->registationData.at(i)
                     .getKDData(letter2)
                     .UDSummation[((int)letter2After) - 97]) /
                (this->registationData.at(i)
                     .getKDData(letter2)
                     .countSecond[i])) -
               (this->meanUD[((int)letter2) - 97][((int)letter2After) - 97]));
          break;
        default:
          val2 = 0;
          break;
      }

      covarianceVal += (val1 * val2);
    }
    covarianceVal = covarianceVal / this->registationData.size();
  }

  std::string toString() {
    // Number of entries, [Arrays (Mean/Var, DU, DD, UD)], {}, {}, {},...
    std::ostringstream stringBuilder;
    stringBuilder << this->registationData.size() << ",[";

    // Means
    for (int i = 0; i < 26; i++) {
      stringBuilder << meanHold[i] << ",";
    }
    stringBuilder << "],[";

    for (int i = 0; i < 26; i++) {
      for (int j = 0; j < 26; j++) {
        stringBuilder << meanDD[i][j] << ",";
      }
    }
    stringBuilder << "],[";

    for (int i = 0; i < 26; i++) {
      for (int j = 0; j < 26; j++) {
        stringBuilder << meanUD[i][j] << ",";
      }
    }
    stringBuilder << "],[";

    // Variances
    for (int i = 0; i < 26; i++) {
      stringBuilder << varianceHold[i];
      if (i != 25) {
        stringBuilder << ",";
      }
    }
    stringBuilder << "],[";

    for (int i = 0; i < 26; i++) {
      for (int j = 0; j < 26; j++) {
        stringBuilder << varianceDD[i][j] << ",";
      }
    }
    stringBuilder << "],[";

    for (int i = 0; i < 26; i++) {
      for (int j = 0; j < 26; j++) {
        stringBuilder << varianceUD[i][j] << ",";
      }
    }
    stringBuilder << "],";

    for (int i = 0; i < (int)this->registationData.size(); i++) {
      stringBuilder << "{" << this->registationData.at(i).toString() << "}";
      if (i < (int)this->registationData.size() - 1) {
        stringBuilder << ",";
      }
    }

    return stringBuilder.str();
  }

  // Means
  void setMeanDU(std::string input) {
    // Assume string surrounded in brackets
    input.erase(0);

    for (int i = 0; i < 26; i++) {
      int index = input.find(",");
      std::string numAsStr = input.substr(0, index);
      this->meanHold[i] = stol(numAsStr);
      input.erase(0, (numAsStr.length() + 1));
    }
  }

  void setMeanDD(std::string input) {
    // Assume string surrounded in brackets
    input.erase(0);

    for (int i = 0; i < 26; i++) {
      for (int j = 0; j < 26; j++) {
        int index = input.find(",");
        std::string numAsStr = input.substr(0, index);
        this->meanDD[i][j] = stol(numAsStr);
        input.erase(0, (numAsStr.length() + 1));
      }
    }
  }

  void setMeanUD(std::string input) {
    // Assume string surrounded in brackets
    input.erase(0);

    for (int i = 0; i < 26; i++) {
      for (int j = 0; j < 26; j++) {
        int index = input.find(",");
        std::string numAsStr = input.substr(0, index);
        this->meanUD[i][j] = stol(numAsStr);
        input.erase(0, (numAsStr.length() + 1));
      }
    }
  }

  // Variances
  void setVarianceDU(std::string input) {
    // Assume string surrounded in brackets
    input.erase(0);

    for (int i = 0; i < 26; i++) {
      int index = input.find(",");
      std::string numAsStr = input.substr(0, index);
      this->varianceHold[i] = stod(numAsStr);
      input.erase(0, (numAsStr.length() + 1));
    }
  }

  void setVarianceDD(std::string input) {
    // Assume string surrounded in brackets
    input.erase(0);

    for (int i = 0; i < 26; i++) {
      for (int j = 0; j < 26; j++) {
        int index = input.find(",");
        std::string numAsStr = input.substr(0, index);
        this->varianceDD[i][j] = stod(numAsStr);
        input.erase(0, (numAsStr.length() + 1));
      }
    }
  }

  void setVarianceUD(std::string input) {
    // Assume string surrounded in brackets
    input.erase(0);

    for (int i = 0; i < 26; i++) {
      for (int j = 0; j < 26; j++) {
        int index = input.find(",");
        std::string numAsStr = input.substr(0, index);
        this->varianceUD[i][j] = stod(numAsStr);
        input.erase(0, (numAsStr.length() + 1));
      }
    }
  }

  bool testUserEntry(KDDataStore testData, double compareVal) {
    std::vector<char> firstLetter;
    std::vector<char> secondLetter;
    std::vector<std::string> type;

    int totalFeatures = 0;
    int capturedFeatures = 0;

    for (int i = 97; i <= 122; i++) {
      char letterBase = (char)i;
      if (testData.getKDData(letterBase).useLetter == true) {
        totalFeatures++;
        capturedFeatures++;
        firstLetter.push_back(letterBase);
        secondLetter.push_back('\0');
        type.push_back("DU");

        for (int j = 0; j < 26; j++) {
          if (testData.getKDData(letterBase).useSecondLetter[j] == true) {
            totalFeatures += 2;
            if (testData.getKDData(letterBase).countSecond[j] != 0) {
              capturedFeatures += 2;
              // DD
              firstLetter.push_back(letterBase);
              secondLetter.push_back((char)(97 + j));
              type.push_back("DD");

              // UD
              firstLetter.push_back(letterBase);
              secondLetter.push_back((char)(97 + j));
              type.push_back("UD");
            }
          }
        }
      }
    }
    if ((((double)capturedFeatures) / ((double)totalFeatures)) < 0.75) {
      return false;
    }

    // Predefine needed vectors/matrix
    double *leftVector = new double[capturedFeatures];
    double *rightVector = new double[capturedFeatures];

    double **covarianceMatrix = new double *[capturedFeatures];
    for (int i = 0; i < capturedFeatures; i++) {
      covarianceMatrix[i] = new double[capturedFeatures * 2];
      for (int j = 0; j < capturedFeatures * 2; j++) {
        covarianceMatrix[i][j] = 0;
      }
    }

    // Add elements to vectors/matrix
    // Vectors, mean - testValue
    for (int i = 0; i < capturedFeatures; i++) {
      double meanVal =
          getMean(type.at(i), firstLetter.at(i), secondLetter.at(i));
      double testVal = testData.getTestData(type.at(i), firstLetter.at(i),
                                            secondLetter.at(i));
      leftVector[i] = (meanVal - testVal);
      rightVector[i] = (meanVal - testVal);
      covarianceMatrix[i][i] =
          getVariance(type.at(i), firstLetter.at(i), secondLetter.at(i));
      covarianceMatrix[i][i + capturedFeatures] = 1;
    }

    for (int i = 0; i < capturedFeatures; i++) {
      for (int j = 0; i < capturedFeatures; j++) {
        if (i != j) {
          int t1;
          int t2;

          if (type.at(i) == "DU") {
            t1 = KDRegStore::DOWN_UP;
          } else if (type.at(i) == "DD") {
            t1 = KDRegStore::DOWN_DOWN;
          } else {
            t1 = KDRegStore::UP_DOWN;
          }

          if (type.at(j) == "DU") {
            t2 = KDRegStore::DOWN_UP;
          } else if (type.at(j) == "DD") {
            t2 = KDRegStore::DOWN_DOWN;
          } else {
            t2 = KDRegStore::UP_DOWN;
          }
          covarianceMatrix[i][j] =
              getCovariance(t1, firstLetter.at(i), secondLetter.at(i), t2,
                            firstLetter.at(j), secondLetter.at(j));
        }
      }
    }

    // Have covariance, get inverse
    for (int i = 0; i < capturedFeatures; i++) {
      // divide all elements in the row by the first column, get 1 as val
      for (int j = 0; j < (capturedFeatures * 2); j++) {
        covarianceMatrix[i][j] =
            covarianceMatrix[i][j] / covarianceMatrix[i][i];
      }
      for (int j = 0; j < capturedFeatures; j++) {
        if (i != j) {
          for (int k = 0; k < (capturedFeatures * 2); k++) {
            covarianceMatrix[j][k] =
                covarianceMatrix[j][k] -
                (covarianceMatrix[j][j] * covarianceMatrix[i][k]);
          }
        }
      }
    }

    double **covarianceMatrixInverse = new double *[capturedFeatures];
    for (int i = 0; i < capturedFeatures; i++) {
      covarianceMatrixInverse[i] = new double[capturedFeatures];
      for (int j = 0; j < capturedFeatures; j++) {
        covarianceMatrixInverse[i][j] =
            covarianceMatrix[i + capturedFeatures][j];
      }
    }

    // Have inverse, run calculation
    double *matSubCalc = new double[capturedFeatures];
    for (int i = 0; i < capturedFeatures; i++) {
      matSubCalc[i] = 0;
    }
    for (int i = 0; i < capturedFeatures; i++) {
      for (int j = 0; j < capturedFeatures; j++) {
        matSubCalc[i] += (covarianceMatrixInverse[j][i] * leftVector[i]);
      }
    }

    double result = 0;
    for (int i = 0; i < capturedFeatures; i++) {
      result += (matSubCalc[i] * rightVector[i]);
    }
    return compareVal > result;
  }

  double getMean(std::string type, char firstLetter, char secondLetter) {
    if (type == "DU") {
      return (double)this->meanHold[(int)(firstLetter - 97)];
    } else if (type == "DD") {
      return (double)this
          ->meanDD[(int)(firstLetter - 97)][(int)(secondLetter - 97)];
    } else if (type == "UD") {
      return (double)this
          ->meanUD[(int)(firstLetter - 97)][(int)(secondLetter - 97)];
    } else {
      return -1;
    }
  }

  double getVariance(std::string type, char firstLetter, char secondLetter) {
    if (type == "DU") {
      return this->varianceHold[(int)(firstLetter - 97)];
    } else if (type == "DD") {
      return this
          ->varianceDD[(int)(firstLetter - 97)][(int)(secondLetter - 97)];
    } else if (type == "UD") {
      return this
          ->varianceUD[(int)(firstLetter - 97)][(int)(secondLetter - 97)];
    } else {
      return -1;
    }
  }
};

#endif