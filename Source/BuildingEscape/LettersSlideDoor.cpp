// Fill out your copyright notice in the Description page of Project Settings.


#include "LettersSlideDoor.h"
#include "LetterChosenCube.h"

// Sets default values for this component's properties
ULettersSlideDoor::ULettersSlideDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULettersSlideDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	FindPressurePlates();
	
}


// Called every frame
void ULettersSlideDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULettersSlideDoor::FindPressurePlates()
{
	if (!FirstLetterPlace || !SecondLetterPlace || !ThirdLetterPlace || !ForthLetterPlace)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has the open door component on it, but there are pressure plates missing to be set!!"), *GetOwner()->GetName());
	} else {
		FirstLetterPlace->OnActorBeginOverlap.AddDynamic(this, &ULettersSlideDoor::FirstPlateBeginOverlap);
		FirstLetterPlace->OnActorEndOverlap.AddDynamic(this, &ULettersSlideDoor::FirstPlateEndOverlap);

		SecondLetterPlace->OnActorBeginOverlap.AddDynamic(this, &ULettersSlideDoor::SecondPlateBeginOverlap);
		SecondLetterPlace->OnActorEndOverlap.AddDynamic(this, &ULettersSlideDoor::SecondPlateEndOverlap);				

		ThirdLetterPlace->OnActorBeginOverlap.AddDynamic(this, &ULettersSlideDoor::ThirdPlateBeginOverlap);
		ThirdLetterPlace->OnActorEndOverlap.AddDynamic(this, &ULettersSlideDoor::ThirdPlateEndOverlap);				

		ForthLetterPlace->OnActorBeginOverlap.AddDynamic(this, &ULettersSlideDoor::ForthPlateBeginOverlap);
		ForthLetterPlace->OnActorEndOverlap.AddDynamic(this, &ULettersSlideDoor::ForthPlateEndOverlap);				
	}
}


void ULettersSlideDoor::FirstPlateBeginOverlap(AActor* owner, AActor* otherActor)
{	
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 0);
		if(v == true){
					
		}
		
	}
}


void ULettersSlideDoor::FirstPlateEndOverlap(AActor* owner, AActor* otherActor)
{	
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 0);
		if(v == true){
						
		}		
	}	
}

void ULettersSlideDoor::SecondPlateBeginOverlap(AActor* owner, AActor* otherActor)
{		
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 1);
		if(v == true){
			
		}
		
	}
}


void ULettersSlideDoor::SecondPlateEndOverlap(AActor* owner, AActor* otherActor)
{	
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 1);
		if(v == true){
			
		}		
	}	
}

void ULettersSlideDoor::ThirdPlateBeginOverlap(AActor* owner, AActor* otherActor)
{	
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 2);
		if(v == true){
			
		}
		
	}
}


void ULettersSlideDoor::ThirdPlateEndOverlap(AActor* owner, AActor* otherActor)
{	
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 2);
		if(v == true){
			
		}		
	}	
}

void ULettersSlideDoor::ForthPlateBeginOverlap(AActor* owner, AActor* otherActor)
{	
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 3);
		if(v == true){
			
		}
		
	}
}


void ULettersSlideDoor::ForthPlateEndOverlap(AActor* owner, AActor* otherActor)
{		
	if (otherActor->Implements<ULetterChosenCube>()){
		bool v = ILetterChosenCube::Execute_IsTheRightCube(otherActor, 3);
		if(v == true){
			
		}		
	}	
}