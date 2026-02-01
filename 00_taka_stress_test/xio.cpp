#include "xio.hpp"



bool XIO__IsFileOpen(char const *fp, std::ifstream &ifs)
{
    bool result = false;

    ifs.open(fp, std::ifstream::in);
    if (ifs.good())
        result = true;
    return (result);
}




bool XIO__IsFileEmpty(std::ifstream &ifs)
{
    bool result = false;
    int length;
    ifs.seekg(0, ifs.end);
    length = ifs.tellg();
    ifs.seekg(0, ifs.beg);

#ifdef __DEBUG__
    std::cout << "file size: " << length << " bytes." << std::endl;
#endif /* __DEBUG__ */

    if (length == 0)
        result = true;
    return (result);
}




bool    XIO__OpenFile(std::string const &fpStr, std::ifstream &dbInpFileStream)
{
    bool    result = false;

    try
    {
        // open file -
        if (!XIO__IsFileOpen(fpStr.c_str(), dbInpFileStream))
            throw std::runtime_error(XIO__ERR_MSG_FileOpenFailed);

        // is file empty
        if (XIO__IsFileEmpty(dbInpFileStream))
        {
            result = false;
            goto escape;
        }
        result = true;

#ifdef __DEBUG__
        std::cout << "openFile: " << "open file -> successful" << std::endl;
#endif /* __DEBUG__ */

    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
escape:
    return (result);
}


int XIO__GetFilelines(char const *fp, std::vector<std::string> &fileLines)
{
    std::string              fpStr(fp);
    std::ifstream            dbInpFileStream;
    std::string              lineStr = "";
    int                      retCode = -1;

    try
    {
        if ( !XIO__OpenFile(fpStr, dbInpFileStream) )
            goto escape;

        while (dbInpFileStream.good())
        {
            std::getline(dbInpFileStream, lineStr);
            fileLines.push_back( lineStr );
        }
        dbInpFileStream.close();
        retCode = 0;

#ifdef __DEBUG__
        for (int x=0; x<10; x++)
        {
            COUT << fileLines[x] << ENDL;
        }
        COUT << "total line: " << fileLines.size() << ENDL;
#endif /* __DEBUG__ */

    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
escape:
    return retCode;
}

