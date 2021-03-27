// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BuildingEscapeGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BUILDINGESCAPE_API ABuildingEscapeGameModeBase : public AGameMode
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:
	ABuildingEscapeGameModeBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Lives", Meta= (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> PlayerHUDClass;

	UPROPERTY()
	class UUserWidget* CurrentWidget;
	
};
