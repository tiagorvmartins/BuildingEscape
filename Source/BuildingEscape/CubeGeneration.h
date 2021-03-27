// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"
#include "Materials/Material.h"
#include "MaterialsEnum.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Pawn.h"
#include "GameCube.h"
#include "GameStats.h"
#include "Engine/Canvas.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Internationalization/Text.h"
#include "CubeGeneration.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UCubeGeneration : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCubeGeneration();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TMap<int32, FMaterials> MaterialsMap;

	
	UPROPERTY()
	UFont* MyFont = nullptr;

	UPROPERTY()
	UMaterial* PanelMaterialDisplay = nullptr;

	UPROPERTY()
	UStaticMeshComponent* PanelMeshComponent = nullptr;

	void InitCubeMaterials();

	UFUNCTION()
	void DrawToCanvasRenderTarget(UCanvas* Canvas, int32 Width, int32 Height);

	UPROPERTY()
	UCanvasRenderTarget2D* CanvasRenderTarget = nullptr;

	
	int32* TotalSum = nullptr;	
	UPROPERTY()
	int32 TotalSumFixed;	

	UPROPERTY(EditAnywhere)
	ANavMeshBoundsVolume* NavMesh = nullptr;	

	UPROPERTY()
	TArray<AGameCube*> Cubes;
	
	UPROPERTY()
	UGameStats* GameStats = nullptr;

	UPROPERTY()
	APawn* Pawn = nullptr;
};
