#include "table.hpp"

Table::Table(char const *filePath, int rowIdxHead, int rowIdxBody, char chrDelimiter )
: chrSeparator('|')
{
    int                      retCode = 1;
    std::vector<std::string> filelines;

    retCode = XIO__GetFilelines( filePath, filelines );
    if (retCode == 0)
    {
        this->pHeader = new TableHdr(filelines[rowIdxHead], chrDelimiter);
        this->pBody = new TableBody(filelines, rowIdxBody, chrDelimiter);
        this->pHeader->Show();
        this->pBody->Show();
        this->Show();
    }
}




Table::~Table(void)
{
    std::cout << "Table: destructor" << std::endl;
    delete pHeader;
    delete pBody;
}





void Table::Show( void )
{
#ifdef __DEBUG__
    std::cout << "Nbr of rows: " << this->pBody->GetRowSize() << std::endl 
              << "Nbr of cols: " << this->pHeader->GetColSize() << std::endl;

#endif /* __DEBUG__ */
    this->pHeader->Show();
    this->pBody->Show();
}



int  Table::AddDataSet(TABLE_HDR__dataType_ten dtype, std::string title)
{
    int         retCode = this->pHeader->AddCol(dtype, title);
    std::string data = "x";

    /* add entries to body for rows */
    if (retCode == 0)
    {
        pBody->vRowsItr = pBody->vRows.begin();
        while (pBody->vRowsItr != pBody->vRows.end())
        {
            (*(pBody->vRowsItr))->push_back( data );
            ((pBody->vRowsItr)++);
        }
    }
    return (retCode);
}



int Table::GetColIdxByTitle(std::string title, int unsigned &idx)
{
    int                                       retCode = 1;
    std::vector<TABLE_HDR__col_t *>::iterator itr = this->pHeader->vCols.begin();

    while ( itr != this->pHeader->vCols.end())
    {
        if ((*itr)->strColText == title)
        {
            idx = (*itr)->idx;
            retCode = 0;
            break ;
        }
        itr++;
    }
   

    return (retCode);
    
}



int  Table::SumColsValueByIdxToNewCol( std::vector<int> &idxCols, std::string &ResultColTitle)
{
    //int sumResult = 0;
    int                        retCode = 1;
    std::string                dataStr;
    std::vector<int>::iterator itrIdxCol;
    int unsigned long          result = 0;
    int unsigned long          intValue = 0;
    int unsigned               idxResultCol;
    std::stringstream          oss;

    this->AddDataSet( TABLE_HDR__enDataType_Int, ResultColTitle);

    this->pBody->vRowsItr = this->pBody->vRows.begin();
    while ( this->pBody->vRowsItr != this->pBody->vRows.end())
    {
        itrIdxCol = idxCols.begin();
        while( itrIdxCol != idxCols.end())
        {
            retCode = this->pBody->GetRowCellDataByColIdx(*(*(this->pBody->vRowsItr)),
                                                          *itrIdxCol,
                                                           dataStr);
            if (retCode != 0) goto escape;
            retCode = XSTRING__ConvToUnsignedLongInt(dataStr, intValue);
            if (retCode != 0) goto escape;
            result += intValue;
#ifdef __DEBUG__
            std::cout << "|" << dataStr << "-> sum: " << result << std::endl;
#endif /* __DEBUG__ */
            dataStr.clear();
            itrIdxCol++;
        }
        retCode = GetColIdxByTitle( ResultColTitle, idxResultCol);
        if (retCode != 0) goto escape;
        oss << result;
        (*(this->pBody->vRowsItr))->pop_back();
        (*(this->pBody->vRowsItr))->push_back( oss.str() );
        oss.str("");
        oss.clear();
        (this->pBody->vRowsItr)++;
        result = 0; /* reset the result value */
        intValue = 0;
    }
escape:
    return (retCode);
}


