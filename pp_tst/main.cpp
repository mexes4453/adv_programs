
#include "pp_tst.hpp"

int main(int argc, char *argv[])
{
    PostProcessorTst *ppTst;

    try
    {
        if (argc == 1)      // check if user did not provide argument
        {
            throw std::runtime_error(ERR_MSG_NoInputFile);
        }
        else if (argc == 2) // check if user provided only one argument
        {
            ppTst = new PostProcessorTst(argv[1]);
            delete ppTst;
        }
        else                // user provided more than one argument
        {
            throw std::runtime_error(ERR_MSG_ToManyArgs);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return (0);
}
