#ifndef TABLE_HPP
#define TABLE_HPP

# include "table_hdr.hpp"
# include "table_body.hpp"
# include "xio.hpp"



class Table 
{
    private:
    public:
        char       chrSeparator;
        TableHdr  *pHeader;
        TableBody *pBody;

        /* methods */
        Table(char const *filePath, int rowIdxHead, int rowIdxBody, char chrDelimiter );
        ~Table(void);
        void  Show(void);
        int   AddDataSet(TABLE_HDR__dataType_ten dtype, std::string title);
        int   GetColIdxByTitle(std::string title, int unsigned &idx);
        int   SumColsValueByIdxToNewCol( std::vector<int> &idxCols,
                                         std::string &ResultColTitle);
        int   ComputeColsValueByIdxToNewCol( std::vector<int> &idxCols,
                                             std::string &ResultColTitle);
                                             int (*algorithm)(int, int);
};



#endif /* TABLE_HPP */ 
