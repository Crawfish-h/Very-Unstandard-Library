#include "TIterator.h"
#include "TContainer.h"

TIterator It_Begin(TContainer* container)
{
    return container->C_It_Begin(container);
}

TIterator It_At(TContainer* container, size_t index)
{
    return container->C_It_At(container, index);
}

TIterator It_End(TContainer* container)
{
    return container->C_It_End(container);
}

void It_Next(TContainer* container, TIterator* it)
{
    container->C_It_Next(container, it);
}

bool It_Cmp(TContainer* container, TIterator* it_0, TIterator* it_1)
{
    return container->C_It_Cmp(container, it_0, it_1);
}