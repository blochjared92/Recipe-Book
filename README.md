# Recipe-Book

A header-only library that defines a class that can hold recipes and a class that represents a simple recipe app.

In this project, you will be making a header-only library that defines a class that can hold recipes and a class that represents a (extremely) simple recipe app.

Here are descriptions of the only three files that should be submitted:

recipe.h: This header file needs to contain the declaration and definition of a class named Recipe. It can also contain other functions / classes if you find them useful. 

app.h: This header file needs to contain the declaration and definition of a class named RecipeApp. It can also contain other functions / classes if you find them useful. 

utility.h: This file is never directly included by the unit tests, but you can put code needed to be included by the other files in this file if you would like.

Only the public member functions of Recipe and RecipeApp you are allowed to have are described here. Your solutions will likely have private attributes, as well as, additional functions and classes to support the described classes.

The test cases make use of raw string literals. The R”(…)” is how to make a raw  string literal. This is useful for using multiline strings in code. See: https://stackoverflow.com/questions/1135841/c-multiline-string-literal 

class Recipe:

This class should have a constructor that takes a string (representing the name of a recipe) and a integer representing the number of servings this recipe will serve. In this project, all number of servings will be positive integers. Example: Recipe r("Microwave Popcorn", 3);

There should be a member function named AddIngredient that accepts a string with three parts: a (possibly fractional) value, a unit, and a name each separated by white space. For example: r.AddIngredient("1/2 cup unpopped popcorn")

The values can consist of whole numbers (i.e. 4), fractions (i.e. 3/4), mixed fractions (i.e. 3-4/5), or even improper fractions (i.e. 9/4). All values when written to an ostream must be in reduced-mixed form (3-9/2 should become 7-1/2).

The class should also have a member function named SetInstructions that accepts a (possibly multiline) string denoting the instructions to follow for the recipe. This string may have blank lines, leading whitespace, and/or trailing whitespace on each line. This whitespace should be removed when a Recipe is written to an ostream.

The class should support the operator<< to write to an ostream. Please see the test cases or below for examples of the specific formatting required.

The class should also support a function, named IngredientInOneServing, which accepts the name of a possible ingredient, and results a string denoting the value, unit, and name (each delimited by a space character) of the ingredient that a single serving of the recipe would require. Example, if Microwave Popcorn serves 3, and has as "1/2 cup unpopped popcorn" an ingredient, then a call like r.IngredientInOneServing("unpopped popcorn") would return "1/6 cup unpopped popcorn". If the recipe doesn't use that ingredient, throw a std::invalid_argument exception.

Lastly, the class should support a member function named ChangeServings that accepts a positive integer denoting the number of servings the values of the ingredients should be scaled to. This member function should mutate the Recipe to reflect the change.

class RecipeApp:

This class represents a simple recipe manager that can save recipes and items in a pantry. And can also scale recipes to consume nearly all of a particular ingredient a customer have have.

This class should have a default constructor.

This class should have a member function named AddRecipe that accepts a Recipe and saves it in the class.

This class should have a member function named AddIngredientToPantry that accepts a string denoting the value, unit, and name of an ingredient and saves it in the class.

This class should support the operator<< and write itself to an ostream with both the recipes and pantry contents ordered by name.

Lastly, this class should implement a member function named UseUpIngredient that accepts a string denoting the value, unit, and name of an ingredient. It should search the recipes (in order by name) and if the recipe uses that ingredient, it should return a copy of that recipe with the most amount of number of servings that the recipe can support with the indicated ingredient. If no recipe uses that ingredient, throw a std::invalid_argument exception.
