#include "StageProgress.h"

int StageProgress::getStage() const
{
    return stage;
}

int StageProgress::getFlags() const
{
    return flags;
}

void StageProgress::collectFlag()
{
    flags++;
    stage++;
}

void StageProgress::reset()
{
    stage = 1;
    flags = 0;
}
