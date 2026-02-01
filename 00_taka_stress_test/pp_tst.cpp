
#include "pp_tst.hpp"

PostProcessorTst::PostProcessorTst(void) {}

PostProcessorTst::PostProcessorTst(char const *fp)
{
    COUT << fp << ENDL;
    pResultTab = new Table( fp, PP_TST__HEADER_ROW_IDX, PP_TST__BODY_ROW_IDX, ',' );
    pResultTab->Show();
    this->ComputeTop();
    this->ComputeBot();
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
    std::string       kwInColTitle = "Top";

    // create the vector to store selected col idx from the header to sum up in rows. 
    this->pResultTab->pHeader->GetColIdxByKeyword( vHdrIdx, kwInColTitle);
    this->pResultTab->SumColsValueByIdxToNewCol( vHdrIdx, newColTitle);

}




void PostProcessorTst::ComputeBot( void )
{
    std::vector<int>  vHdrIdx;
    std::string       newColTitle = "bot";
    std::string       kwInColTitle = "Bot";

    // create the vector to store selected col idx from the header to sum up in rows. 
    this->pResultTab->pHeader->GetColIdxByKeyword( vHdrIdx, kwInColTitle);
    this->pResultTab->SumColsValueByIdxToNewCol( vHdrIdx, newColTitle);
}



int  PostProcessorTst::ComputeAlgorthmForTer( int time, int valueTopBot)
{
    return (time);
}

void PostProcessorTst::ComputeTerTop( void )
{

}



void PostProcessorTst::ComputeTerBot( void )
{

}
