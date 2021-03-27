// Fill out your copyright notice in the Description page of Project Settings.


#include "GuessWordComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Components/ActorComponent.h"
#include "Internationalization/Text.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "NavigationSystem.h"
#include "LetterChosenCube.h"
#include "Engine/Canvas.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "Kismet/GameplayStatics.h"
#include "LetterCube.h"
#include "GameStats.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UGuessWordComponent::UGuessWordComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	UMaterial* panelMaterial = nullptr;
	static ConstructorHelpers::FObjectFinder<UMaterial> PanelMaterial(TEXT("/Game/Textures/LevelPanel3_Mat"));
	if (PanelMaterial.Succeeded()) {
		panelMaterial = PanelMaterial.Object;
	}
	PanelMaterialDisplay = panelMaterial;

	UFont* font = nullptr;
	static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("/Game/Fonts/WordFont"));
	if (Font.Succeeded()) {
		font = Font.Object;
	}
	MyFont = font;

	static ConstructorHelpers::FObjectFinder<UMaterial> redMaterial(TEXT("/Game/Meshes/red"));
	if (redMaterial.Succeeded()) {
		RedMaterial = redMaterial.Object;
	}
	

	static ConstructorHelpers::FObjectFinder<UMaterial> greenMaterial(TEXT("/Game/Meshes/green"));
	if (greenMaterial.Succeeded()) {
		GreenMaterial = greenMaterial.Object;
	}
	
}


// Called when the game starts
void UGuessWordComponent::BeginPlay()
{
	Super::BeginPlay();
	
	int64 DateInSeconds = FDateTime::Now().ToUnixTimestamp();	
	FRandomStream SRand = FRandomStream(DateInSeconds*-1);
	float number = SRand.FRandRange(0, Words.Num()-1);
	int32 pos = (int32)number;
	
	
	Word = Words[pos];

	HiddenLetterFirstIndex = FMath::RandRange(0, 3);
	HiddenLetterSecondIndex = FMath::RandRange(0, 3);
	while(HiddenLetterFirstIndex == HiddenLetterSecondIndex){
		HiddenLetterSecondIndex = FMath::RandRange(0, 3);
	}

	if(GetOwner()){
		PanelMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	}

	Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if(Pawn){
		GameStats = Pawn->FindComponentByClass<UGameStats>();
	}

	if(FirstLetterPlate){		
		FirstLetterStaticMesh = FirstLetterPlate->FindComponentByClass<UStaticMeshComponent>();
		FirstLetterStaticMesh->SetMaterial(0, RedMaterial);
	}

	if(SecondLetterPlate){
		SecondLetterStaticMesh = SecondLetterPlate->FindComponentByClass<UStaticMeshComponent>();
		SecondLetterStaticMesh->SetMaterial(0, RedMaterial);
	}

	if(ThirdLetterPlate){
		ThirdLetterStaticMesh = ThirdLetterPlate->FindComponentByClass<UStaticMeshComponent>();
		ThirdLetterStaticMesh->SetMaterial(0, RedMaterial);
	}

	if(ForthLetterPlate){
		ForthLetterStaticMesh = ForthLetterPlate->FindComponentByClass<UStaticMeshComponent>();
		ForthLetterStaticMesh->SetMaterial(0, RedMaterial);
	}

	UWorld* world = GetWorld();

	if(world){
		if (PanelMeshComponent){
				
			CanvasRenderTarget = Cast<UCanvasRenderTarget2D>(UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(world, UCanvasRenderTarget2D::StaticClass(), 1024, 1024));
			CanvasRenderTarget->OnCanvasRenderTargetUpdate.AddDynamic(this, &UGuessWordComponent::DrawToCanvasRenderTarget);			

			UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(PanelMaterialDisplay, PanelMeshComponent);
			MaterialInstance->SetTextureParameterValue(FName("TextureMaterial"), CanvasRenderTarget);
			PanelMeshComponent->SetMaterial(0, MaterialInstance);			
			CanvasRenderTarget->UpdateResource();
		}

		UNavigationSystemV1* NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

		if (NavigationArea) {
			FNavLocation endPosition;			
			for(int i = 0; i < 4; i++){
				if(NavMesh){
					if (NavigationArea->GetRandomPointInNavigableRadius(NavMesh->GetActorLocation(), 5.f, endPosition, nullptr, 0)) {
						FRotator Rotation(0.0f, 0.0f, 0.0f);
						FVector locationObject = endPosition.Location;
						locationObject.Z = locationObject.Z + 2.f; 
						FActorSpawnParameters SpawnInfo;

						ALetterCube* myLetterCube = world->SpawnActor<ALetterCube>(ALetterCube::StaticClass(), locationObject, Rotation, SpawnInfo);
					
						myLetterCube->Init(Word, Word.Mid(i, 1));	
						Cubes.Add(myLetterCube);					
					}
				}
			}
		}

		FindPressurePlates();		
	}
}


// Called every frame
void UGuessWordComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if(GameStats->Level3Started){
		
		if(AreAllCubesProperlyPlaced()){
			GameStats->SetLevelCompleted(2, true);
		
		} else {
			GameStats->SetLevelCompleted(2, false);			
		}
	} else {
		bool shouldGameStart = true;
		for(ALetterCube* cube : Cubes)
		{			
			if(cube->Moving){
				shouldGameStart = false;
				break;
			}			
		}
		GameStats->Level3Started = shouldGameStart;
	}

	
	// ...
}

bool UGuessWordComponent::AreAllCubesProperlyPlaced()
{
	TArray<AActor*> actorsOverlappingFirst;
	FirstLetterPlace->GetOverlappingActors(actorsOverlappingFirst);
	
	bool isOverlappingFirst = false;
	for(AActor* actor : actorsOverlappingFirst)
	{
		if(actor->Implements<ULetterChosenCube>())
		{
			isOverlappingFirst = ILetterChosenCube::Execute_IsTheRightCube(actor, 0);			
		}
	}
	if(!isOverlappingFirst)
		return false;

	bool isOverlappingSecond = false;
	TArray<AActor*> actorsOverlappingSecond;
	SecondLetterPlace->GetOverlappingActors(actorsOverlappingSecond);

	for(AActor* actor : actorsOverlappingSecond)
	{
		if(actor->Implements<ULetterChosenCube>())
		{
			isOverlappingSecond = ILetterChosenCube::Execute_IsTheRightCube(actor, 1);			
		}
	}
	if(!isOverlappingSecond)
		return false;


	bool isOverlappingThird = false;
	TArray<AActor*> actorsOverlappingThird;
	ThirdLetterPlace->GetOverlappingActors(actorsOverlappingThird);

	for(AActor* actor : actorsOverlappingThird)
	{
		if(actor->Implements<ULetterChosenCube>())
		{
			isOverlappingThird = ILetterChosenCube::Execute_IsTheRightCube(actor, 2);			
		}
	}
	if(!isOverlappingThird)
		return false;

	bool isOverlappingForth = false;
	TArray<AActor*> actorsOverlappingForth;
	ForthLetterPlace->GetOverlappingActors(actorsOverlappingForth);

	for(AActor* actor : actorsOverlappingForth)
	{
		if(actor->Implements<ULetterChosenCube>())
		{
			isOverlappingForth = ILetterChosenCube::Execute_IsTheRightCube(actor, 3);			
		}
	}
	
	return isOverlappingFirst && isOverlappingSecond && isOverlappingThird && isOverlappingForth;
}

void UGuessWordComponent::DrawToCanvasRenderTarget(UCanvas* Canvas, int32 Width, int32 Height)
{
	FString newWord = FString("");	
	if(HiddenLetterFirstIndex > 0 || HiddenLetterSecondIndex > 0) {		
		for(int i = 0; i < Word.Len(); i++)
		{
			if(i == HiddenLetterFirstIndex || i == HiddenLetterSecondIndex){				
				newWord.Append(FString("_"));
			} else {
				newWord.Append(Word.Mid(i, 1));
			}				
		}
	} else {
		newWord = Word;
	}
		
	Canvas->DrawText(MyFont, FText::FromString(newWord), (Width/2)-390, Height/2-150);	 	 
}

void UGuessWordComponent::FindPressurePlates()
{
	if (!FirstLetterPlace || !SecondLetterPlace || !ThirdLetterPlace || !ForthLetterPlace)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has the open door component on it, but there are pressure plates missing to be set!!"), *GetOwner()->GetName());
	} else {		
		FirstLetterPlace->OnActorBeginOverlap.AddDynamic(this, &UGuessWordComponent::FirstPlateBeginOverlap);
		FirstLetterPlace->OnActorEndOverlap.AddDynamic(this, &UGuessWordComponent::FirstPlateEndOverlap);

		SecondLetterPlace->OnActorBeginOverlap.AddDynamic(this, &UGuessWordComponent::SecondPlateBeginOverlap);
		SecondLetterPlace->OnActorEndOverlap.AddDynamic(this, &UGuessWordComponent::SecondPlateEndOverlap);				

		ThirdLetterPlace->OnActorBeginOverlap.AddDynamic(this, &UGuessWordComponent::ThirdPlateBeginOverlap);
		ThirdLetterPlace->OnActorEndOverlap.AddDynamic(this, &UGuessWordComponent::ThirdPlateEndOverlap);				

		ForthLetterPlace->OnActorBeginOverlap.AddDynamic(this, &UGuessWordComponent::ForthPlateBeginOverlap);
		ForthLetterPlace->OnActorEndOverlap.AddDynamic(this, &UGuessWordComponent::ForthPlateEndOverlap);				
	}
}

void UGuessWordComponent::FirstPlateBeginOverlap(AActor* owner, AActor* otherActor)
{	
	if (otherActor->Implements<ULetterChosenCube>()){
		
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 0);
		if(v == true){
			
			FirstLetterStaticMesh->SetMaterial(0, GreenMaterial);

			if(HiddenLetterFirstIndex == 0){
				HiddenLetterFirstIndex = -1;
			}

			if(HiddenLetterSecondIndex == 0){
				HiddenLetterSecondIndex = -1;
			}
			CanvasRenderTarget->UpdateResource();

		} else {
			FirstLetterStaticMesh->SetMaterial(0, RedMaterial);
			if(GameStats->Level3Started)
				GameStats->UpdateCurrentLives(1);						
		}			
	}
}

void UGuessWordComponent::FirstPlateEndOverlap(AActor* owner, AActor* otherActor)
{	
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 0);
		if(v == true){
			FirstLetterStaticMesh->SetMaterial(0, RedMaterial);		
		}	
	}	
}

void UGuessWordComponent::SecondPlateBeginOverlap(AActor* owner, AActor* otherActor)
{		
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 1);
		if(v == true){
			SecondLetterStaticMesh->SetMaterial(0, GreenMaterial);

			if(HiddenLetterFirstIndex == 1){
				HiddenLetterFirstIndex = -1;
			}

			if(HiddenLetterSecondIndex == 1){
				HiddenLetterSecondIndex = -1;
			}
			CanvasRenderTarget->UpdateResource();

		} else {
			SecondLetterStaticMesh->SetMaterial(0, RedMaterial);
			if(GameStats->Level3Started)
				GameStats->UpdateCurrentLives(1);
		}	
	}
}


void UGuessWordComponent::SecondPlateEndOverlap(AActor* owner, AActor* otherActor)
{	
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 1);
		if(v == true){
			SecondLetterStaticMesh->SetMaterial(0, RedMaterial);
		}		
	}	
}

void UGuessWordComponent::ThirdPlateBeginOverlap(AActor* owner, AActor* otherActor)
{	
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 2);
		if(v == true){
			ThirdLetterStaticMesh->SetMaterial(0, GreenMaterial);

			if(HiddenLetterFirstIndex == 2){
				HiddenLetterFirstIndex = -1;
			}

			if(HiddenLetterSecondIndex == 2){
				HiddenLetterSecondIndex = -1;
			}
			CanvasRenderTarget->UpdateResource();
		} else {
			ThirdLetterStaticMesh->SetMaterial(0, RedMaterial);
			if(GameStats->Level3Started)
				GameStats->UpdateCurrentLives(1);
		}		
	}
}


void UGuessWordComponent::ThirdPlateEndOverlap(AActor* owner, AActor* otherActor)
{	
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 2);
		if(v == true){
			ThirdLetterStaticMesh->SetMaterial(0, RedMaterial);
		}	
	}	
}

void UGuessWordComponent::ForthPlateBeginOverlap(AActor* owner, AActor* otherActor)
{	
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 3);
		if(v == true){
			ForthLetterStaticMesh->SetMaterial(0, GreenMaterial);
			if(HiddenLetterFirstIndex == 3){
				HiddenLetterFirstIndex = -1;
			}

			if(HiddenLetterSecondIndex == 3){
				HiddenLetterSecondIndex = -1;
			}

			CanvasRenderTarget->UpdateResource();

		} else {
			ForthLetterStaticMesh->SetMaterial(0, RedMaterial);
			if(GameStats->Level3Started)
				GameStats->UpdateCurrentLives(1);
		}	
	}
}


void UGuessWordComponent::ForthPlateEndOverlap(AActor* owner, AActor* otherActor)
{		
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 3);
		if(v == true){
			ForthLetterStaticMesh->SetMaterial(0, RedMaterial);
		}		
	}	
}