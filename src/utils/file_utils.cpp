#include "file_utils.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace FileUtils
{
  std::string readTextFile( const char* filepath )
  {
    std::ifstream file(filepath, std::ios::in);
    
    if ( !file.is_open() )
    {
      std::cerr << "Failed to open file: " << filepath << std::endl;
      return {};
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  }
}
