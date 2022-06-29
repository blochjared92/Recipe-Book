#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <iterator>
#include <utility>
#include <cmath>
#include <sstream>
using std::gcd;

class Recipe {
 private:
  int servings = 0;
  std::string food = "";  // Name of Recipe
  std::vector<std::string> value_;  // Fraction
  std::vector<std::string> unit_;  // Unit
  std::vector<std::string> name_;  // Name of Ingredient
  std::string Instructions_ = "";

 public:
  Recipe(std::string f, int val) : servings(val), food(f){};
  void AddIngredient(std::string);
  void SetInstructions(std::string);
  void ChangeServings(int);
  friend std::ostream &operator<<(std::ostream &, Recipe const &);
  std::string IngredientInOneServing(std::string);
  friend std::string RecipeName(
      Recipe const &);  // Added Friend function to get Private Members
  friend std::vector<std::string> GetName(
      Recipe const &);  // Added Friend function to get Private Members
  friend std::vector<std::string> GetValues(
      Recipe const &);  // Added Friend function to get Private Members
};

std::vector<std::string> GetValues(Recipe const &r) {
  // Get the fraction value of the ingredients
  return r.value_;
}

std::vector<std::string> GetName(Recipe const &r) {
  // Get the Name of the ingredients
  return r.name_;
}

std::string RecipeName(Recipe const &r) {
  // Get the Name of the Recipe
  return r.food;
}

std::vector<std::string> CleanBeginEndString(std::string &I) {
  // Look through the string and adds it into an istringstream, which ignores
  // the spaces
  // Then it erases everything in the string past that "word" and inputs it into
  // a vector
  // Continues doing it twice, so whatever is left over is the name of the
  // ingredient
  std::vector<std::string> clean_vector;
  std::istringstream iss(I);
  std::string component;
  for (int n = 0; n < 2; n++) {
    iss >> component;
    unsigned index = I.find(component);
    I.erase(index, component.length());
    clean_vector.push_back(component);
  }
  return clean_vector;
}

std::string &trim(std::string &s) {
  // trims (removes spaces or any uneeded info on both the left and ride side of
  // the string
  auto it = std::find_if(s.begin(), s.end(), [](char c) {
    return !std::isspace<char>(c, std::locale::classic());
  });
  s.erase(s.begin(), it);

  auto it2 = std::find_if(s.rbegin(), s.rend(), [](char c) {
    return !std::isspace<char>(c, std::locale::classic());
  });
  s.erase(it2.base(), s.end());
  return s;
}

void Seperate(std::string v, std::vector<int> &WND) {
  // Looks through the string to determine if it has an integer or not (ex:
  // 1-1/2 or 1/2)
  // Makes a vector of ints with the first index being the integer, second is
  // numerator, and third is denominator
  // If it doesn't have integer, it is always 0
  // If it is only a whole number, the second and third spot are 0
  auto it = std::find(v.begin(), v.end(), '-');
  int index = std::distance(v.begin(), std::find(v.begin(), v.end(), '-')) + 1;
  if (it != v.end()) {
    for (int i = index; i <= v.size(); i++) {
      if (v[i] == '/') {
        WND[0] = std::stoi(v.substr(0, v.size() - index));
        WND[1] = std::stoi(v.substr(index, i - index));  // Numerator
        WND[2] = std::stoi(v.substr(i + 1, v.size() - i));  // Denominator
      }
    }
  } else {
    for (int i = 0; i <= v.size(); i++) {
      if (v[i] == '/') {
        WND[1] = std::stoi(v.substr(0, i));  // Numerator
        WND[2] = std::stoi(v.substr(i + 1, v.size() - i));  // Denominator
      }
    }
  }
}

std::vector<int> FindNumAndDen(std::string v) {
  // Sets a vector of 0s to be edited based on the fraction entered
  // Ex: 1/2 entered --> vector of {0,1,2}
  // Ex: 1-1/2 entered --> vector of {1,1,2}
  // Ex: 1 entered --> vector of {1,0,0}
  std::vector<int> WND = {0, 0, 0};
  Seperate(v, WND);
  if (v.find('/') == std::string::npos) {
    WND[0] = std::stoi(v);
  }
  return WND;
}

void Simplify(std::vector<int> &WND) {
  // If given a fraction that can be simplifed, will simplify it accordingly
  // with vector of ints
  // Ex: {0,9,4} --> {2, 1, 4}
  while (WND[1] >= WND[2] && WND[1] != 0 && WND[2] != 0) {
    WND[1] = WND[1] - WND[2];
    WND[0] += 1;
  }
  int gcd_ = gcd(WND[1], WND[2]);
  if (gcd_ != 0) {
    WND[2] = WND[2] / gcd_;
    WND[1] = WND[1] / gcd_;
  }
}

std::string MakeString(std::vector<int> WND) {
  // Turns my vector of ints into a working fraction to be outputted later on
  std::string newvalue;
  if (WND[0] != 0 && WND[1] != 0) {
    newvalue = std::to_string(WND[0]) + "-" + std::to_string(WND[1]) + "/" +
               std::to_string(WND[2]);
    return newvalue;
  } else if (WND[1] == 0) {
    newvalue = std::to_string(WND[0]);
    return newvalue;
  } else {
    newvalue = std::to_string(WND[1]) + "/" + std::to_string(WND[2]);
    return newvalue;
  }
}

std::string ServeOne(std::vector<int> WND, int s) {
  // Checks if it contains an integer, if it is a fraction, or it is a whole
  // number
  // Then multiplies the fraction where needed to get to one serving
  // Ex: 2 servings {0,1,2} --> {0,1,4}
  // Ex: 2 servings {1,0,0} --> {0,1,2}
  if (WND[0] >= 1 && WND[2] != 0) {
    WND[1] = (WND[0] * WND[2]) + WND[1];
    WND[0] = 0;
  }
  if (WND[2] == 0 && WND[1] == 0) {
    WND[1] = WND[0];
    WND[2] = s;
    WND[0] = 0;
  } else {
    WND[2] = WND[2] * s;
  }
  Simplify(WND);
  return MakeString(WND);
}

void Recipe::AddIngredient(std::string I) {
  // Cleans the string by triming it
  // Adds each "word" into dedicated areas
  // value = fraction, unit = unit, name_ = name of ingredient
  // Runs ChangeServings at end to the same servings in order to get the WND
  // (Whole, Numerator, Denominator) and Simplify
  std::vector<std::string> V;
  std::string word = "";
  trim(I);
  V = CleanBeginEndString(I);
  value_.push_back(V[0]);
  unit_.push_back(V[1]);
  trim(I);
  name_.push_back(I);
  ChangeServings(servings);
}

void CleanInstruct(std::string Instructions, std::string &Ins) {
  // Works hand and hand with FixInstruct
  // Removes any spaces leftover from FixInstruct after a \n until you hit a
  // non-space
  for (int i = Instructions.size() - 1; i >= 0; i--) {
    if (Instructions[i] == '\n') {
      Ins = Instructions[i] + Ins;
      i--;
      while (isspace(Instructions[i])) {
        i--;
      }
      Ins = Instructions[i] + Ins;
    } else {
      Ins = Instructions[i] + Ins;
    }
  }
}
void FixInstruct(std::string Instructions, std::string &Ins) {
  // Works hand and hand with CleanInstruct
  // When a '\n' is found, skip all the whitespaces afterwards until text is
  // found
  // Otherwise add text and whitespaces
  for (int i = 0; i < Instructions.size(); i++) {
    if (Instructions[i] == '\n') {
      Ins = Ins + Instructions[i];
      i++;
      while (isspace(Instructions[i])) {
        i++;
      }
      Ins = Ins + Instructions[i];
    } else {
      Ins = Ins + Instructions[i];
    }
  }
  Instructions = Ins;
  Ins = "";
  CleanInstruct(Instructions, Ins);
}

bool isend(char a) {
  // If it is a punctuation, a '/', or word return true;
  if (isalpha(a) || ispunct(a) || a == '/') {
    return true;
  } else {
    return false;
  }
}

void Recipe::SetInstructions(std::string Instructions) {
  // Sets up the instructions based on the string given
  // Erases all tabs
  // Erases all spaces in the beginning and after the string
  // Then sets up the Instructions with newlines and spaces based on what was
  // desired
  std::string Ins = "";
  Instructions.erase(
      std::remove(Instructions.begin(), Instructions.end(), '\t'),
      Instructions.end());
  int index =
      std::distance(Instructions.begin(),
                    std::find_if(Instructions.begin(), Instructions.end(),
                                 [](auto a) { return isalpha(a); }));
  Instructions.erase(Instructions.begin(), Instructions.begin() + index);
  int index2 =
      std::distance(Instructions.rbegin(),
                    std::find_if(Instructions.rbegin(), Instructions.rend(),
                                 [](auto a) { return isend(a); }));
  Instructions.erase(Instructions.end() - index2, Instructions.end());
  FixInstruct(Instructions, Ins);
  Instructions_ = Ins;
}

std::ostream &operator<<(std::ostream &oss, Recipe const &r) {
  // Outputs all the Recipe Information to the Ostream
  oss << "Recipe for: " << r.food << std::endl;
  oss << "Serves " << r.servings << std::endl;
  oss << "Ingredients:" << std::endl;
  for (int i = 0; i <= r.name_.size() - 1; i++) {
    oss << r.value_[i] << " " << r.unit_[i] << " " << r.name_[i] << std::endl;
  }
  oss << std::endl;
  oss << "Instructions:" << std::endl;
  oss << r.Instructions_ << std::endl;
  oss << std::endl;
  return oss;
}

std::string Recipe::IngredientInOneServing(std::string n) {
  // accepts the name of a possible ingredient, and results a string denoting
  // the value, unit, and name
  //(each delimited by a space character) of the ingredient that a single
  //serving of the recipe would require.
  auto it = std::find(name_.begin(), name_.end(), n);
  std::string one_serve = "";
  int s = servings;
  if (it != name_.end()) {
    int index =
        std::distance(name_.begin(), std::find(name_.begin(), name_.end(), n));
    std::vector<int> WND = FindNumAndDen(value_[index]);
    std::string one_serve =
        ServeOne(WND, s) + " " + unit_[index] + " " + name_[index];
    return one_serve;
  } else {
    throw std::invalid_argument("Doesn't use this Ingredient");
  }
}

std::string WholeServeChanges(std::vector<int> &WND, int servings, int s) {
  // Changes Servings based on if the new serving is lower or higher than the
  // serving before
  // Ex: 3 servings {0,1,2} -> 10 servings {0,10,6} -> 10 servings {1,2,3}
  if (s > servings) {
    WND[1] = WND[1] * s;
    WND[2] = WND[2] * servings;
    WND[0] = 0;
    Simplify(WND);
    return MakeString(WND);
  } else if (s < servings) {
    WND[1] = WND[1] * s;
    WND[2] = WND[2] * servings;
    WND[0] = 0;
    Simplify(WND);
    return MakeString(WND);
  } else {
    Simplify(WND);
    return MakeString(WND);
  }
}

void Recipe::ChangeServings(int s) {
  // Puts the Whole number added onto the fraction and simplifes from there
  // accepts a positive integer denoting the number of servings the values of
  // the ingredients should be scaled to.
  // This member function should mutate the Recipe to reflect the change.
  for (int i = 0; i < value_.size(); i++) {
    std::vector<int> WND = FindNumAndDen(value_[i]);
    if (WND[0] >= 1 && WND[1] != 0 && WND[2] != 0) {
      WND[1] = (WND[0] * WND[2]) + WND[1];
      WND[0] = 0;
    } else if (WND[0] >= 1 && WND[1] == 0 && WND[2] == 0) {
      WND[1] = WND[0];
      WND[2] = 1;
      WND[0] = 0;
    }
    value_[i] = WholeServeChanges(WND, servings, s);
  }
  servings = s;
}
