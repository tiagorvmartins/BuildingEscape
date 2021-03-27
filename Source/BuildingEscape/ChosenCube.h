#pragma once

#include "ChosenCube.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UChosenCube : public UInterface
{
    GENERATED_BODY()
};

class IChosenCube
{    
    GENERATED_BODY()

public:
    /** Add interface function declarations here */
   UFUNCTION(BlueprintNativeEvent)
   bool IsTheRightCube();
};