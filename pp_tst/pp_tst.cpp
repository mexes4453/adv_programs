
#include "pp_tst.hpp"

PostProcessorTst::PostProcessorTst(void) {}

PostProcessorTst::PostProcessorTst(char const *fp)
{
    COUT << fp << ENDL;
    pResultTab = new Table( fp, PP_TST__HEADER_ROW_IDX, PP_TST__BODY_ROW_IDX, ',' );
    pResultTab->Show();
    this->ComputeTop();
    this->ComputeBot();
    this->ComputeTerTop();
    this->ComputeTerBot();
    pResultTab->Show();
}


PostProcessorTst::PostProcessorTst(PostProcessorTst const &obj)
{
    if (&obj)
    {
        // do nothing
    }
    // db = obj.db;
}




PostProcessorTst::~PostProcessorTst(void) 
{
    if (pResultTab)
    {
        delete pResultTab;
    }
}



void PostProcessorTst::ComputeTop( void )
{
    std::vector<int>  vHdrIdx;
    std::string       newColTitle = "top";
    std::string       colTitle("");
    int               colIdx = 0;
    std::vector<std::string> colTitles;
    colTitles.push_back( "\"Top SkMacCrc8Err Datapath\"");
    colTitles.push_back( "\"Top SkMacCrc8Err FIF main\"");
    colTitles.push_back( "\"Top SkMacCrc8Err FIF aux\"");
    colTitles.push_back( "\"Top SkSbCrcErrCount\"");
    colTitles.push_back( "\"Top SkSb1bitFixCnt\"");
    colTitles.push_back( "\"Top SkSb2bitFixCnt\"");
    colTitles.push_back( "\"Top SkSbEcFailCnt\"" );

    // create the vector to store selected col idx from the header to sum up in rows. 
    /* fetch the data for interval */
    std::vector<std::string>::iterator vStrItr = colTitles.begin();
    while ( vStrItr != colTitles.end() )
    {
        colTitle = *vStrItr;
        colIdx = this->pResultTab->pHeader->GetColIdxByTitle( colTitle ); 
        if (colIdx < 0) goto escape;
        vHdrIdx.push_back( colIdx );
        std::cout << "colTitle: " << colTitle << ": " << colIdx << std::endl;
        vStrItr++;
    }

    // create the vector to store selected col idx from the header to sum up in rows. 
    this->pResultTab->SumColsValueByIdxToNewCol( vHdrIdx, newColTitle);

escape:
    return ;
}




void PostProcessorTst::ComputeBot( void )
{
    std::vector<int>  vHdrIdx;
    std::string       newColTitle = "bot";
    std::string       colTitle("");
    int               colIdx;
    std::vector<std::string> colTitles;
    colTitles.push_back( "\"Bot SkMacCrc8Err Datapath\"");
    colTitles.push_back( "\"Bot SkSbCrcErrCnt\"");
    colTitles.push_back( "\"Bot SkSb1bitFixCnt\"");
    colTitles.push_back( "\"Bot SkSb2bitFixCnt\"");
    colTitles.push_back( "\"Bot SkSbEcFailCnt\"");

    // create the vector to store selected col idx from the header to sum up in rows. 
    /* fetch the data for interval */
    std::vector<std::string>::iterator vStrItr = colTitles.begin();
    while ( vStrItr != colTitles.end() )
    {
        colTitle = *vStrItr;
        colIdx = this->pResultTab->pHeader->GetColIdxByTitle( colTitle ); 
        if (colIdx < 0) goto escape;
        vHdrIdx.push_back( colIdx );
        vStrItr++;
    }
    this->pResultTab->SumColsValueByIdxToNewCol( vHdrIdx, newColTitle);

escape:
    return ;
}



int  PostProcessorTst::ComputeTer( int interval, int sumValueTopBot)
{
    int result = 0;
    int valueDefault = -15;
    double valComputed = 0.0;
    
#if 1
    std::cout << "int( " << interval << " ); "
              << "sum( " << sumValueTopBot  << " ); "
              << "const( " << PP_TST__CONST_VAL  << " ); ";
    if ( sumValueTopBot )
    {
        valComputed = static_cast<double>(interval) * PP_TST__CONST_VAL;
        valComputed = static_cast<double>(sumValueTopBot) / valComputed;
        std::cout << "algebra ( " << valComputed  << " ); ";
        valComputed = std::log10(valComputed);
        std::cout << "log10 ( " << valComputed  << " ); ";
        result = static_cast<int>( valComputed );
        std::cout << "result ( " << result  << " ); " << std::endl;
    }
    else 
    {
        result = valueDefault;
    }
#else
    result = valueDefault + valueComputed;
#endif
    return (result);
}




int PostProcessorTst::ComputeTerBot( void )
{
    int                        retCode = 1;
    std::string                dataStr;
    std::stringstream          oss;

    int unsigned               idxPosInterval;
    int unsigned               idxPosTop;     
    int unsigned long          valInterval = 0;
    int unsigned long          valTop = 0;
    int                        result = 0;
    std::string                ResultColTitle = "botTer";

    this->pResultTab->AddDataSet( TABLE_HDR__enDataType_Int, ResultColTitle);
    
    /* fetch the column index for interval and top */
    this->pResultTab->GetColIdxByTitle( "Interval", idxPosInterval);
    this->pResultTab->GetColIdxByTitle( "bot", idxPosTop);
    
    /* Fetch the value stored in the row and cell */
    this->pResultTab->pBody->vRowsItr = this->pResultTab->pBody->vRows.begin();
    while ( this->pResultTab->pBody->vRowsItr != this->pResultTab->pBody->vRows.end())
    {
        /* fetch the data for interval */
        retCode = this->pResultTab->pBody->GetRowCellDataByColIdx(
                                              *(*(this->pResultTab->pBody->vRowsItr)),
                                              idxPosInterval,
                                              dataStr);

        if (retCode != 0) goto escape;

#ifdef __DEBUG__
        std::cout << dataStr << valInterval << "|" << idxPosInterval << std::endl;
#endif /* __DEBUG__ */

        retCode = XSTRING__ConvToUnsignedLongInt(dataStr, valInterval);
        if (retCode != 0) goto escape;

        /* fetch the data for top */
        retCode = this->pResultTab->pBody->GetRowCellDataByColIdx(
                                              *(*(this->pResultTab->pBody->vRowsItr)),
                                              idxPosTop,
                                              dataStr);
        if (retCode != 0) goto escape;
        std::cout << dataStr << valTop << "|" << idxPosTop << std::endl;

        retCode = XSTRING__ConvToUnsignedLongInt(dataStr, valTop);
        if (retCode != 0) goto escape;
         


#ifdef __DEBUG__
            std::cout << "|" << dataStr << "-> sum: " << result << std::endl;
#endif /* __DEBUG__ */

        result = this->ComputeTer(valInterval, valTop);
        oss << result;
        std::cout << "result" << oss.str() << std::endl;
        (*(this->pResultTab->pBody->vRowsItr))->pop_back();
        (*(this->pResultTab->pBody->vRowsItr))->push_back( oss.str() );
        oss.str(""); oss.clear();
        (this->pResultTab->pBody->vRowsItr)++;
        result = 0; /* reset the result value */
    }

escape:
    return (retCode);
}




int   PostProcessorTst::ComputeTerTop( void )
{
    int                        retCode = 1;
    std::string                dataStr;
    std::stringstream          oss;

    int unsigned               idxPosInterval;
    int unsigned               idxPosTop;     
    int unsigned long          valInterval = 0;
    int unsigned long          valTop = 0;
    int                        result = 0;
    std::string                ResultColTitle = "topTer";

    this->pResultTab->AddDataSet( TABLE_HDR__enDataType_Int, ResultColTitle);
    
    /* fetch the column index for interval and top */
    this->pResultTab->GetColIdxByTitle( "Interval", idxPosInterval);
    this->pResultTab->GetColIdxByTitle( "top", idxPosTop);
    
    /* Fetch the value stored in the row and cell */
    this->pResultTab->pBody->vRowsItr = this->pResultTab->pBody->vRows.begin();
    while ( this->pResultTab->pBody->vRowsItr != this->pResultTab->pBody->vRows.end())
    {
        /* fetch the data for interval */
        retCode = this->pResultTab->pBody->GetRowCellDataByColIdx(
                                              *(*(this->pResultTab->pBody->vRowsItr)),
                                              idxPosInterval,
                                              dataStr);

        if (retCode != 0) goto escape;

#ifdef __DEBUG__
        std::cout << dataStr << valInterval << "|" << idxPosInterval << std::endl;
#endif /* __DEBUG__ */

        retCode = XSTRING__ConvToUnsignedLongInt(dataStr, valInterval);
        if (retCode != 0) goto escape;

        /* fetch the data for top */
        retCode = this->pResultTab->pBody->GetRowCellDataByColIdx(
                                              *(*(this->pResultTab->pBody->vRowsItr)),
                                              idxPosTop,
                                              dataStr);
        if (retCode != 0) goto escape;
        std::cout << dataStr << valTop << "|" << idxPosTop << std::endl;

        retCode = XSTRING__ConvToUnsignedLongInt(dataStr, valTop);
        if (retCode != 0) goto escape;
         


#ifdef __DEBUG__
            std::cout << "|" << dataStr << "-> sum: " << result << std::endl;
#endif /* __DEBUG__ */

        result = this->ComputeTer(valInterval, valTop);
        oss << result;
        std::cout << "result" << oss.str() << std::endl;
        (*(this->pResultTab->pBody->vRowsItr))->pop_back();
        (*(this->pResultTab->pBody->vRowsItr))->push_back( oss.str() );
        oss.str(""); oss.clear();
        (this->pResultTab->pBody->vRowsItr)++;
        result = 0; /* reset the result value */
    }
escape:
    return (retCode);
}


