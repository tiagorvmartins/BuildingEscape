// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "LettersSlideDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API ULettersSlideDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULettersSlideDoor();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	// Called every frame
	UPROPERTY(EditAnywhere)
	ATriggerVolume* FirstLetterPlace = nullptr;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* SecondLetterPlace = nullptr;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* ThirdLetterPlace = nullptr;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* ForthLetterPlace = nullptr;

	
	void FindPressurePlates();

	UFUNCTION()
	void FirstPlateBeginOverlap(AActor* owner, AActor* otherActor);
	UFUNCTION()
	void FirstPlateEndOverlap(AActor* owner, AActor* otherActor);

	UFUNCTION()
	void SecondPlateBeginOverlap(AActor* owner, AActor* otherActor);
	UFUNCTION()
	void SecondPlateEndOverlap(AActor* owner, AActor* otherActor);

	UFUNCTION()
	void ThirdPlateBeginOverlap(AActor* owner, AActor* otherActor);
	UFUNCTION()
	void ThirdPlateEndOverlap(AActor* owner, AActor* otherActor);

	UFUNCTION()
	void ForthPlateBeginOverlap(AActor* owner, AActor* otherActor);
	UFUNCTION()
	void ForthPlateEndOverlap(AActor* owner, AActor* otherActor);

		
};
