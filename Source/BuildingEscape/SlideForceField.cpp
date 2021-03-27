// Fill out your copyright notice in the Description page of Project Settings.


#include "SlideForceField.h"
#include "CubeGeneration.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "GameStats.h"
#include "GameCube.h"

// Sets default values for this component's properties
USlideForceField::USlideForceField()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterial> redMaterial(TEXT("/Game/Meshes/red"));
	if (redMaterial.Succeeded()) {
		RedMaterial = redMaterial.Object;
	}
	

	static ConstructorHelpers::FObjectFinder<UMaterial> greenMaterial(TEXT("/Game/Meshes/green"));
	if (greenMaterial.Succeeded()) {
		GreenMaterial = greenMaterial.Object;
	}

	// ...
}


// Called when the game starts
void USlideForceField::BeginPlay()
{
	Super::BeginPlay();

	Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if(Pawn){
		GameStats = Pawn->FindComponentByClass<UGameStats>();
	}

	FVector InitialLocation = GetOwner()->GetActorLocation();
	StartingLocation = InitialLocation;
	
	if (SlideToX != 0.f){
		SlideMovement = SlideToX;
		InitialSlide = StartingLocation.X;
		MovementSlide = StartingLocation.X;
	} else if (SlideToY != 0.f){
		SlideMovement = SlideToY;
		InitialSlide = StartingLocation.Y;
		MovementSlide = StartingLocation.Y;
	} else if (SlideToZ != 0.f){
		SlideMovement = SlideToZ;
		InitialSlide = StartingLocation.Z;
		MovementSlide = StartingLocation.Z;
	}

	FindPressurePlate();
	FindAudioComponent();

	if(CubeActor){
		CubeActorMesh = CubeActor->FindComponentByClass<UStaticMeshComponent>();
		CubeActorMesh->SetMaterial(0, RedMaterial);
	}
	
}

void USlideForceField::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent) {
		UE_LOG(LogTemp, Error, TEXT("%s Missing audio component!"), *GetOwner()->GetName());
		return;
	}	
}


void USlideForceField::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (isCubePlaced) {
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
		CloseDoorSoundPlayed = false;
		if (AudioComponent && OpenDoorSoundPlayed == false)
		{
			AudioComponent->Play();
			OpenDoorSoundPlayed = true;		
		}
	} else {
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{			
			CloseDoor(DeltaTime);
			OpenDoorSoundPlayed = false;
			if (AudioComponent && CloseDoorSoundPlayed == false)
			{
				AudioComponent->Play();
				CloseDoorSoundPlayed = true;
			}			
		}
		
	}
}

void USlideForceField::OpenDoor(float DeltaTime)
{
	if (SlideToX != 0.f){
		Slide(DeltaTime, SlideToX, true);
	} else if (SlideToY != 0.f){
		Slide(DeltaTime, SlideToY, true);
	} else if (SlideToZ != 0.f){
		Slide(DeltaTime, SlideToZ, true);
	}

	
}

void USlideForceField::Slide(float DeltaTime, float Slide, bool open)
{
	if(open)
		MovementSlide = FMath::FInterpTo(MovementSlide, Slide, DeltaTime, OpenSpeed);
	else
		MovementSlide = FMath::FInterpTo(MovementSlide, Slide, DeltaTime, CloseSpeed);

	UStaticMeshComponent* mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	if (mesh) {
		mesh->SetWorldLocation(FVector(StartingLocation.X, MovementSlide, StartingLocation.Z)); //TODO: hardcoded location move on Y axis.
	}
}

void USlideForceField::CloseDoor(float DeltaTime)
{	
	if (SlideToX != 0.f){
		Slide(DeltaTime, InitialSlide, false);
	} else if (SlideToY != 0.f){
		Slide(DeltaTime, InitialSlide, false);
	} else if (SlideToZ != 0.f){
		Slide(DeltaTime, InitialSlide, false);
	}	
}

void USlideForceField::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has the open door component on it, but no pressureplate set!!"), *GetOwner()->GetName());
	} else {		
		PressurePlate->OnActorBeginOverlap.AddDynamic(this, &USlideForceField::PlateBeginOverlap);
		PressurePlate->OnActorEndOverlap.AddDynamic(this, &USlideForceField::PlateEndOverlap);				
	}
}

void USlideForceField::PlateBeginOverlap(AActor* owner, AActor* otherActor)
{	
	bool IsRightCubeOnTop = false;

	if (otherActor->Implements<UChosenCube>()){
		bool v = IChosenCube::Execute_IsTheRightCube(otherActor);
		if(v == true){
			isCubePlaced = true;		
			GameStats->SetLevelCompleted(0, true);
			CubeActorMesh->SetMaterial(0, GreenMaterial);
		} else {
			CubeActorMesh->SetMaterial(0, RedMaterial);
			if(GameStats->Level1Started)
				GameStats->UpdateCurrentLives(1);	
		}
		
	}
}


void USlideForceField::PlateEndOverlap(AActor* owner, AActor* otherActor)
{	
	bool IsRightCubeOnTop = false;
	if (otherActor->Implements<UChosenCube>()){
		bool v = IChosenCube::Execute_IsTheRightCube(otherActor);
		if(v == true){
			isCubePlaced = false;
			GameStats->SetLevelCompleted(0, false);	
			CubeActorMesh->SetMaterial(0, RedMaterial);	
		}		
	}	
}
