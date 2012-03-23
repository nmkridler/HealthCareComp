#include "category.h"
#include <iostream>
#include <string>
#include <vector>
#include "csvfile.h"
#include "pcgdsfs.h"

int main( int argc, char** argv)
{
   std::string directory("/home/nick/competition/data/");

   // Create a vector of strings pointing to the various
   // lists of categories
   std::vector<std::string> categories;
   categories.push_back(directory+"categories/DSFS.txt");
   categories.push_back(directory+"categories/PrimaryConditionGroup.txt");

   std::string dataFile(directory+"pcgdsfsY2_dih.csv");
  

   // Create the parser
   pcgdsfs csv(dataFile,categories[0],categories[1]);
   csv.combinations();
   csv.parseFile();
}

