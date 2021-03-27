// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	StartingYaw = MovePositionYaw = CurrentRotation.Yaw;	

	FindPressurePlate();
	FindAudioComponent();
}

void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has the open door component on it, but no pressureplate set!!"), *GetOwner()->GetName())
	}
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent) {
		UE_LOG(LogTemp, Error, TEXT("%s Missing audio component!"), *GetOwner()->GetName());
		return;
	}	
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > MassToOpenDoors)
	{
		OpenDoor(DeltaTime);
		//DoorLastOpened when the door was opened
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	} 
	else 
	{
		//if the door has been open long than DoorCloseDelay
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{			
			CloseDoor(DeltaTime);
		}
	}	
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	MovePositionYaw = FMath::FInterpTo(MovePositionYaw, StartingYaw+OpenAngle, DeltaTime, OpenSpeed);
	UStaticMeshComponent* mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	if (mesh) {
		mesh->SetRelativeRotation(FRotator(0.f, MovePositionYaw, 0.f));
		CloseDoorSoundPlayed = false;
		if (AudioComponent && OpenDoorSoundPlayed == false)
		{
			AudioComponent->Play();
			OpenDoorSoundPlayed = true;		
		}
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{	
	MovePositionYaw = FMath::FInterpTo(MovePositionYaw, StartingYaw, DeltaTime, CloseSpeed);
	UStaticMeshComponent* mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	if (mesh) {
		mesh->SetRelativeRotation(FRotator(0.f, MovePositionYaw, 0.f));
		OpenDoorSoundPlayed = false;
		if (AudioComponent && CloseDoorSoundPlayed == false)
		{
			AudioComponent->Play();
			CloseDoorSoundPlayed = true;
		}
		
	}	
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;
	
	if(!PressurePlate)
	{
		return TotalMass;
	}
	
	TArray<AActor*> OverlappingActors; 
	PressurePlate->GetOverlappingActors(OverlappingActors);

	for(AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	// Add Up Their Masses.

	return TotalMass;
}