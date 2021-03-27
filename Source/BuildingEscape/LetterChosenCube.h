#pragma once

#include "LetterChosenCube.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class ULetterChosenCube : public UInterface
{
    GENERATED_BODY()
};

class ILetterChosenCube
{    
    GENERATED_BODY()

public:
    /** Add interface function declarations here */
   UFUNCTION(BlueprintNativeEvent)
   bool IsTheRightCube(int32 index);
};