// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStats.h"

// Sets default values for this component's properties
UGameStats::UGameStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Lives = 10;
	CurrentLives = 10;

	// ...
}


// Called when the game starts
void UGameStats::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UGameStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

	// ...
}

int32 UGameStats::GetInitialLives()
{
	return Lives;
}

int32 UGameStats::GetCurrentLives()
{
	return CurrentLives;
}

void UGameStats::UpdateCurrentLives(int32 SubtractLives)
{
	if(Level1Started && Level2Started && Level3Started){
		CurrentLives = CurrentLives - SubtractLives;
	}

	if(CurrentLives <= 0){
		Gameover = true;
	}
}


bool UGameStats::GetGameCompleted()
{
	return Level1Completed && Level2Completed && Level3Completed;
}

bool UGameStats::IsGameover()
{
	return Gameover;
}

void UGameStats::SetLevelCompleted(int32 index, bool complete)
{
	if(index == 0)
	{
		Level1Completed = complete;
	} 
	else if(index == 1)
	{
		Level2Completed = complete;
	} 
	else if(index == 2)
	{
		Level3Completed = complete;
	}
}


bool UGameStats::GetLevelCompleted(int32 index)
{
	if(index == 0)
	{
		return Level1Completed;
	} 
	else if(index == 1)
	{
		return Level2Completed;
	} 
	else if(index == 2)
	{
		return Level3Completed;
	}
	return false;
}
