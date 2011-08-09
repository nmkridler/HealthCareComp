#ifndef __category_h__
#define __category_h__

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <boost/shared_ptr.hpp>

typedef std::map<std::string,int> categoryMap; 

class category
{
   public:
      // Constructor
      category(std::string const &fileName, int const &start) : m_size(0),
                                                          m_fileName(fileName), 
                                                          m_name(),
                                                          m_startIdx(start) {}

      // Destructor
      ~category(){}

      // Input file reader
      bool readFile()
      {
         // open up the file
         std::ifstream inFile;
         inFile.open(m_fileName.c_str(),std::ifstream::in);
         if( !inFile ) return false;
 
         // First line is the gategory name
         std::getline(inFile,m_name);
         int idx = m_startIdx;
         std::string tmpStr;
         // Loop over the other lines
         while( std::getline(inFile,tmpStr) ) m_list.push_back(tmpStr);
         inFile.close();

         for( size_t i = 0; i < m_list.size(); ++i)
         { 
            m_catIdx[m_list[i]] = idx;
            idx++;
         }
         m_size = m_catIdx.size();
         categoryMap::iterator iter = m_catIdx.begin();
         
         return true;
      }
      
      // Idx getter
      int const  & getIdx(std::string &input)
      {
         if( m_catIdx[input] == 0 ) return m_startIdx;
         return m_catIdx[input];
      }

      // size getter
      size_t const & getSize() const { return m_size; }

      // Name getter
      std::string const & getName() const {return m_name;}
   private:
      size_t           m_size;
      std::string      m_fileName;  // Input file name
      std::string      m_name;      // Name of the category
      int              m_startIdx;  // First Index
      std::vector<std::string> m_list;
      categoryMap      m_catIdx;    // Index of the category
};

typedef boost::shared_ptr<category> categoryPtr;
#endif
