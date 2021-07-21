#include "GeneralAbilityCallback.h"
#include "GeneralAbility.h"


GeneralAbilityCallback* GeneralAbilityCallback::getDeriveInstance()
{
    static GeneralAbility generalAbility;
    return &generalAbility;
}
