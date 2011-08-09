#include "category.h"
#include <iostream>
#include <string>
#include <vector>
#include "csvfile.h"

int main( int argc, char** argv)
{
   std::string directory("/home/nick/competition/data/");

   // Create a vector of strings pointing to the various
   // lists of categories
   std::vector<std::string> categories;
   categories.push_back(directory+"categories/AgeAtFirstClaim.txt");
   categories.push_back(directory+"categories/Sex.txt");
   categories.push_back(directory+"categories/LengthOfStay.txt");
   categories.push_back(directory+"categories/PrimaryConditionGroup.txt");
   categories.push_back(directory+"categories/CharlsonIndex.txt");
   categories.push_back(directory+"categories/SupLOS.txt");

   std::string dataFile(directory+"filtered/sort_nostayData_y1.csv");
  

   // Create the parser
   csvfile csv(dataFile,categories);
   csv.parseFile();

}

