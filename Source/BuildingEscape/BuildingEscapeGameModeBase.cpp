// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingEscapeGameModeBase.h"
#include "Blueprint/UserWidget.h"

void ABuildingEscapeGameModeBase::BeginPlay()
{
    Super::BeginPlay();
   
    if(PlayerHUDClass != nullptr)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);

        if(CurrentWidget != nullptr){
            CurrentWidget->AddToViewport();
        }
    }
}

ABuildingEscapeGameModeBase::ABuildingEscapeGameModeBase() : Super()
{

}