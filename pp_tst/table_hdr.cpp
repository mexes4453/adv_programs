#include "table_hdr.hpp"



TableHdr::TableHdr( std::string strHeaderLine, char chrDelimiter): chrSeparator('|')
{
    
    std::vector<std::string>::iterator  head, tail;
    int                                 idxValue = 0;
    TABLE_HDR__col_t                   *pHdrColumn = (TABLE_HDR__col_t *)0; /* null ptr */
    std::vector<std::string>            vStrHeaderColumns;
    int                                 retCode = 0;

    /* split the line according to the given delimiter character  */
    retCode = XSTRING__Split( vStrHeaderColumns, strHeaderLine, chrDelimiter );
    std::string strTest = "";
    
    /* check status of return code here */
    if ( retCode )
    {
        head = vStrHeaderColumns.begin();
        tail = vStrHeaderColumns.end();
        
        while (head != tail)
        {
            retCode = this->AddCol(TABLE_HDR__enDataType_Str, *head);
            if (retCode) { break ;}
            idxValue++;
            head++;
        }
    }
}



TableHdr::~TableHdr(void) 
{
    
    std::cout << "TableHdr::destructor" << std::endl;
    this->itr = (this->vCols).begin();
    while ( this->itr != (this->vCols).end() )
    {

#ifdef __DEBUG__
        std::cout << "TableHdr::destructor - deleting column item" << std::endl;
#endif /* __DEBUG__ */

        delete (*(this->itr));
        (this->itr)++;
    }
}




void TableHdr::Show(void) 
{

    this->itr = (this->vCols).begin();
#ifdef __DEBUG__
    while ( this->itr != (this->vCols).end() )
    {
        std::cout << (*(this->itr))->idx << 
              " " << chrSeparator << (*(this->itr))->strColText <<
              " " << chrSeparator << (*(this->itr))->maxStrLength <<
              " " << chrSeparator << (*(this->itr))->dataType << std::endl;

        (this->itr)++;
    }
#endif /* __DEBUG__ */

    DrawRuler();
    this->itr = (this->vCols).begin();

    std::cout << this->chrSeparator;
    while ( this->itr != (this->vCols).end() )
    {
        std::cout << std::setw((*(this->itr))->maxStrLength) 
                  << (*(this->itr))->strColText
                  << this->chrSeparator;
        (this->itr)++;
    }
    std::cout << std::endl;
    DrawRuler();
}



std::string  TableHdr::CreateRuler(int nbrOfChr)
{
    std::ostringstream oss;
    char               chr = '-';
    for (int idx =0; idx < nbrOfChr; idx++)
    {
        oss << chr;
    }
    return (oss.str());
}




void   TableHdr::DrawRuler( void )
{

    std::ostringstream oss;
    char               chr = '+';

    this->itr = (this->vCols).begin();
    oss << chr;
    while ( this->itr != (this->vCols).end() )
    {
        oss << CreateRuler((*(this->itr))->maxStrLength) << chr;
        (this->itr)++;
    }
    std::cout << oss.str() << std::endl;
}


int unsigned TableHdr::GetColSize( void ) const
{
    return (vCols.size());
}



int  TableHdr::AddCol(TABLE_HDR__dataType_ten dType, std::string title)
{
    int retCode = 1;

    TABLE_HDR__col_t *pHdrCol = new TABLE_HDR__col_t;
    if (pHdrCol)
    {
            pHdrCol->idx = this->vCols.size();
            pHdrCol->dataType = dType;
            pHdrCol->strColText = title;
            XSTRING__StripWhiteSpace(pHdrCol->strColText);
            pHdrCol->maxStrLength = pHdrCol->strColText.size();
            (this->vCols).push_back( pHdrCol );
            retCode = 0;
    }
    return (retCode);
}


void  TableHdr::UpdateColTextWidth( TABLE_HDR__col_t *pCol, int strlen)
{
    if (pCol)
    {
        pCol->maxStrLength = (pCol->maxStrLength > strlen) ? pCol->maxStrLength : strlen;
    }
}


void  TableHdr::GetColIdxByKeyword( std::vector<int> &selectedColIdxs, std::string &kw )
{
    char  *pHaystackStr;
    char  *pChr = (char *)0; /* null */

    this->itr = vCols.begin();
    while ( this->itr != this->vCols.end())
    {
        pHaystackStr = &(((*(this->itr))->strColText)[0]);
        pChr = strstr( pHaystackStr, kw.c_str() );
        if (pChr)
        {
            selectedColIdxs.push_back( (*(this->itr))->idx );

#ifdef __DEBUG__
            std::cout << (*(this->itr))->idx << "-" << pHaystackStr << std::endl;
#endif /* DEBUG */

        }
        (this->itr)++;
    }
#ifdef __DEBUG__
    for (int x=0; x < vHdrIdx.size(); x++)
    {
         std::cout << "(" << vHdrIdx[x] << ") - " ;
    }
    std::cout << std::endl;
#endif /* DEBUG */

}



int TableHdr::GetColIdxByTitle( std::string &t )
{
    int idx = -1;
    this->itr = this->vCols.begin();

    while (this->itr != this->vCols.end())
    {
#ifdef __DEBUG__
        std::cout << t << ": " << (*(this->itr))->strColText << std::endl;
#endif /* DEBUG */
        if (t == (*(this->itr))->strColText )
        {
            idx = (*(this->itr))->idx;
            break ;
        }
        (this->itr)++;
    }
#ifdef __DEBUG__
    std::cout << "index: " << idx << std::endl;
#endif /* DEBUG */
    return (idx);
}
