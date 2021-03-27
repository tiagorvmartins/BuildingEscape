// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeGeneration.h"
#include "GameCube.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "GameCube.h"
#include "GameStats.h"
#include "NavMesh/NavMeshBoundsVolume.h"
// #include "Engine/Canvas.h"
// #include "Engine/CanvasRenderTarget2D.h"
// #include "Internationalization/Text.h"
#include "MaterialsEnum.h"


// Sets default values for this component's properties
UCubeGeneration::UCubeGeneration()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;	
	InitCubeMaterials();

	UFont* font = nullptr;
	static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("/Game/Fonts/MyFont"));
	if (Font.Succeeded()) {
		font = Font.Object;
	}
	MyFont = font;


	UMaterial* panelMaterial = nullptr;
	static ConstructorHelpers::FObjectFinder<UMaterial> PanelMaterial(TEXT("/Game/Textures/LevelPanel1_Mat"));
	if (PanelMaterial.Succeeded()) {
		panelMaterial = PanelMaterial.Object;
	}
	PanelMaterialDisplay = panelMaterial;	
}

void UCubeGeneration::DrawToCanvasRenderTarget(UCanvas* Canvas, int32 Width, int32 Height)
{
    //Drawing code with UCanvas goes here...
	if(TotalSumFixed > 9){		 
		Canvas->DrawText(MyFont, FText::FromString(FString::Printf(TEXT("%i"), TotalSumFixed)), (Width/2)-390, Height/2-1024/2+100);	 	 
	} else {
		Canvas->DrawText(MyFont, FText::FromString(FString::Printf(TEXT("%i"), TotalSumFixed)), (Width/2)-256, Height/2-1024/2+100);	 	 
	}	 
}

// Called when the game starts
void UCubeGeneration::BeginPlay()
{
	Super::BeginPlay();

	Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if(Pawn){
		GameStats = Pawn->FindComponentByClass<UGameStats>();
	}

	UWorld* world = GetWorld();

	if(GetOwner()){
		PanelMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	}

	if (world) {		
		if (PanelMeshComponent){
			
			CanvasRenderTarget = Cast<UCanvasRenderTarget2D>(UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(world, UCanvasRenderTarget2D::StaticClass(), 1024, 1024));
			CanvasRenderTarget->OnCanvasRenderTargetUpdate.AddDynamic(this, &UCubeGeneration::DrawToCanvasRenderTarget);			

			UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(PanelMaterialDisplay, PanelMeshComponent);
			MaterialInstance->SetTextureParameterValue(FName("TextureMaterial"), CanvasRenderTarget);
			PanelMeshComponent->SetMaterial(0, MaterialInstance);			
			CanvasRenderTarget->UpdateResource();
		}

		UNavigationSystemV1* NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
		
		if (NavigationArea) {
			FNavLocation endPosition;
			
			int32 ChosenCubeIndex = FMath::RandRange(0, 2);
			for(int i = 0; i < 3; i++){
				if(NavMesh){
					if (NavigationArea->GetRandomPointInNavigableRadius(NavMesh->GetActorLocation(), 1.f, endPosition, nullptr, 0)) {											
						bool foundMyCube = false;
						if(ChosenCubeIndex == i)
						{
							foundMyCube = true;						
						}						
									
						FRotator Rotation(0.0f, 0.0f, 0.0f);
						FVector locationObject = endPosition.Location;
						locationObject.Z = locationObject.Z + 2.f; 
						FActorSpawnParameters SpawnInfo;

						AGameCube* MyGameCube = world->SpawnActor<AGameCube>(AGameCube::StaticClass(), locationObject, Rotation, SpawnInfo);			
						MyGameCube->Init(MaterialsMap, &TotalSumFixed);
						if (MyGameCube) {
							if(foundMyCube) {
								TotalSumFixed = MyGameCube->GetTotalSum();
							}

							Cubes.Add(MyGameCube);
						}														
					}
				}
			}
			CanvasRenderTarget->UpdateResource();
		} else {
			UE_LOG(LogTemp, Error, TEXT("Couldn't get nav area!"));
		}
	} else {
		UE_LOG(LogTemp, Error, TEXT("Couldn't get world!"));
	}
	
}




// Called every frame
void UCubeGeneration::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!GameStats->Level1Started){
		bool shouldGameStart = true;
		for(AGameCube* cube : Cubes)
		{			
			if(cube->Moving){
				shouldGameStart = false;
				break;
			}
		}
		GameStats->Level1Started = shouldGameStart;		
	}

	// ...
}

void UCubeGeneration::InitCubeMaterials()
{
	for(int32 i = 0; i <= 9; i++)
	{		
		FString DefaultMaterialIndex = FString::Printf(TEXT("/Game/Textures/CubeMaterials/%i_Default_Mat"), i);
		FString LitMaterialIndex = FString::Printf(TEXT("/Game/Textures/CubeMaterials/%i_Mat"), i);

		UMaterial* MaterialDefault = nullptr;
		ConstructorHelpers::FObjectFinder<UMaterial> MaterialDefaultObj(*DefaultMaterialIndex);
		if (MaterialDefaultObj.Succeeded()) {			
			MaterialDefault = MaterialDefaultObj.Object;
		} else {
			UE_LOG(LogTemp, Warning, TEXT("Didn't found material of cube at %s"), *DefaultMaterialIndex);
		}

		UMaterial* MaterialLit = nullptr;
		ConstructorHelpers::FObjectFinder<UMaterial> MaterialLitObj(*LitMaterialIndex);
		if (MaterialLitObj.Succeeded()) {			
			MaterialLit = MaterialLitObj.Object;
		} else {
			UE_LOG(LogTemp, Warning, TEXT("Didn't found material of cube at %s"), *LitMaterialIndex);
		}

		TArray<UMaterial*> MaterialsArray;
		MaterialsArray.Add(MaterialDefault);
		MaterialsArray.Add(MaterialLit);

		FMaterials CubeIndexMaterials = { MaterialsArray, FString::Printf(TEXT("%i"), i) };
		MaterialsMap.Add(i, CubeIndexMaterials);
	}
}
