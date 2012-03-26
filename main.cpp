#include "category.h"
#include <iostream>
#include <string>
#include <vector>
#include "csvfile.h"
#include "pcgdsfs.h"
#include "pcgdsfsY3.h"

int main( int argc, char** argv)
{
   std::string directory("/home/nick/competition/data/");

   // Create a vector of strings pointing to the various
   // lists of categories
   std::vector<std::string> categories;
   categories.push_back(directory+"categories/DSFS.txt");
   categories.push_back(directory+"categories/PrimaryConditionGroup.txt");

   std::string dataFileY3(directory+"dsfspcgY3.csv");
   std::string dataFileY2(directory+"pcgdsfsY2_dih.csv");
   std::string dataFileY1(directory+"pcgdsfsY1_dih.csv");
  

   // Create the parser
   pcgdsfs csvY1(dataFileY1,categories[0],categories[1]);
   pcgdsfs csvY2(dataFileY2,categories[0],categories[1]);
   pcgdsfsY3 csvY3(dataFileY3,categories[0],categories[1]);
   
   csvY1.combinations();
   csvY1.parseFile();

   csvY2.combinations();
   csvY2.parseFile();

   csvY3.combinations();
   csvY3.parseFile();

   std::vector<double> h0matY1 = csvY1.H0Matrix();
   std::vector<double> h1matY1 = csvY1.H1Matrix();
   std::vector<double> h0matY2 = csvY2.H0Matrix();
   std::vector<double> h1matY2 = csvY2.H1Matrix();

   // Determine the ratio
   std::vector<double> ratio;
   ratio.assign(h0matY1.size(),0.);
   double meanValue = 0;
   double nvalid = 0;
   for( size_t i = 0; i < ratio.size(); ++i)
   {
      double nMem = h0matY1[i] + h1matY1[i] + h0matY2[i] + h1matY2[i];
      double h1mem = h1matY1[i] + h1matY2[i];
      if( nMem > 100 )
      {
         meanValue += h1mem/nMem;
         ratio[i] = h1mem/nMem;
         ++nvalid;
      } //else ratio[i] = -1;
      
   }
   meanValue /= nvalid;
#if 0
   for( size_t i = 0; i < ratio.size(); ++i)
   {
      if( ratio[i] < 0 ) ratio[i] = meanValue;
   }
   std::cout << "mean: " << meanValue << std::endl;
#endif
   // Now create the metrics
   csvY1.createMetrics(ratio);
   csvY2.createMetrics(ratio);
   csvY3.createMetrics(ratio);
   csvY1.writeMetricsToFile(std::string("y1members.csv"));
   csvY2.writeMetricsToFile(std::string("y2members.csv"));
   csvY3.writeMetricsToFile(std::string("y3members.csv"));
}

