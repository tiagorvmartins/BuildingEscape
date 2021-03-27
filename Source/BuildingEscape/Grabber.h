// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/EngineTypes.h"
#include "GameCube.h"
#include "Grabber.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;	

private:
	
	float const DefaultReach = 100.f;
	
	float Reach = DefaultReach;
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY()
	UInputComponent* InputComponent = nullptr;
	UPROPERTY()
	UParticleSystemComponent* ParticleSystemComponent = nullptr;
	UPROPERTY()
	UParticleSystem* LightParticle = nullptr;
	UPROPERTY()
	UParticleSystemComponent* LightParticleComponent = nullptr;
	UPROPERTY()
	UMaterial* GrabbingMaterial = nullptr;
	UPROPERTY()
	float Grabbing = false;
	UPROPERTY()
	AGameCube* GrabbedGameCube = nullptr;

	void Grab();
	void GrabCloser();
	void GrabFurther();
	void Release();
	void FindPhysicsHandle();
	void FindParticleSystemComponent();
	void SetupInputComponent();

	// Return the first Actor within reach with physics body.
	FHitResult GetFirstPhysicsBodyInReach() const;
	// Return the Line Trace End
	FVector GetPlayersReach() const;
	// Get Players Position In World
	FVector GetPlayersWorldPos() const;			
};
