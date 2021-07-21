#include "Spdlogger.hpp"
#include "CentralPMCallback.h"

int main(int argc, char* argv[])
{
    LOGINFO(emModuleId::Main, "***************progress starting***************");


    auto cpmPtr = CentralPMCallback::getDeriveInstance();
    cpmPtr->init();
    cpmPtr->run();
    cpmPtr->loop();

    return 0;
}
