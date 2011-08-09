#ifndef __csvfile_h__
#define __csvfile_h__

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include "category.h"

class csvfile
{
   public:
      // Constructor
      csvfile(std::string &csvName,
              std::vector<std::string> &categories) : m_fileName(csvName),
                                                      m_catFiles(categories),
                                                      m_member()
      {
         // Go through the list of categories
         int startIdx = 0;
         for( size_t idx = 0; idx < m_catFiles.size(); ++idx)
         {
            categoryPtr tmp( new category( m_catFiles[idx], startIdx ) );
            if( tmp->readFile() )
            {
               // Get the name and then assign the pointer to the map
               std::string name(tmp->getName()); 
               startIdx += tmp->getSize();
               m_category.push_back(tmp); 
            }
            tmp.reset();
         }
         m_numCat = startIdx;
         m_features.assign(m_numCat,0);
      }

      // Destructor
      ~csvfile(){}

      // Parse the file
      bool parseFile()
      {
         std::ifstream inFile(m_fileName.c_str());
         m_outFile.open("features.csv");
         if( !inFile ) return false;

         std::string tmpStr;
         bool firstLine = true;
         while( getline(inFile,tmpStr) )
         {
            // Tokenize the line
            std::vector<std::string> strVector;
            std::stringstream tmpStream(tmpStr);
         
            // 0  - the member ID
            // 1  - the age                category[0]
            // 2  - the sex                category[1]
            // 10 - length of stay         category[2]
            // 12 - the condition group    category[3]
            // 13 - charlson idx           category[4]
            // 14 - SupLOS                 category[5]
            while( std::getline(tmpStream,tmpStr,',') )
            { 
               if( tmpStr.size() == 0 ) tmpStr = "NULL";
               strVector.push_back(tmpStr);
            }
           
            // Update the vector
            if( strVector.size() < 14 || m_category.size() < 5 ) return false;
            int ageIdx = m_category[0]->getIdx(strVector[1]);
            int sexIdx = m_category[1]->getIdx(strVector[2]);
            int losIdx = m_category[2]->getIdx(strVector[10]);
            int pcgIdx = m_category[3]->getIdx(strVector[12]);
            int chlIdx = m_category[4]->getIdx(strVector[13]);
            int supIdx = m_category[5]->getIdx(strVector[15]);
 
            std::string currentID = strVector[0];
            if( firstLine )
            {
               m_memberID = currentID;
               firstLine = false;
         
               // Add to the group
               m_member.push_back(ageIdx);
               m_member.push_back(sexIdx);
               m_member.push_back(losIdx);
               m_member.push_back(pcgIdx);
               m_member.push_back(chlIdx);
               m_member.push_back(supIdx);
               updateFeatures();
            }
 
            // check the name
            if( m_memberID != currentID )
            {
               // Form the outer product and store it
               writeToFile();
               m_member.clear();
               m_features.assign(m_numCat,0);

               // Update the ID
               m_member.push_back(ageIdx);
               m_member.push_back(sexIdx);
               m_member.push_back(losIdx);
               m_member.push_back(pcgIdx);
               m_member.push_back(chlIdx);
               m_member.push_back(supIdx);
               m_memberID = currentID;
               updateFeatures();
            }
            else
            {
               // The member exists, only need to 
               // add the claim specific data
               m_member.clear();
               m_member.push_back(losIdx);
               m_member.push_back(pcgIdx);
               m_member.push_back(chlIdx);
               m_member.push_back(supIdx);
               updateFeatures();
            }
    
         }
         writeToFile();
         std::cout << "Success!" << std::endl;
         inFile.close();
         m_outFile.close();
        
         return true;
      } 
 
      void updateFeatures()
      {
         for( size_t idx = 0; idx < m_member.size(); ++idx)
         {
            int fIdx = m_member[idx];
            if( fIdx < static_cast<int>(m_numCat) ) m_features[fIdx] += 1.0;
         }
        
      }

      void writeToFile()
      {

         for( size_t i = 0; i < m_numCat-1; ++i)
         {
            m_outFile << m_features[i] <<",";
         }
         m_outFile << m_features[m_numCat-1] << std::endl;
          
      }

   private:
      std::string                m_fileName;  ///< Data file name
      std::vector<categoryPtr>   m_category;  ///< Category Objects
      std::vector<std::string>   m_catFiles;  ///< Category Files
      std::vector<int>           m_member;    ///< Indices to change
      std::vector<float>         m_features;    
      size_t                     m_numCat;
      std::string                m_memberID;
      std::ofstream              m_outFile;
};

#endif
