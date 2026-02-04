#ifndef TABLE_HDR_H
#define TABLE_HDR_H

#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <iomanip>
#include "./xstring/inc/xstring.hpp"

typedef enum TABLE_HDR__dataType_e
{
    TABLE_HDR__enDataType_Int = 0,
    TABLE_HDR__enDataType_Str,
    TABLE_HDR__enDataType_Max,
}   TABLE_HDR__dataType_ten;


typedef struct TABLE_HDR__col_s
{
     int unsigned            idx;
     int unsigned            maxStrLength;
     std::string             strColText; 
     TABLE_HDR__dataType_ten dataType;
}    TABLE_HDR__col_t;
    



class TableHdr
{
    private:
    public:
        /* attributes  */ 
        char                            chrSeparator;
        std::vector<TABLE_HDR__col_t *> vCols;
        std::vector<TABLE_HDR__col_t *>::iterator itr;

        /* methods */ 
        TableHdr( std::string strHeaderLine, char chrDelimiter );
        ~TableHdr( void );
        void         Show( void );
        int unsigned GetColSize( void ) const;
        int          AddCol(TABLE_HDR__dataType_ten dType, std::string title);
        void         DrawRuler (void);
        std::string  CreateRuler(int nbrOfChr);
        void         UpdateColTextWidth( TABLE_HDR__col_t *pCol, int strlen);
        void         GetColIdxByKeyword( std::vector<int> &selectedColIdxs, std::string &kw );
        int          GetColIdxByTitle( std::string &t );
};



#endif /* TABLE_HDR_H */
