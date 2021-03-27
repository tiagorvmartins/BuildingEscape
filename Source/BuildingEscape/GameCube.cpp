// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCube.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/Object.h"
#include "ChosenCube.h"
#include "MaterialsEnum.h"
#include "Materials/Material.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
AGameCube::AGameCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UStaticMesh* cubeMesh = nullptr;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Game/Meshes/GameCube"));
	if (CubeMeshAsset.Succeeded()) {
		cubeMesh = CubeMeshAsset.Object;
	}

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GameCube"), false);
	if(CubeMesh && cubeMesh) {
		CubeMesh->SetStaticMesh(cubeMesh);
	}

	CubeMesh->SetSimulatePhysics(true);
	CubeMesh->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	CubeMesh->SetWorldScale3D(FVector(1.f,1.f,1.f));

	SetRootComponent(CubeMesh);

}

void AGameCube::Init(TMap<int32, FMaterials> MaterialsMap, int32* Sum)
{	
	SumChosen = Sum;	
	for(int32 i = 0; i <= 5; i++)
	{
		int32 FaceNumber = FMath::RandRange(MinNumberFace, MaxNumberFace);

		UMaterial* p = MaterialsMap[FaceNumber].MaterialEntries[0];		
		UMaterialInstanceDynamic* DynamicMaterialDefault = UMaterialInstanceDynamic::Create(p, CubeMesh);	
		CubeMesh->SetMaterial(i, DynamicMaterialDefault);
		UMaterialInstanceDynamic* DynamicMaterialLit = UMaterialInstanceDynamic::Create(MaterialsMap[FaceNumber].MaterialEntries[1], CubeMesh);		

		TotalAmount += FaceNumber;

		TArray<UMaterialInstanceDynamic*> MaterialsUsed;
		MaterialsUsed.Add(DynamicMaterialDefault);
		MaterialsUsed.Add(DynamicMaterialLit);
		
		FMaterialsInstances CubeIndexMaterialInstances = { MaterialsUsed, FaceNumber };

		SelectedMaterialsMap.Add(i, CubeIndexMaterialInstances);
	}
}

bool AGameCube::IsTheRightCube_Implementation()
{
	if(SumChosen){
		return TotalAmount == *SumChosen;
	}
	return false;
	
}

int32 AGameCube::GetTotalSum()
{
	return TotalAmount;
}

// Called when the game starts or when spawned
void AGameCube::BeginPlay()
{
	Super::BeginPlay();
	InitialSpawnTime = GetWorld()->GetTimeSeconds();
}

float AGameCube::GetNormalizedDegree(float rotationValue)
{
	float normalizedDegree = 0.f;
  	// x between 45° and 135° ( ~ 90)
	if (rotationValue > 45.f && rotationValue < 135.f) {
		normalizedDegree = 90.f;		
	}
	// x between -45° and -135° ( ~ -90)
	else if (rotationValue < -45.f && rotationValue > -135.f) {
		normalizedDegree = -90.f;
	}
	// x between 135° and 215° or x between -135° and -215° ( ~ 180)
	else if ((rotationValue > 135.f && rotationValue < 215.f) || (rotationValue < -135.f && rotationValue > -215.f)) {
		normalizedDegree = 180.f;
	}

	return normalizedDegree;
}

// Called every frame
void AGameCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector velocity = CubeMesh->GetComponentVelocity();
	
	if(GetWorld()->GetTimeSeconds() - InitialSpawnTime > 5.f && velocity.X == 0.f && velocity.Y == 0.f && velocity.Z == 0.f ){
		Moving = false;		
	}
}

