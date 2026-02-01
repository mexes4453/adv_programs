#ifndef TABLE_BODY_HPP
#define TABLE_BODY_HPP

#include <vector>
#include <string>
#include <iostream>
#include "./xstring/inc/xstring.hpp"


#if 0
typedef struct TABLE_BODY__row_s
{
    std::vector<std::string> row;
    
}   TABLE_BODY__row_t;
#endif


class TableBody
{
    private:
    public:
        typedef std::vector<std::string> row_t;
        std::vector<row_t *> vRows;
        std::vector<row_t *>::iterator vRowsItr;
       
        TableBody( std::vector<std::string> vStrFilelines, unsigned int idxLine,  
                                                           char         chrDelimiter );
        ~TableBody( void );
        void StriptWhiteSpaceFromRowContents( TableBody::row_t &r);
        void Show( void );
        int unsigned GetRowSize( void ) const;
        int          GetRowCellDataByColIdx(TableBody::row_t &row, int idxCol,
                                                                   std::string &data);
        
};




#endif /* TABLE_BODY_HPP */
