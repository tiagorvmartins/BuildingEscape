// Fill out your copyright notice in the Description page of Project Settings.


#include "WordGeneration.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Components/ActorComponent.h"
#include "Internationalization/Text.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "NavigationSystem.h"
#include "LetterChosenCube.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "LetterCube.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UWordGeneration::UWordGeneration()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	UMaterial* panelMaterial = nullptr;
	static ConstructorHelpers::FObjectFinder<UMaterial> PanelMaterial(TEXT("/Game/Textures/LevelPanel2_Mat"));
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
void UWordGeneration::BeginPlay()
{
	Super::BeginPlay();

	int64 DateInSeconds = FDateTime::Now().ToUnixTimestamp();
	
	FRandomStream SRand = FRandomStream(DateInSeconds);
	float number = SRand.FRandRange(0, Words.Num()-1);

	int32 pos = (int32)number;
	
	Word = Words[pos];

	Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if(Pawn){
		GameStats = Pawn->FindComponentByClass<UGameStats>();
	}

	if(LeftDoor){
		LeftDoorComponent = LeftDoor->FindComponentByClass<UStaticMeshComponent>();	
		StartingLocationLeft = LeftDoor->GetActorLocation();
		
		if (SlideLeftToX != 0.f){
			SlideMovementLeft = SlideLeftToX;
			InitialSlideLeft = StartingLocationLeft.X;
			MovementSlideLeft = StartingLocationLeft.X;
		} else if (SlideLeftToY != 0.f){
			SlideMovementLeft = SlideLeftToY;
			InitialSlideLeft = StartingLocationLeft.Y;
			MovementSlideLeft = StartingLocationLeft.Y;
		} else if (SlideLeftToZ != 0.f){
			SlideMovementLeft = SlideLeftToZ;
			InitialSlideLeft = StartingLocationLeft.Z;
			MovementSlideLeft = StartingLocationLeft.Z;
		}
	}

	if(RightDoor){
		RightDoorComponent = RightDoor->FindComponentByClass<UStaticMeshComponent>();
		StartingLocationRight = RightDoor->GetActorLocation();
		
		if (SlideRightToX != 0.f){
			SlideMovementRight = SlideRightToX;
			InitialSlideRight = StartingLocationRight.X;
			MovementSlideRight = StartingLocationRight.X;
		} else if (SlideRightToY != 0.f){
			SlideMovementRight = SlideRightToY;
			InitialSlideRight = StartingLocationRight.Y;
			MovementSlideRight = StartingLocationRight.Y;
		} else if (SlideRightToZ != 0.f){
			SlideMovementRight = SlideRightToZ;
			InitialSlideRight = StartingLocationRight.Z;
			MovementSlideRight = StartingLocationRight.Z;
		}
	}

	if(GetOwner()){
		PanelMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
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
			CanvasRenderTarget->OnCanvasRenderTargetUpdate.AddDynamic(this, &UWordGeneration::DrawToCanvasRenderTarget);			

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
		FindAudioComponent();
	}
	
}


// Called every frame
void UWordGeneration::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(GameStats->Level2Started){
		
		if(AreAllCubesProperlyPlaced()){
				//Open Door
			GameStats->SetLevelCompleted(1, true);
			OpenLeftDoor(DeltaTime);
			OpenRightDoor(DeltaTime);

			DoorLastOpened = GetWorld()->GetTimeSeconds();
			CloseDoorSoundPlayed = false;
			if (AudioComponent && OpenDoorSoundPlayed == false)
			{
				AudioComponent->Play();
				OpenDoorSoundPlayed = true;		
			}

		} else {
			//Close Door
			GameStats->SetLevelCompleted(1, false);
			if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
			{			
				CloseLeftDoor(DeltaTime);
				CloseRightDoor(DeltaTime);
				OpenDoorSoundPlayed = false;
				if (AudioComponent && CloseDoorSoundPlayed == false)
				{
					AudioComponent->Play();
					CloseDoorSoundPlayed = true;
				}			
			}		
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
		GameStats->Level2Started = shouldGameStart;
	}
	
}

void UWordGeneration::DrawToCanvasRenderTarget(UCanvas* Canvas, int32 Width, int32 Height)
{
    //Drawing code with UCanvas goes here...
	//if(TotalSumFixed > 9){
		
		Canvas->DrawText(MyFont, FText::FromString(Word), (Width/2)-390, Height/2-150);	 	 
	// } else {
	// 	Canvas->DrawText(MyFont, FText::FromString(FString::Printf(TEXT("%i"), TotalSumFixed)), (Width/2)-256, Height/2-1024/2+100);	 	 
	// }	 
}

void UWordGeneration::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent) {
		UE_LOG(LogTemp, Error, TEXT("%s Missing audio component!"), *GetOwner()->GetName());
		return;
	}	
}


void UWordGeneration::FindPressurePlates()
{
	if (!FirstLetterPlace || !SecondLetterPlace || !ThirdLetterPlace || !ForthLetterPlace)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has the open door component on it, but there are pressure plates missing to be set!!"), *GetOwner()->GetName());
	} else {
		FirstLetterPlace->OnActorBeginOverlap.AddDynamic(this, &UWordGeneration::FirstPlateBeginOverlap);
		FirstLetterPlace->OnActorEndOverlap.AddDynamic(this, &UWordGeneration::FirstPlateEndOverlap);

		SecondLetterPlace->OnActorBeginOverlap.AddDynamic(this, &UWordGeneration::SecondPlateBeginOverlap);
		SecondLetterPlace->OnActorEndOverlap.AddDynamic(this, &UWordGeneration::SecondPlateEndOverlap);				

		ThirdLetterPlace->OnActorBeginOverlap.AddDynamic(this, &UWordGeneration::ThirdPlateBeginOverlap);
		ThirdLetterPlace->OnActorEndOverlap.AddDynamic(this, &UWordGeneration::ThirdPlateEndOverlap);				

		ForthLetterPlace->OnActorBeginOverlap.AddDynamic(this, &UWordGeneration::ForthPlateBeginOverlap);
		ForthLetterPlace->OnActorEndOverlap.AddDynamic(this, &UWordGeneration::ForthPlateEndOverlap);				
	}
}


void UWordGeneration::FirstPlateBeginOverlap(AActor* owner, AActor* otherActor)
{	
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 0);
		if(v == true){						
			FirstLetterStaticMesh->SetMaterial(0, GreenMaterial);
			FirstLetterStaticMesh->MarkRenderStateDirty();
		} else {
			FirstLetterStaticMesh->SetMaterial(0, RedMaterial);
			if(GameStats->Level2Started){
				GameStats->UpdateCurrentLives(1);
			}
		}			
	}
}

void UWordGeneration::FirstPlateEndOverlap(AActor* owner, AActor* otherActor)
{	
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 0);
		if(v == true){
			FirstLetterStaticMesh->SetMaterial(0, RedMaterial);		
		}	
	}	
}

void UWordGeneration::SecondPlateBeginOverlap(AActor* owner, AActor* otherActor)
{		
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 1);
		if(v == true){
			SecondLetterStaticMesh->SetMaterial(0, GreenMaterial);
		} else {
			SecondLetterStaticMesh->SetMaterial(0, RedMaterial);
			if(GameStats->Level2Started)
				GameStats->UpdateCurrentLives(1);		
		}	
	}
}


void UWordGeneration::SecondPlateEndOverlap(AActor* owner, AActor* otherActor)
{	
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 1);
		if(v == true){
			SecondLetterStaticMesh->SetMaterial(0, RedMaterial);
		}		
	}	
}

void UWordGeneration::ThirdPlateBeginOverlap(AActor* owner, AActor* otherActor)
{	
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 2);
		if(v == true){
			ThirdLetterStaticMesh->SetMaterial(0, GreenMaterial);
		} else {
			ThirdLetterStaticMesh->SetMaterial(0, RedMaterial);
			if(GameStats->Level2Started)
				GameStats->UpdateCurrentLives(1);		
		}		
	}
}


void UWordGeneration::ThirdPlateEndOverlap(AActor* owner, AActor* otherActor)
{	
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 2);
		if(v == true){
			ThirdLetterStaticMesh->SetMaterial(0, RedMaterial);
		}	
	}	
}

void UWordGeneration::ForthPlateBeginOverlap(AActor* owner, AActor* otherActor)
{	
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 3);
		if(v == true){
			
			ForthLetterStaticMesh->SetMaterial(0, GreenMaterial);
		} else {
			ForthLetterStaticMesh->SetMaterial(0, RedMaterial);
			if(GameStats->Level2Started)
				GameStats->UpdateCurrentLives(1);		
		}	
	}
}


void UWordGeneration::ForthPlateEndOverlap(AActor* owner, AActor* otherActor)
{		
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 3);
		if(v == true){
			ForthLetterStaticMesh->SetMaterial(0, RedMaterial);
		}		
	}	
}

bool UWordGeneration::AreAllCubesProperlyPlaced()
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



void UWordGeneration::OpenLeftDoor(float DeltaTime)
{
	if (SlideLeftToX != 0.f){
		SlideLeft(DeltaTime, SlideLeftToX, true);
	} else if (SlideLeftToY != 0.f){
		SlideLeft(DeltaTime, SlideLeftToY, true);
	} else if (SlideLeftToZ != 0.f){
		SlideLeft(DeltaTime, SlideLeftToZ, true);
	}

	
}

void UWordGeneration::SlideLeft(float DeltaTime, float Slide, bool open)
{
	if(open)
		MovementSlideLeft = FMath::FInterpTo(MovementSlideLeft, Slide, DeltaTime, OpenSpeed);
	else
		MovementSlideLeft = FMath::FInterpTo(MovementSlideLeft, Slide, DeltaTime, CloseSpeed);

	LeftDoorComponent->SetWorldLocation(FVector(StartingLocationLeft.X, MovementSlideLeft, StartingLocationLeft.Z)); //TODO: hardcoded location move on Y axis.
	
}

void UWordGeneration::CloseLeftDoor(float DeltaTime)
{	
	if (SlideLeftToX != 0.f){
		SlideLeft(DeltaTime, InitialSlideLeft, false);
	} else if (SlideLeftToY != 0.f){
		SlideLeft(DeltaTime, InitialSlideLeft, false);
	} else if (SlideLeftToZ != 0.f){
		SlideLeft(DeltaTime, InitialSlideLeft, false);
	}	
}

void UWordGeneration::OpenRightDoor(float DeltaTime)
{
	if (SlideRightToX != 0.f){
		SlideRight(DeltaTime, SlideRightToX, true);
	} else if (SlideRightToY != 0.f){
		SlideRight(DeltaTime, SlideRightToY, true);
	} else if (SlideRightToZ != 0.f){
		SlideRight(DeltaTime, SlideRightToZ, true);
	}

	
}

void UWordGeneration::SlideRight(float DeltaTime, float Slide, bool open)
{
	if(open)
		MovementSlideRight = FMath::FInterpTo(MovementSlideRight, Slide, DeltaTime, OpenSpeed);
	else
		MovementSlideRight = FMath::FInterpTo(MovementSlideRight, Slide, DeltaTime, CloseSpeed);

	RightDoorComponent->SetWorldLocation(FVector(StartingLocationRight.X, MovementSlideRight, StartingLocationRight.Z)); //TODO: hardcoded location move on Y axis.
	
}

void UWordGeneration::CloseRightDoor(float DeltaTime)
{	
	if (SlideRightToX != 0.f){
		SlideRight(DeltaTime, InitialSlideRight, false);
	} else if (SlideRightToY != 0.f){
		SlideRight(DeltaTime, InitialSlideRight, false);
	} else if (SlideRightToZ != 0.f){
		SlideRight(DeltaTime, InitialSlideRight, false);
	}	
}