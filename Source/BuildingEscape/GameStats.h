// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStats.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGameStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGameStats();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure, Category = "Lives")
	int32 GetInitialLives();

	UFUNCTION(BlueprintPure, Category = "Lives")
	int32 GetCurrentLives();

	UFUNCTION(BlueprintCallable, Category = "Lives")
	void UpdateCurrentLives(int32 Lives);

	UPROPERTY()
	bool Level1Started = false;

	UPROPERTY()
	bool Level2Started = false;

	UPROPERTY()
	bool Level3Started = false;

	UFUNCTION(BlueprintCallable, Category = "GameCompleted")
	bool GetGameCompleted();

	UFUNCTION(BlueprintCallable, Category = "GameCompleted")
	bool IsGameover();

	UFUNCTION(BlueprintCallable, Category = "GameCompleted")
	void SetLevelCompleted(int32 level, bool complete);

	UFUNCTION(BlueprintCallable, Category = "GameCompleted")
	bool GetLevelCompleted(int32 level);
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	// Called every frame
	UPROPERTY(EditAnywhere, Category = "Lives")
	int32 Lives;

	UPROPERTY(EditAnywhere, Category = "Lives")
	int32 CurrentLives;

	UPROPERTY()
	bool Level1Completed = false;

	UPROPERTY()
	bool Level2Completed = false;

	UPROPERTY()
	bool Level3Completed = false;

	UPROPERTY()
	bool Gameover = false;

	
		
};
