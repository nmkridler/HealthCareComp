#ifndef __pcgdsfsfile_h__
#define __pcgdsfsfile_h__

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include "category.h"

typedef std::vector<std::string>      strList;
typedef std::map<std::string,strList> strMap;

class pcgdsfs
{
   public:
      // Constructor
      pcgdsfs(std::string &csvName,
              std::string &dsfsFile,
              std::string &pcgFile) : m_fileName(csvName),
                                      m_dsfsFile(dsfsFile),
                                      m_pcgFile(pcgFile)
      {
         // Go through the list of categories
         m_dsfs.reset(new category( m_dsfsFile, 0 ));
         m_pcg .reset(new category( m_pcgFile , 0 ));
      }

      // Destructor
      ~pcgdsfs(){}

      // Create all of the combinations of pcg,dsfs
      void combinations()
      {
         std::vector<std::string> pcg = m_pcg->elements();
         std::vector<std::string> dsfs = m_dsfs->elements();
         m_comboIdx.clear();
         // For each pcg, loop over dsfs
         int idx = 0;
         for( size_t i = 0; i < pcg.size(); ++i)
         {
            for( size_t j = 0; j < dsfs.size(); ++j)
            {
               std::string tmp(dsfs[j]+","+pcg[i]);
               m_comboIdx[tmp] = idx;
               idx++;
            }
         }
         // Initialize the adjacency matrices
         m_h0Mat.assign(m_comboIdx.size()*m_comboIdx.size(),0.);
         m_h1Mat.assign(m_comboIdx.size()*m_comboIdx.size(),0.);

      }

      // Parse the file
      bool parseFile()
      {
         std::ifstream inFile(m_fileName.c_str());
         if( !inFile ) return false;

         std::string tmpStr;
         while( getline(inFile,tmpStr) )
         {
            // Tokenize the line
            std::vector<std::string> strVector;
            std::stringstream tmpStream(tmpStr);
         
            // 0 - memberID
            // 1 - dsfs
            // 2 - pcg
            // 3 - days in hospital
            while( std::getline(tmpStream,tmpStr,',') )
            { 
               if( tmpStr.size() == 0 ) tmpStr = "NULL";
               strVector.push_back(tmpStr);
            }
            // Join the dsfs and pcg
            std::string tmp(strVector[1]+","+strVector[2]);
            if( std::atoi(strVector[3].c_str()) > 0 )
            {
               m_h1Member[strVector[0]].push_back(tmp);
            } else
            {
               m_h0Member[strVector[0]].push_back(tmp);
            }
         } 
         inFile.close();

         // Call build matrix
         buildMatrix(m_h1Member, m_h1Mat);
         buildMatrix(m_h0Member, m_h0Mat);

         // Write to file
         writeToFile();
         return true;
      } 
 

      void writeToFile()
      {
         std::ofstream outFileH0;
         std::ofstream outFileH1;
         outFileH0.open("adjacencyH0.csv");
         outFileH1.open("adjacencyH1.csv");

         size_t npts = m_comboIdx.size();
         for( size_t i = 0; i < npts; ++i)
         {
            for( size_t j = 0; j < npts-1; ++j)
            {
               outFileH0 << m_h0Mat[j + i*npts] <<",";
               outFileH1 << m_h1Mat[j + i*npts] <<",";
            }
            outFileH0 << m_h0Mat[(i+1)*npts - 1] << std::endl;
            outFileH1 << m_h1Mat[(i+1)*npts - 1] << std::endl;
         }
         outFileH0.close();
         outFileH1.close();
          
      }

   private:
      std::string                m_fileName;  ///< Data file name
      std::string                m_dsfsFile;  ///< Data file name
      std::string                m_pcgFile;   ///< Data file name
      categoryPtr                m_pcg;       ///< Category Objects
      categoryPtr                m_dsfs;      ///< Category Objects
      std::vector<double>        m_h0Mat;     ///< H0 adjacency matrix
      std::vector<double>        m_h1Mat;     ///< H1 adjacency matrix
      strMap                     m_h0Member;    ///< Member map
      strMap                     m_h1Member;    ///< Member map
      categoryMap                m_comboIdx;  ///< Combos

      // Build the adjacency matrix
      void buildMatrix(strMap              const & member,
                       std::vector<double>       & matrix)
      {
         strMap::const_iterator mIter = member.begin();
         int npts = static_cast<int>(m_comboIdx.size()); 
         while( mIter != member.end() )
         {
            // For this member, loop over the claims
            for( size_t i = 0; i < mIter->second.size(); ++i)
            {
               for( size_t j = i; j < mIter->second.size(); ++j)
               {
                  int idx = m_comboIdx[mIter->second[i]];
                  int jdx = m_comboIdx[mIter->second[j]];
                  int index = jdx + idx*npts;
                  matrix[index] += 1.0;
               }
            }
            ++mIter;
         }
      }

};

#endif
