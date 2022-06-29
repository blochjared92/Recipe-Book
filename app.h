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
#include "recipe.h"

class RecipeApp {
 private:
  std::vector<Recipe> Recipes;  // Vector of Recipes
  std::vector<std::string> value_;  // Fraction
  std::vector<std::string> unit_;  // Unit
  std::vector<std::string> name_;  // Name of Ingredient
  std::vector<std::string> Ingredients;  // Ingredients of Recipes

 public:
  RecipeApp() = default;
  void AddRecipe(Recipe &);
  void AddIngredientToPantry(std::string);
  Recipe UseUpIngredient(std::string);
  friend std::ostream &operator<<(std::ostream &, RecipeApp &);
};

void RecipeApp::AddRecipe(Recipe &r) {
  // Add Recipe into vector of Classes
  // Also sorts the vector by name of the Recipe
  Recipes.push_back(r);
  std::sort(Recipes.begin(), Recipes.end(),
            [r](Recipe a, Recipe b) { return RecipeName(a) < RecipeName(b); });
}

std::string Refresh(std::vector<int> WND) {
  // Simplifies the WND vector and turns it into a string again
  // {1,2,3} --> {0,5,3}
  // {3,0,0} --> {0,3,1}
  if (WND[0] >= 1 && WND[1] != 0 && WND[2] != 0) {
    WND[1] = (WND[0] * WND[2]) + WND[1];
    WND[0] = 0;
  } else if (WND[0] >= 1 && WND[1] == 0 && WND[2] == 0) {
    WND[1] = WND[0];
    WND[2] = 1;
    WND[0] = 0;
  }
  Simplify(WND);
  return MakeString(WND);
}

std::vector<int> MakeVector(std::string Ingredient) {
  // Makes the WND vector given a string of specified value,unit,name
  std::vector<std::string> V;
  std::string word = "";
  trim(Ingredient);
  V = CleanBeginEndString(Ingredient);
  return FindNumAndDen(V[0]);
}

std::string FindName(std::string Ingredient) {
  // Gets the name of a string of specified value,unit,name
  std::vector<std::string> V;
  std::string word = "";
  trim(Ingredient);
  V = CleanBeginEndString(Ingredient);
  return Ingredient;
}

void RecipeApp::AddIngredientToPantry(std::string Ingredient) {
  // accepts a string denoting the value, unit, and name of an ingredient and
  // saves it in the class.
  // Trims the string and Cleans it just like AddIngredient in recipe.h
  Ingredients.push_back(Ingredient);
  std::vector<std::string> V;
  std::string word = "";
  trim(Ingredient);
  V = CleanBeginEndString(Ingredient);
  std::vector<int> WND = FindNumAndDen(V[0]);
  value_.push_back(Refresh(WND));
  unit_.push_back(V[1]);
  trim(Ingredient);
  name_.push_back(Ingredient);
}

std::ostream &operator<<(std::ostream &oss, RecipeApp &ra) {
  // sends the Recipe and how it is printed to the ostream
  // write itself to an ostream with both the recipes and pantry contents
  // ordered by name.
  oss << "Recipes in the app (ordered by name):" << std::endl;
  for (int i = 0; i < ra.Recipes.size(); i++) {
    oss << ra.Recipes[i];
  }
  oss << "Ingredients in pantry (ordered by name):" << std::endl;
  std::sort(ra.name_.begin(), ra.name_.end(),
            [](auto a, auto b) { return a < b; });
  int count = 0;
  while (count != ra.name_.size()) {
    for (int j = 0; j < ra.name_.size(); j++) {  // Putting Ingredients in order
      for (int k = 0; k < ra.Ingredients.size(); k++) {
        if (ra.Ingredients[k].find(ra.name_[j]) != std::string::npos) {
          oss << ra.value_[k] << " " << ra.unit_[k] << " " << ra.name_[j]
              << std::endl;
          count += 1;
        }
      }
    }
  }
  std::cout << std::endl;
  return oss;
}

double MakeNum(std::vector<int> WND) {
  // Makes a decimal of the WND vector sent in
  // Ex: 1/2 -> 0.5
  if (WND[2] != 0) {
    return WND[0] + static_cast<double>(WND[1]) / static_cast<double>(WND[2]);
  } else {
    return static_cast<double>(WND[0]);
  }
}

bool CheckLimit(std::string value, double num) {
  // Checks if the decimal made in MakeNum with the WND made in FindNumAndDen is
  // smaller, equal, or
  // larger than the fraction entered in UseUpIngredient
  // returns a bool
  double v = MakeNum(FindNumAndDen(value));
  if (v < num) {
    return true;
  }
  if (v == num) {
    return true;
  } else {
    return false;
  }
}

void AdjustServings(Recipe &r, double num, int index) {
  // Changes the Servings based on how many times the value of the ingredient
  // can be used til it equals or is larger than the number entered in
  // UseUpIngredient
  // If it is larger, runs ChangeServings with the previous index
  int i = 0;
  bool TF = true;
  r.ChangeServings(1);
  while (TF != false) {
    std::vector<std::string> V = GetValues(r);
    TF = CheckLimit(V[index], num);
    if (TF) {
      i += 1;
      r.ChangeServings(1 + i);
    }
    if (!TF) {
      r.ChangeServings(i);
      break;
    }
  }
}

Recipe RecipeApp::UseUpIngredient(std::string I) {
  // Accepts a string denoting the value, unit, and name of an ingredient.
  // It should search the recipes (in order by name) and if the recipe uses that
  // ingredient,
  // it should return a copy of that recipe with the most amount of number of
  // servings
  // that the recipe can support with the indicated ingredient.
  std::vector<int> WND = MakeVector(I);
  std::string N = FindName(I);
  trim(N);
  int count = 0;
  Recipe r = Recipes[0];
  double num = MakeNum(WND);
  for (int i = 0; i < Recipes.size(); i++) {
    std::vector<std::string> names = GetName(Recipes[i]);
    r = Recipes[i];
    if (std::find(names.begin(), names.end(), N) != names.end()) {
      int index = std::distance(names.begin(),
                                std::find(names.begin(), names.end(), N));
      AdjustServings(r, num, index);
      count += 1;
      return r;
    }
  }
  if (count == 0) {
    throw std::invalid_argument("Doesn't use this Ingredient");
  }
  return r;
}
