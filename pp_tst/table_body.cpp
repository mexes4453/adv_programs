#include "table_body.hpp"


TableBody::TableBody( std::vector<std::string> vStrFilelines, unsigned int idxLine,
                                                              char chrDelimiter )
{
    int unsigned                        counter = 0;
    TableBody::row_t                   *pvRow = (TableBody::row_t *)0;
    std::vector<std::string>::iterator  vStrItrHead ;
    int retCode = 0;

    if (vStrFilelines.size() > idxLine )
    {
        vStrItrHead = vStrFilelines.begin() + idxLine;
        while ( vStrItrHead != vStrFilelines.end() )
        {
            std::cout << (*vStrItrHead) << std::endl;
            pvRow = new TableBody::row_t;
            retCode = XSTRING__Split( *pvRow, *vStrItrHead, chrDelimiter );
            if (retCode)
            {
                this->StriptWhiteSpaceFromRowContents( *pvRow );
                this->vRows.push_back( pvRow );
            }
            
            vStrItrHead++;
            counter++;
            if (counter == 10) { break; }
        }
    }
}




TableBody::~TableBody( void )
{
    std::cout << "TableBody::destructor" << std::endl;
    this->vRowsItr = (this->vRows).begin();
    while ( this->vRowsItr != (this->vRows).end() )
    {

#ifdef __DEBUG__
        std::cout << "TableBody::destructor - deleting row items" << std::endl;
#endif /* __DEBUG__ */

        delete (*(this->vRowsItr));
        (this->vRowsItr)++;
    }
}


void TableBody::StriptWhiteSpaceFromRowContents( TableBody::row_t &r)
{
    TableBody::row_t::iterator itr = r.begin();
    while (itr != r.end())
    {
        XSTRING__StripWhiteSpace( *itr );
        itr++;
    }
}


void TableBody::Show( void )
{
    this->vRowsItr = (this->vRows).begin();
    TableBody::row_t::iterator  vRowItrHead;
    TableBody::row_t::iterator  vRowItrTail;

    while ( this->vRowsItr != (this->vRows).end() )
    {
        vRowItrHead = (*(this->vRowsItr))->begin();
        vRowItrTail = (*(this->vRowsItr))->end();
        std::cout << "|" ;
        while ( vRowItrHead != vRowItrTail )
        {
             std::cout << *vRowItrHead << "|";
             vRowItrHead++;
        }
        std::cout << std::endl;
        (this->vRowsItr)++;
    }
}



int unsigned TableBody::GetRowSize( void ) const
{
    return (vRows.size());
}



int   TableBody::GetRowCellDataByColIdx(TableBody::row_t &row, int idxCol,
                                                               std::string &data)
{
    int retCode = 1;
    if (idxCol < row.size() )
    {
        data = row[idxCol];
        retCode = 0;
    }
    return (retCode);
}
