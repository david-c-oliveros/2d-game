#include "game.h"


Game game(1920, 1080);


int main(int argc, const char* argv[])
{
    /*******************************/
    /*        Create Window        */
    /*******************************/

    game.Create();
    game.Start();

    return 0;
}
