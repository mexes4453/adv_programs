
#ifndef XIO_HPP
#define XIO_HPP

# include <vector>
# include <string>
# include <iostream>
# include <fstream>
# include <stdexcept>

# define XIO__COL_BLUE "\033[0;34m"
# define XIO__COL_RED "\033[0;31m"
# define XIO__COL_YELLOW "\033[0;33m"
# define XIO__COL_MAGENTA "\033[0;35m"
# define XIO__COL_DEFAULT "\033[0m"


# define XIO__ERR_MSG_FileOpenFailed   XIO__COL_RED "Error: could not open file" XIO__COL_DEFAULT
# define XIO__ERR_MSG_BadInput         XIO__COL_RED "Error: bad input" XIO__COL_DEFAULT

/* Methods */

bool XIO__IsFileOpen(char const *fp, std::ifstream &ifs);
bool XIO__IsFileEmpty(std::ifstream &ifs);
bool XIO__OpenFile(std::string const &fpStr, std::ifstream &dbInpFileStream);
int  XIO__GetFilelines(char const *fp, std::vector<std::string> &fileLines);

#endif /* XIO _HPP */
