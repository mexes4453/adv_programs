#ifndef TAKA_H
#define TAKA_H
# include <string>
# include <iostream>
# include <vector>

typedef std::vector<std::string> vStrTer_t
class Taka 
{
    private:
    public:
        Taka(void);
        ~Taka(void);
        std::string nodeId;
        std::string snHla;
        std::string snChmod;
        std::string nbrSlot;
        std::string nbrChmod;
        vStrTer_t   vStrTerTop;
        vStrTer_t   vStrTerBot;
        void ShowTerSummary(vStrTer_t &vStrTer);
}



#endif /* TAKA_H */
