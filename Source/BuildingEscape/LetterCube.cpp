// Fill out your copyright notice in the Description page of Project Settings.


#include "LetterCube.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Engine/Canvas.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Internationalization/Text.h"
#include "MaterialsEnum.h"
#include "CanvasItem.h"
#include "Engine/Texture.h"
#include "Materials/Material.h"

// Sets default values
ALetterCube::ALetterCube()
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

	
	static ConstructorHelpers::FObjectFinder<UMaterial> letterMaterial(TEXT("/Game/Textures/CubeMaterials/LetterCube_Mat"));
	if (letterMaterial.Succeeded()) {
		LetterMaterial = letterMaterial.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UFont> font(TEXT("/Game/Fonts/LetterFont"));
	if (font.Succeeded()) {
		MyFont = font.Object;
	}
}

// Called when the game starts or when spawned
void ALetterCube::BeginPlay()
{
	Super::BeginPlay();
	InitialSpawnTime = GetWorld()->GetTimeSeconds();
}

void ALetterCube::DrawToCanvasRenderTarget1(UCanvas* Canvas, int32 Width, int32 Height)
{    
	Canvas->SetDrawColor(FColor(0.f,0.f,0.f, 255.f));
	Canvas->DrawText(MyFont, FText::FromString(Letters[0]), Width/2-250, 0); 	
	
}

void ALetterCube::DrawToCanvasRenderTarget2(UCanvas* Canvas, int32 Width, int32 Height)
{    
	Canvas->SetDrawColor(FColor(0.f,0.f,0.f, 255.f));
	Canvas->DrawText(MyFont, FText::FromString(Letters[1]), Width/2-250, 0); 	
	
}

void ALetterCube::DrawToCanvasRenderTarget3(UCanvas* Canvas, int32 Width, int32 Height)
{    
	Canvas->SetDrawColor(FColor(0.f,0.f,0.f, 255.f));
	Canvas->DrawText(MyFont, FText::FromString(Letters[2]), Width/2-250, 0); 	
	
}

void ALetterCube::DrawToCanvasRenderTarget4(UCanvas* Canvas, int32 Width, int32 Height)
{    
	Canvas->SetDrawColor(FColor(0.f,0.f,0.f, 255.f));
	Canvas->DrawText(MyFont, FText::FromString(Letters[3]), Width/2-250, 0); 	
	
}

void ALetterCube::DrawToCanvasRenderTarget5(UCanvas* Canvas, int32 Width, int32 Height)
{    
	Canvas->SetDrawColor(FColor(0.f,0.f,0.f, 255.f));
	Canvas->DrawText(MyFont, FText::FromString(Letters[4]), Width/2-250, 0); 	
	
}

void ALetterCube::DrawToCanvasRenderTarget6(UCanvas* Canvas, int32 Width, int32 Height)
{    
	Canvas->SetDrawColor(FColor(0.f,0.f,0.f, 255.f));
	Canvas->DrawText(MyFont, FText::FromString(Letters[5]), Width/2-250, 0); 		
}

void ALetterCube::Init(FString word, FString mustContainLetter)
{	
	Word = word;
	Letters.Add(mustContainLetter);
	Letters.Add(alphabet[FMath::RandRange(0, alphabet.Num()-1)]);
	Letters.Add(alphabet[FMath::RandRange(0, alphabet.Num()-1)]);
	Letters.Add(alphabet[FMath::RandRange(0, alphabet.Num()-1)]);
	Letters.Add(alphabet[FMath::RandRange(0, alphabet.Num()-1)]);
	Letters.Add(alphabet[FMath::RandRange(0, alphabet.Num()-1)]);	

	StartRender();
}

void ALetterCube::StartRender()
{
	UWorld* world = GetWorld();
	if(world){
		CanvasRenderTarget1 = Cast<UCanvasRenderTarget2D>(UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(world, UCanvasRenderTarget2D::StaticClass(), 1024, 1024));
		CanvasRenderTarget1->OnCanvasRenderTargetUpdate.AddDynamic(this, &ALetterCube::DrawToCanvasRenderTarget1);			
		CanvasRenderTarget1->ClearColor = FLinearColor::White;

		CanvasRenderTarget2 = Cast<UCanvasRenderTarget2D>(UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(world, UCanvasRenderTarget2D::StaticClass(), 1024, 1024));
		CanvasRenderTarget2->OnCanvasRenderTargetUpdate.AddDynamic(this, &ALetterCube::DrawToCanvasRenderTarget2);			
		CanvasRenderTarget2->ClearColor = FLinearColor::White;

		CanvasRenderTarget3 = Cast<UCanvasRenderTarget2D>(UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(world, UCanvasRenderTarget2D::StaticClass(), 1024, 1024));
		CanvasRenderTarget3->OnCanvasRenderTargetUpdate.AddDynamic(this, &ALetterCube::DrawToCanvasRenderTarget3);			
		CanvasRenderTarget3->ClearColor = FLinearColor::White;

		CanvasRenderTarget4 = Cast<UCanvasRenderTarget2D>(UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(world, UCanvasRenderTarget2D::StaticClass(), 1024, 1024));
		CanvasRenderTarget4->OnCanvasRenderTargetUpdate.AddDynamic(this, &ALetterCube::DrawToCanvasRenderTarget4);			
		CanvasRenderTarget4->ClearColor = FLinearColor::White;

		CanvasRenderTarget5 = Cast<UCanvasRenderTarget2D>(UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(world, UCanvasRenderTarget2D::StaticClass(), 1024, 1024));
		CanvasRenderTarget5->OnCanvasRenderTargetUpdate.AddDynamic(this, &ALetterCube::DrawToCanvasRenderTarget5);			
		CanvasRenderTarget5->ClearColor = FLinearColor::White;

		CanvasRenderTarget6 = Cast<UCanvasRenderTarget2D>(UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(world, UCanvasRenderTarget2D::StaticClass(), 1024, 1024));
		CanvasRenderTarget6->OnCanvasRenderTargetUpdate.AddDynamic(this, &ALetterCube::DrawToCanvasRenderTarget6);			
		CanvasRenderTarget6->ClearColor = FLinearColor::White;		

		UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(LetterMaterial, CubeMesh);
		MaterialInstance->SetTextureParameterValue(FName("LetterInput"), CanvasRenderTarget1);
		CubeMesh->SetMaterial(0, MaterialInstance);

		UMaterialInstanceDynamic* MaterialInstance2 = UMaterialInstanceDynamic::Create(LetterMaterial, CubeMesh);
		MaterialInstance2->SetTextureParameterValue(FName("LetterInput"), CanvasRenderTarget2);
		CubeMesh->SetMaterial(1, MaterialInstance2);

		UMaterialInstanceDynamic* MaterialInstance3 = UMaterialInstanceDynamic::Create(LetterMaterial, CubeMesh);
		MaterialInstance3->SetTextureParameterValue(FName("LetterInput"), CanvasRenderTarget3);
		CubeMesh->SetMaterial(2, MaterialInstance3);

		UMaterialInstanceDynamic* MaterialInstance4 = UMaterialInstanceDynamic::Create(LetterMaterial, CubeMesh);
		MaterialInstance4->SetTextureParameterValue(FName("LetterInput"), CanvasRenderTarget4);
		CubeMesh->SetMaterial(3, MaterialInstance4);

		UMaterialInstanceDynamic* MaterialInstance5 = UMaterialInstanceDynamic::Create(LetterMaterial, CubeMesh);
		MaterialInstance5->SetTextureParameterValue(FName("LetterInput"), CanvasRenderTarget5);
		CubeMesh->SetMaterial(4, MaterialInstance5);

		UMaterialInstanceDynamic* MaterialInstance6 = UMaterialInstanceDynamic::Create(LetterMaterial, CubeMesh);
		MaterialInstance6->SetTextureParameterValue(FName("LetterInput"), CanvasRenderTarget6);
		CubeMesh->SetMaterial(5, MaterialInstance6);		
	}

	CanvasRenderTarget1->UpdateResource();
	CanvasRenderTarget2->UpdateResource();
	CanvasRenderTarget3->UpdateResource();
	CanvasRenderTarget4->UpdateResource();
	CanvasRenderTarget5->UpdateResource();
	CanvasRenderTarget6->UpdateResource();
}

// Called every frame
void ALetterCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	FVector velocity = CubeMesh->GetComponentVelocity();
	if(GetWorld()->GetTimeSeconds() - InitialSpawnTime > 5.f && velocity.X == 0.f && velocity.Y == 0.f && velocity.Z == 0.f ){
		Moving = false;		
	}	
}

float ALetterCube::GetNormalizedDegree(float rotationValue)
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

bool ALetterCube::IsTheRightCube_Implementation(int32 index)
{	
	FString charOnTop = GetCharacterOnTop(index);	

	if(charOnTop == Word.Mid(index, 1)){		
		return true;
	}	
	return false;	
}

FString ALetterCube::GetCharacterOnTop(int32 index)
{	
	FRotator componentRotation = CubeMesh->GetComponentRotation();
	
	TArray<FPositionItem> rotationLibrary = {
		{0, 0, 0, 5, true, true, false},
		{180, 180, 0, 5, true, true, false},
		{ 0, 180, 0, 0, true, true, false},
		{ 180, 0, 0, 0, true, true, false},	
		{ 0, -90, 0, 1, false, true, false},
		{ 0, 90, 0, 2, false, true, false},
		{ 90, 0, 0, 4, true, true, false},
		{ -90, 0, 0, 3, true, true, false},			
	};

	FRotator normalizedRotation;
	normalizedRotation.Roll = GetNormalizedDegree(componentRotation.Roll);
	normalizedRotation.Pitch = GetNormalizedDegree(componentRotation.Pitch);
	normalizedRotation.Yaw = GetNormalizedDegree(componentRotation.Yaw);

	for (int32 i = 0; i < rotationLibrary.Num(); i++) {
		// check if the objects match, then get the face
		if (rotationLibrary[i].normalize_x && rotationLibrary[i].normalize_y && rotationLibrary[i].normalize_z) {
			if (rotationLibrary[i].x == normalizedRotation.Roll && rotationLibrary[i].y == normalizedRotation.Pitch && rotationLibrary[i].z == normalizedRotation.Yaw) {
				return Letters[rotationLibrary[i].face];
			}
		} else if (rotationLibrary[i].normalize_x && rotationLibrary[i].normalize_y && !rotationLibrary[i].normalize_z){
			if (rotationLibrary[i].x == normalizedRotation.Roll && rotationLibrary[i].y == normalizedRotation.Pitch) {			
				return Letters[rotationLibrary[i].face];
			}
		} else if (rotationLibrary[i].normalize_x && !rotationLibrary[i].normalize_y && rotationLibrary[i].normalize_z){
			if (rotationLibrary[i].x == normalizedRotation.Roll && rotationLibrary[i].z == normalizedRotation.Yaw) {				
				return Letters[rotationLibrary[i].face];
			}
		} else if (!rotationLibrary[i].normalize_x && rotationLibrary[i].normalize_y && rotationLibrary[i].normalize_z){
			if (rotationLibrary[i].y == normalizedRotation.Pitch && rotationLibrary[i].z == normalizedRotation.Yaw) {			
				return Letters[rotationLibrary[i].face];
			}
		} else if (!rotationLibrary[i].normalize_x && rotationLibrary[i].normalize_y && !rotationLibrary[i].normalize_z){
			if (rotationLibrary[i].y == normalizedRotation.Pitch) {			
				return Letters[rotationLibrary[i].face];
			}
		} else if (!rotationLibrary[i].normalize_x &&! rotationLibrary[i].normalize_y && rotationLibrary[i].normalize_z){
			if (rotationLibrary[i].z == normalizedRotation.Yaw) {		
				return Letters[rotationLibrary[i].face];
			}
		}
	}

	return FString("");
}