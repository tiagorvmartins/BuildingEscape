// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Engine/EngineTypes.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/Classes/Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CollisionQueryParams.h"
#include "MaterialsEnum.h"
#include "GameCube.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FoundParticle(TEXT("/Game/Particles/LightBubble2"));
	if (FoundParticle.Succeeded()) {
		LightParticle = FoundParticle.Object;
	}
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	FindPhysicsHandle();
	SetupInputComponent();	
	FindParticleSystemComponent();
}


void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {

		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);	
		InputComponent->BindAction("GrabCloser", IE_Pressed, this, &UGrabber::GrabCloser);
		InputComponent->BindAction("GrabFurther", IE_Pressed, this, &UGrabber::GrabFurther);
	}
}
//Checking for Physics Handle Component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle component found on %s !"), *GetOwner()->GetName());
	} 
}

void UGrabber::FindParticleSystemComponent()
{
	ParticleSystemComponent = GetOwner()->FindComponentByClass<UParticleSystemComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle component found on %s !"), *GetOwner()->GetName());
	} 
}

void UGrabber::GrabCloser()
{
	Reach = Reach - 10.f;
}

void UGrabber::GrabFurther()
{	
	Reach = Reach + 10.f;
}

void UGrabber::Grab()
{	
	FHitResult Hit = GetFirstPhysicsBodyInReach();	
	UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();	
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		if(!PhysicsHandle)
		{
			UE_LOG(LogTemp, Error, TEXT("Gonna return no Physics Handle"));
			return;
		}		

		AGameCube* GameCube = Cast<AGameCube>(Hit.GetActor());	

		if (GameCube) {			
			GrabbedGameCube = GameCube;
			GrabbedGameCube->Stationary = false;
			for (auto& Elem : GameCube->SelectedMaterialsMap){				
				FMaterialsInstances MaterialInstances = Elem.Value;
				ComponentToGrab->SetMaterial(Elem.Key, MaterialInstances.MaterialEntries[1]);
			}		
		}
			
		if (LightParticle) {						
			LightParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LightParticle, ComponentToGrab->GetComponentTransform(), false);			
		}


		if (ParticleSystemComponent) {			
			Grabbing = true;
			FVector Location;
			FRotator Rotation;
			GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Location, OUT Rotation);
				
			ParticleSystemComponent->Activate();
			ParticleSystemComponent->SetBeamSourcePoint(0, Location, 0);
			ParticleSystemComponent->SetBeamTargetPoint(0, ComponentToGrab->GetComponentLocation(), 0);
		}
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetPlayersReach());
	}
}

void UGrabber::Release()
{	
	if(!PhysicsHandle)
	{
		return;
	}

	if (ParticleSystemComponent) {
		Grabbing = false;
		ParticleSystemComponent->Deactivate();		
	}

	if (GrabbedGameCube) {		
		for (auto& Elem : GrabbedGameCube->SelectedMaterialsMap) {
		
			FMaterialsInstances MaterialInstances = Elem.Value;
			PhysicsHandle->GetGrabbedComponent()->SetMaterial(Elem.Key, MaterialInstances.MaterialEntries[0]);
		}
		GrabbedGameCube->Stationary = true;	
	}
		
	if (LightParticleComponent) {
		LightParticleComponent->Deactivate();
		LightParticleComponent = nullptr;
	}

	PhysicsHandle->ReleaseComponent();
	Reach = DefaultReach;
	GrabbedGameCube = nullptr;
}



// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!PhysicsHandle)
	{
		return;
	}

	if (PhysicsHandle->GrabbedComponent)
	{
		FVector PlayerReach = GetPlayersReach();
		PhysicsHandle->SetTargetLocation(PlayerReach);		
	}

	if (ParticleSystemComponent && Grabbing) {
		FVector Location;
		FRotator Rotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Location, OUT Rotation);
				
		FVector TargetParam = Location + Rotation.Vector() * 500.f;	

		ParticleSystemComponent->Activate();
		ParticleSystemComponent->SetBeamSourcePoint(0, Location, 0);
		ParticleSystemComponent->SetBeamTargetPoint(0, PhysicsHandle->GrabbedComponent->GetComponentLocation(), 0);

		if (LightParticleComponent) {
			FTransform transform = PhysicsHandle->GrabbedComponent->GetComponentTransform();
			transform.SetScale3D(FVector(1.f, 1.f, 1.f));
			LightParticleComponent->SetWorldTransform(transform);				
		}
	} else {
		ParticleSystemComponent->Deactivate();		
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const {

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit, 
		GetPlayersWorldPos(), 
		GetPlayersReach(), 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), 
		TraceParams
	);

	return Hit;
}

FVector UGrabber::GetPlayersReach() const
{
	FVector Location;
	FRotator Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Location, OUT Rotation);
	//Ray-cast out to a certain distance (reach)	
	return Location + Rotation.Vector() * Reach;
}

FVector UGrabber::GetPlayersWorldPos() const
{
	FVector Location;
	FRotator Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Location, OUT Rotation);
	
	return Location;
}
