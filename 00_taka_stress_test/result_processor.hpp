/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cudoh <cudoh@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:40:42 by cudoh             #+#    #+#             */
/*   Updated: 2023/05/21 16:40:42 by cudoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP
# include <map>
# include <iostream>
# include <stdexcept>
# include <fstream>
# include <cstdlib>
# include <sstream>
# include <string>
# include <cstring>
# include <utility>
# include <ctime>
# include <algorithm>

# define COUT std::cout
# define ENDL std::endl
# define COL_BLUE "\033[0;34m"
# define COL_RED "\033[0;31m"
# define COL_YELLOW "\033[0;33m"
# define COL_MAGENTA "\033[0;35m"
# define COL_DEFAULT "\033[0m"

# define FILE_FMT_CSV (".csv")
# define FILE_FMT_TXT (".txt")
# define FILE_DB ("data.csv")
//# define FILE_DB ("data_test_01.csv")
//# define FILE_DB ("data_test_02.csv")
//# define FILE_DB ("data_test_03.csv")
//# define FILE_DB ("data_test_04.csv")
//# define FILE_DB ("data_test_05.csv")
//# define FILE_DB ("data_test_06.csv")
//# define FILE_DB ("data_test_07_emp.csv")

# define FILE_DB_HDR_1 ("data")
# define FILE_DB_HDR_2 ("exchange")
# define ERR_MSG_NoInputFile        COL_RED "Error: could not open file." COL_DEFAULT
# define ERR_MSG_ToManyArgs         COL_RED "Error: Too many args." COL_DEFAULT
# define ERR_MSG_InvalidFileFmt     COL_RED "Error: Invalid file format." COL_DEFAULT
# define ERR_MSG_InvalidLineFmt     COL_RED "Error: Invalid line format." COL_DEFAULT
# define ERR_MSG_FileOpenFailed     COL_RED "Error: could not open file" COL_DEFAULT
# define ERR_MSG_InvalidFloat       COL_RED "Error: Invalid format or corrupt float found" COL_DEFAULT
# define ERR_MSG_InvalidFmtSpace    COL_RED "Error: Invalid format - space found" COL_DEFAULT
# define ERR_MSG_InvalidFmtBadChar  COL_RED "Error: Invalid format - invalid char found" COL_DEFAULT
# define ERR_MSG_ValueTooHigh       COL_RED "Error: too large a number" COL_DEFAULT
# define ERR_MSG_ValueTooLow        COL_RED "Error: not a positive number." COL_DEFAULT
# define ERR_MSG_BadInput           COL_RED "Error: bad input" COL_DEFAULT
# define ERR_MSG_NegativeDateComp   COL_RED "Error: Invalid date entry" COL_DEFAULT
# define ERR_MSG_DateInFuture       COL_RED "Error: Date is in future" COL_DEFAULT


# define SHOW_HEADER(MSG) COUT << ENDL << COL_YELLOW "== "#MSG" ==" COL_DEFAULT << ENDL; 
# define EXCEPTION_HANDLER()            \
    catch (const std::exception &e)     \
    {\
        std::cerr << e.what() << '\n'; \
    }\

# define DELIMITER_COMMA (",")
# define DELIMITER_HYPHEN ("-")
# define DELIMITER_PIPE ("|")
# define CHARS_WHITESPACE ("\n\t\v\f\r ")
# define CHARS_VALID_FLOAT ("-+.0123456789Ee")
# define CHARS_DIGITS ("0123456789")
# define MAX_VALUE (1000)
# define REGEX_PAT_DATE ("^\\d{4}-\\d{2}-\\{2}$")

typedef enum eRetCode
{
    RC_LineEmpty = -127,
    RC_BadInput,
    RC_Success = 0
}   tRetCode;

typedef enum eFlag
{
    FLAG_OnOkProcDataAsHeader = 1,
    FLAG_OnErrProcDataAsHeader,
    FLAG_ProcParseErr,
    FLAG_DoNothingOnErr
}   tFlag;

typedef std::map<std::string, float>    dbType;
typedef std::pair<std::string, float>   pairType;
class BitcoinExchange
{
    private:
        dbType      db;
        bool        dbFlag;
        bool        isFileFormatValid(std::string const &fpStr);
        bool        isFileOpen(char const *fp, std::ifstream &ifs);
        bool        isFileEmpty(std::ifstream &ifs);
        bool        isFloatValid(std::string &valueStr, int unsigned &lineCnt);
        bool        isDateValid(std::string &Str);
        void        stripWhiteSpace(std::string &str);
        bool        isSpaceInStr(std::string const &str);
        int         convertMonthStrToInt(std::string &str);
        void        validateDateValue(std::stringstream &ss, 
                    int &nm, int &ny, int &nd);
        bool        isLineEmpty(std::string const &lineStr);
        bool        isHeaderInvalid(
                                  int unsigned &lineCnt,
                                  tFlag flg);
        tRetCode    parseLineStr(std::string const &lineStr,  
                             std::string const &delimter, 
                             std::string &dateStr,  
                             std::string &valStr);
        bool        processData( std::string &dateStr, 
                             std::string &valStr, float &valFloat,  
                             int unsigned &lineCnt );
        bool        openFile(std::string const &fpStr, std::ifstream &dbInpFileStream);
        float       fetchRate(std::string const &dbKeyDate);
        BitcoinExchange(void);

    public:
        BitcoinExchange(char const *fp);
        BitcoinExchange(BitcoinExchange const &obj);
        BitcoinExchange &operator=(BitcoinExchange const &obj);
        ~BitcoinExchange(void);
        dbType  &getDb(void);
        void    readDbFile(void);
        void    readInputFile(char const *fp);
};

std::ostream    &operator<<(std::ostream &o, BitcoinExchange &btc);

#endif  //BITCOINEXCHANGE_HPP
