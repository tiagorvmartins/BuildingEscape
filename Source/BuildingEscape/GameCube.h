// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "MaterialsEnum.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "ChosenCube.h"
#include "GameCube.generated.h"

UCLASS()
class BUILDINGESCAPE_API AGameCube : public AActor, public IChosenCube
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameCube();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	UStaticMeshComponent* CubeMesh = nullptr;

	UPROPERTY()
	TMap<int32, FMaterialsInstances> SelectedMaterialsMap;

	void Init(TMap<int32, FMaterials> MaterialsMap, int32* SumChosen);

	UPROPERTY()
	bool Stationary = true;

	int32 GetTotalSum();

	virtual bool IsTheRightCube_Implementation();

	UPROPERTY()
	bool Moving = true;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	const int32 MaxNumberFace = 9;	
	const int32 MinNumberFace = 0;
	float GetNormalizedDegree(float rotationValue);

	UPROPERTY()
	int32 TotalAmount = 0;	
	
	int32* SumChosen = nullptr;

	UPROPERTY()
	float InitialSpawnTime;
};
