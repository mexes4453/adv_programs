
#ifndef TST_PROCESSOR_HPP
# define TST_PROCESSOR_HPP
# include <stdexcept>
# include <fstream>
# include <cstdlib>
# include <sstream>
# include <cstring>
# include <string>
# include <algorithm>
# include <cmath>
# include "table.hpp"




# define COUT std::cout
# define ENDL std::endl
# define COL_BLUE "\033[0;34m"
# define COL_RED "\033[0;31m"
# define COL_YELLOW "\033[0;33m"
# define COL_MAGENTA "\033[0;35m"
# define COL_DEFAULT "\033[0m"

# define _DEBUG_ (1)

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
# define PP_TST__HEADER_ROW_IDX (0)
# define PP_TST__BODY_ROW_IDX   (1)
# define PP_TST__CONST_VAL      (10312500000)

typedef enum eRetCode
{
    RC_LineEmpty = -127,
    RC_BadInput,
    RC_Success = 0
}   tRetCode;



class PostProcessorTst
{
    private:
        PostProcessorTst(void);

    public:
        Table *pResultTab;
        PostProcessorTst(char const *fp);
        PostProcessorTst(PostProcessorTst const &obj);
        ~PostProcessorTst(void);
        void ComputeTop( void );
        void ComputeBot( void );
        int  ComputeTer( int interval, int sumValueTopBot);
        int  ComputeTerTop( void );
        int  ComputeTerBot( void );
        /* Perform mapping : node to tester model file id */
        /* Perform mapping : node to edc_pon_code - hla sn nbr */
        /* Perform mapping : node to edc_pon_code - channel module  */
        /* Perform mapping : node to (edc_pon_code / tester model file) - tester slot  */
        /* Perform mapping : node to */
        /* check reportwindow for errors */
        /* generate the failure labels */
};


#endif  //TST_PROCESSOR_HPP
