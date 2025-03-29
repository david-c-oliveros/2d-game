#include "game.h"

#include "util.h"


Game game(1920, 1080);


int main(int argc, const char* argv[])
{
    /*******************************/
    /*        Create Window        */
    /*******************************/

//    game.Test();
//    game.OtherTest();

    game.Create();
    game.Start();

    return 0;
}
