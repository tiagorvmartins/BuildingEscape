// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "GameFramework/Pawn.h"
#include "GameStats.h"
#include "CubeGeneration.h"
#include "SlideForceField.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API USlideForceField : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USlideForceField();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.f;

	UPROPERTY(EditAnywhere)
	float SlideToX = 0.f;

	UPROPERTY(EditAnywhere)
	float SlideToY = 0.f;

	UPROPERTY(EditAnywhere)
	float SlideToZ = 0.f;

	UPROPERTY(EditAnywhere)
	float OpenSpeed = 1.f;

	UPROPERTY(EditAnywhere)
	float CloseSpeed = 1.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* CubeActor = nullptr;

	UPROPERTY()
	UMaterial* RedMaterial = nullptr;

	UPROPERTY()
	UMaterial* GreenMaterial = nullptr;

	UPROPERTY()
	UStaticMeshComponent* CubeActorMesh = nullptr;

	FVector StartingLocation;
	float SlideMovement;
	float InitialSlide;
	float MovementSlide;	

	UFUNCTION()
	void PlateBeginOverlap(AActor* owner, AActor* otherActor);
	UFUNCTION()
	void PlateEndOverlap(AActor* owner, AActor* otherActor);

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

	UPROPERTY()
	bool isCubePlaced = false;

	UPROPERTY()
	bool CloseDoorSoundPlayed = true;

	UPROPERTY()
	bool OpenDoorSoundPlayed = false;

	void FindPressurePlate();
	void CloseDoor(float DeltaTime);
	void OpenDoor(float DeltaTime);
	void Slide(float DeltaTime, float slideMove, bool open);
	void FindAudioComponent();

	UPROPERTY()
	APawn* Pawn = nullptr;

	UPROPERTY()
	UGameStats* GameStats = nullptr;
		
};
