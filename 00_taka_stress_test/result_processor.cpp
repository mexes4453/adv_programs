/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cudoh <cudoh@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:40:45 by cudoh             #+#    #+#             */
/*   Updated: 2023/05/21 16:40:45 by cudoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(void) {}

BitcoinExchange::BitcoinExchange(char const *fp) : dbFlag(true)
{
    readDbFile();
    if (db.size() > 0)
    {
        readInputFile(fp);
    }
}

bool BitcoinExchange::isDateValid(std::string &Str)
{
    bool                result = false;
    std::stringstream   dateStream(Str);
    struct tm           t;
    struct tm           *nt;
    time_t              when;
    char                delimiter;
    int y=0, m=0, d=0, ny=0, nm=0, nd=0;

    memset((void *)&t, 0, sizeof(tm));
    stripWhiteSpace(Str);
    try
    {
        dateStream >> y >> delimiter >> m >> delimiter >> d;
        t.tm_year = y - 1900;
        t.tm_mon = m - 1;
        t.tm_mday = d;
        t.tm_isdst = -1;
        if (t.tm_year < 0 || t.tm_mon < 0 || t.tm_mday < 0)
        {
            //throw std::runtime_error(ERR_MSG_NegativeDateComp);
            goto end;
        }
        when = mktime(&t);
        nt = localtime(&when);
        dateStream.clear();
        dateStream.str(asctime(nt));
        
        validateDateValue(dateStream, nm, ny, nd);

        //use time diff to check if data is in future
#ifdef _DEBUG_
        COUT << "Time difference: " << difftime(time(NULL), when) << ENDL;
#endif
        if (difftime(time(NULL), when) < 0)
        {
            //throw std::runtime_error(ERR_MSG_DateInFuture);
            goto end;
        }
        result = (y == ny && m == nm && d == nd);
#ifdef _DEBUG_
        //COUT << dateTimeStr << ENDL;
        COUT << y << " : " << ny << ENDL;
        COUT << m << " : " << nm << ENDL;
        COUT << d << " : " << nd << ENDL;
#endif
    }
    EXCEPTION_HANDLER();
end:
    return (result);
}

BitcoinExchange::BitcoinExchange(BitcoinExchange const &obj)
{
    db = obj.db;
}

BitcoinExchange &BitcoinExchange::operator=(BitcoinExchange const &obj)
{
    db = obj.db;
    return (*this);
}

BitcoinExchange::~BitcoinExchange(void) {}

dbType &BitcoinExchange::getDb(void)
{
    return (db);
}

bool BitcoinExchange::isFileFormatValid(std::string const &fpStr)
{
    bool result = false;
    std::size_t idx = 0;
    std::string fmtStr(FILE_FMT_CSV);

    idx = fpStr.rfind(fmtStr);
    if (idx != std::string::npos)
    {
#ifdef _DEBUG_
        COUT << "FOUND : " << idx << " -> " << fpStr.substr(idx + 1) << ENDL;
#endif // DEBUG
        if (fpStr.substr(idx) == fmtStr)
        {
#ifdef _DEBUG_
            COUT << "end of file name" << ENDL;
#endif // DEBUG
            result = true;
        }
    }
    return (result);
}

bool BitcoinExchange::isFileOpen(char const *fp, std::ifstream &ifs)
{
    bool result = false;

    ifs.open(fp, std::ifstream::in);
    if (ifs.good())
        result = true;
    return (result);
}

bool BitcoinExchange::isFileEmpty(std::ifstream &ifs)
{
    bool result = false;
    int length;
    ifs.seekg(0, ifs.end);
    length = ifs.tellg();
    ifs.seekg(0, ifs.beg);
#ifdef _DEBUG_
    COUT << "file size: " << length << " bytes." << ENDL;
#endif // DEBUG
    if (length == 0)
        result = true;
    return (result);
}

void BitcoinExchange::stripWhiteSpace(std::string &str)
{
    std::string::size_type found = str.find_first_not_of(CHARS_WHITESPACE);

    // remove all leading whitespace characters
    if (found != std::string::npos)
    {
        str = str.substr(found);
    }
    else
    {
        str.clear();
    }

    // remove all trailing whitespace characters
    if (str.size() > 0)
    {
        found = str.find_last_not_of(CHARS_WHITESPACE);
        if (found != std::string::npos)
        {
            str.erase(found + 1);
        }
        else
        {
            str.clear();
        }
    }
#ifdef _DEBUG_
    COUT << str << ENDL;
#endif
}

bool BitcoinExchange::isSpaceInStr(std::string const &str)
{
    std::string::size_type found = str.find_first_of(CHARS_WHITESPACE);
    bool result = false;

    if (found != std::string::npos)
    {
        result = true;
    }
    return (result);
}

bool BitcoinExchange::isFloatValid(std::string &valueStr, int unsigned &lineCnt)
{
    bool result = false;
    std::stringstream sStream(valueStr);
    float valueFloat = 0.0;
    
    stripWhiteSpace(valueStr);
    try
    {
        if (isSpaceInStr(valueStr))
        {
            (!isHeaderInvalid(lineCnt, FLAG_OnErrProcDataAsHeader)) ?
            (throw std::runtime_error(ERR_MSG_InvalidFmtSpace)) :
            lineCnt;
        }
        else if (valueStr.find_first_not_of(CHARS_VALID_FLOAT) != std::string::npos)
        {
            (lineCnt == 1) ?  lineCnt :
            (throw std::runtime_error(ERR_MSG_InvalidFmtBadChar));
        }
        if (sStream >> valueFloat)
        {
#ifdef _DEBUG_            
            COUT << "isFloatValid: valueStr -> " << valueStr << ENDL; 
            COUT << "isFloatValid: valueFloat -> " << valueFloat << ENDL;
#endif
            if ( !dbFlag && valueFloat < 0 )
            {
                (lineCnt > 1) ? 
                (throw std::runtime_error(ERR_MSG_ValueTooLow)) :
                lineCnt;
            }
            else if ( !dbFlag && valueFloat > MAX_VALUE)
            {
                (lineCnt > 1) ?
                (throw std::runtime_error(ERR_MSG_ValueTooHigh)):
                lineCnt;
            }
            result = true;
        }
        else
        {
            (lineCnt > 1) ?
            (throw std::runtime_error(ERR_MSG_InvalidFloat)) :
            lineCnt;
        }
    }
    EXCEPTION_HANDLER();
    return (result);
}

int    BitcoinExchange::convertMonthStrToInt(std::string &str)
{
    int result = -1;
    static std::string months[12] = {
                                     "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                     "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
                                    };
    int x = 0;
    while (x < 12)
    {
        if (months[x] == str)
        {
            result = x + 1;
            break ;
        }
        x++;
    }
    return (result);
}

void BitcoinExchange::validateDateValue(std::stringstream &ss,
                                        int &nm, int &ny, int &nd)
{
    std::string tok;
    int x = 0, z = 0;
    while (std::getline(ss, tok, ' '))
    {
        if (tok.size() > 0)
        {
            z++;
#ifdef _DEBUG_
            COUT << x << " - " << tok << ENDL;
#endif
            switch (z)
            {
                case 2:
                {
                    nm = convertMonthStrToInt(tok);
                    break;
                }
                case 3:
                {
                    nd = (strtof(tok.c_str(), NULL));
                    break;
                }
                case 5:
                {
                    ny = (strtof(tok.c_str(), NULL));
                    break;
                }
                default:
                {
                }
            }
        }
        x++;
    }
}

bool    BitcoinExchange::openFile(std::string const &fpStr, std::ifstream &dbInpFileStream)
{
    bool    result = false;
    try
    {
        if (!isFileFormatValid(fpStr)) // check that file format is valid
            throw std::runtime_error(ERR_MSG_InvalidFileFmt);

        // open file -
        if (!isFileOpen(fpStr.c_str(), dbInpFileStream))
            throw std::runtime_error(ERR_MSG_FileOpenFailed);

        // is file empty
        if (isFileEmpty(dbInpFileStream))
        {
            result = false;
            goto end;
        }
        result = true;
#ifdef _DEBUG_ 
        COUT << "openFile: " << "open file -> successful" << ENDL;
#endif
    }
    EXCEPTION_HANDLER();
end:
    return (result);
}


bool    BitcoinExchange::isLineEmpty(std::string const &lineStr)
{
    std::string line = lineStr;

    stripWhiteSpace(line);
#ifdef _DEBUG_
    COUT << "isLineEmpty : size -> " << line.size() << ENDL;
#endif
    if (line.size() == 0)
    {
        return (true);
    }
    return (false);
}


bool    BitcoinExchange::isHeaderInvalid(
                                        int unsigned &lineCnt,
                                        tFlag flg)
{
    std::stringstream ss("");
    bool              result = false;
#ifdef _DEBUG_
    COUT << "isHeaderInvalid : lineCnt -> " << lineCnt << ENDL;
    COUT << "isHeaderInvalid : flg -> " << flg << ENDL;
#endif
    if (lineCnt == 1)
    {
        try 
        {
            switch (flg)
            {
                case FLAG_OnOkProcDataAsHeader:
                {
                    result = true;
                    ss << COL_YELLOW "Info! missing header." COL_DEFAULT; 
                    throw std::runtime_error(ss.str());
                    break ;
                }
                case FLAG_OnErrProcDataAsHeader:
                {
                    result = true;
                    ss << COL_RED "Error! Bad Input or missing header."
                       << COL_DEFAULT;
                    throw std::runtime_error(ss.str());
                    break ;
                }
                case FLAG_ProcParseErr:
                {
                    result = true;
                    ss << COL_RED "Info! Bad Input or wrong header format." 
                    << COL_DEFAULT;
                    throw std::runtime_error(ss.str());
                    break ;
                }
                case FLAG_DoNothingOnErr:
                {
                    result = true;
                    break ;
                }
                default:
                { 
                }
            }
        }
        EXCEPTION_HANDLER();
    }
    return (result);
}



tRetCode    BitcoinExchange::parseLineStr(std::string const &lineStr, 
                                      std::string const &delimter, 
                                      std::string &dateStr,  
                                      std::string &valStr)
{
    std::string::size_type  found = 0;
    tRetCode                result = RC_LineEmpty;

    try
    {
#ifdef _DEBUG_
        COUT << "parseLineStr: lineStr -> " << lineStr << ENDL;
#endif
        if (isLineEmpty(lineStr))
            goto end;

        found = lineStr.find(delimter);
#ifdef _DEBUG_
        COUT << "parseLineStr: found -> " << found << ENDL;
#endif
        if (found != std::string::npos)
        {
            dateStr = lineStr.substr(0, found);
            valStr = lineStr.substr(found + 1);
            if (dateStr.size() > 0 && valStr.size() > 0)
            {
#ifdef _DEBUG_
                COUT << "parseLineStr : date string -> " << dateStr << ENDL;
                COUT << "parseLineStr : rate string -> " << valStr << ENDL;
#endif
                result = RC_Success;
                goto end;
            }
        }
        result = RC_BadInput;
    }
    EXCEPTION_HANDLER();
end:
#ifdef _DEBUG_
    COUT << "parseLineStr : result -> " << result << ENDL;
#endif
    return (result);
}

bool BitcoinExchange::processData( std::string &dateStr, \
                                   std::string &valStr,  \
                                   float &valFloat, int unsigned &lineCnt
                                   )
{
    bool    result = false;

    if (!isDateValid(dateStr))
    {
        if (lineCnt == 1 && isFloatValid( valStr, lineCnt ))
        { 
            isHeaderInvalid(lineCnt, FLAG_OnErrProcDataAsHeader);
        }
        else if (lineCnt == 1 && !isFloatValid( valStr, lineCnt))
        {  
            if (isSpaceInStr( valStr ))
                isHeaderInvalid(lineCnt, FLAG_ProcParseErr);
            else
            {
                isHeaderInvalid(lineCnt, FLAG_DoNothingOnErr);
            }
        }
        else
        {
            std::cerr << COL_RED "Error! bad input => "
                  << dateStr << COL_DEFAULT << ENDL;
        }
        goto end;
    }

    if (isFloatValid( valStr, lineCnt ))
    {
        valFloat = std::strtof(valStr.c_str(), NULL);
        result = true;
#ifdef _DEBUG_
        COUT << "processData: valStr -> " << valStr << ENDL;
#endif
    }
end:
#ifdef _DEBUG_
    COUT << "processData: result -> " << result << ENDL;
#endif
    return (result);
}

float   BitcoinExchange::fetchRate(std::string const &dbKeyDate)
{
    dbType::iterator  it = db.begin();
    float             rateFloat = 0.0;

    it = db.lower_bound(dbKeyDate);
    if (it != db.end())
    {
        if (it->first != dbKeyDate && it != db.begin())
        {
            rateFloat = (--it)->second;
        }
        else
        {
            rateFloat = it->second;
        }
    }
#ifdef _DEBUG_
    COUT << "fetchRate: rateFloat -> " << rateFloat << ENDL;
#endif
    return (rateFloat);
}



void    BitcoinExchange::readDbFile(void)
{
    std::string         fpStr(FILE_DB);
    std::ifstream       dbInpFileStream;
    std::string         lineStr = "";
    std::string         dateStr;
    std::string         valStr;
    float               rateFloat = 0.0;
    static unsigned int lineCnt = 0;
    tRetCode            retVal = RC_LineEmpty;

    try
    {
        if (!openFile(fpStr, dbInpFileStream))
            goto end;

        while (dbInpFileStream.good())
        {
            std::getline(dbInpFileStream, lineStr);
#ifdef _DEBUG_
            COUT << lineCnt << ENDL;
            COUT << "readDbFile : -----------------------------------" << ENDL;
            COUT << "readDbFile: lineCnt -> " << lineCnt << ENDL;
#endif
            retVal = parseLineStr(lineStr, DELIMITER_COMMA, dateStr, valStr);
            switch  ( retVal ) 
            {
                case RC_LineEmpty: break ;
                case RC_BadInput:
                {
                    ++lineCnt;
                    if (!isHeaderInvalid(lineCnt, FLAG_ProcParseErr))
                    {
                        std::cerr << COL_RED "Error! bad input => " \
                              << lineStr << COL_DEFAULT << ENDL;
                    }
                    break ;
                }
                default:
                {
                    ++lineCnt;
                    if (processData(dateStr, valStr, rateFloat, lineCnt ))
                    {
                        isHeaderInvalid(lineCnt, FLAG_OnOkProcDataAsHeader);
                        db.insert(pairType(dateStr, rateFloat));
                    }
                }
            }
        }
        dbInpFileStream.close();
    }
    EXCEPTION_HANDLER();
    dbFlag = false;
end:
    return ;
}

void    BitcoinExchange::readInputFile(char const *fp)
{
    std::string     fpStr(fp);
    std::ifstream   dbInpFileStream;
    std::string     lineStr = "";
    std::string     dateStr;
    std::string     valStr;
    float           valFloat = 0.0;
    tRetCode        retVal = RC_LineEmpty;
    static unsigned int lineCnt = 0;

    try
    {
        if (!openFile(fpStr, dbInpFileStream))
            goto end;
        
        while (dbInpFileStream.good())
        {
            std::getline(dbInpFileStream, lineStr);
#ifdef _DEBUG_
            COUT << "readInputFile : --------------------------------" << ENDL;
#endif
            retVal = parseLineStr(lineStr, DELIMITER_PIPE, dateStr, valStr);
            switch  ( retVal ) 
            {
                case RC_LineEmpty: break ;
                case RC_BadInput:
                {
                    ++lineCnt;
                    if (!isHeaderInvalid(lineCnt, FLAG_ProcParseErr))
                    {
                        std::cerr << COL_RED "Error! bad input => " \
                              << lineStr << COL_DEFAULT << ENDL;
                    }
                    break ;
                }
                default:
                {
                    ++lineCnt;
                    if (processData( dateStr, valStr, valFloat, lineCnt ))
                    {
                        isHeaderInvalid(lineCnt, FLAG_OnOkProcDataAsHeader);
                        COUT << dateStr << " => "
                             << valFloat << " = " 
                             << fetchRate(dateStr) * valFloat << ENDL;
#ifdef _DEBUG_
                        COUT << "readInputFile: fetchRate -> " << fetchRate(dateStr)
                            << ENDL;
#endif
                    }
                }
            }
        }
        dbInpFileStream.close();
    }
    EXCEPTION_HANDLER();
end:
    return ;
}

std::ostream &operator<<(std::ostream &o, BitcoinExchange &btc)
{
    dbType db = btc.getDb();
    dbType::iterator it;

    if (db.size() > 0)
    {
        it = db.begin();
#ifdef _DEBUG_
        COUT << "exchange is ongoing" << ENDL;
#endif // DEBUG
        while (it != db.end())
        {
            COUT << (*it).first << " | " << (*it).second << ENDL;
            it++;
        }
    }
    return (o);
}
