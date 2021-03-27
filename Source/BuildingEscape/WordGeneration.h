// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LetterCube.h"
#include "GameStats.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Components/AudioComponent.h"
#include "WordGeneration.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UWordGeneration : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWordGeneration();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	// Called every frame

	UPROPERTY()
	UMaterial* PanelMaterialDisplay = nullptr;

	UFUNCTION()
	void DrawToCanvasRenderTarget(UCanvas* Canvas, int32 Width, int32 Height);

	UPROPERTY()
	UCanvasRenderTarget2D* CanvasRenderTarget = nullptr;

	UPROPERTY()
	UStaticMeshComponent* PanelMeshComponent = nullptr;

	UPROPERTY()
	UFont* MyFont = nullptr;

	UPROPERTY(EditAnywhere)
	ANavMeshBoundsVolume* NavMesh = nullptr;

	UPROPERTY()
	FString Word;

	UPROPERTY(EditAnywhere)
	AActor* LeftDoor = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* RightDoor = nullptr;

	UPROPERTY()
	UStaticMeshComponent* LeftDoorComponent = nullptr;

	UPROPERTY()
	UStaticMeshComponent* RightDoorComponent = nullptr;

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
	bool AreAllCubesProperlyPlaced();

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

	UPROPERTY(EditAnywhere)
	AActor* FirstLetterPlate = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* SecondLetterPlate = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* ThirdLetterPlate = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* ForthLetterPlate = nullptr;

	UPROPERTY()
	UMaterial* GreenMaterial = nullptr;

	UPROPERTY()
	UMaterial* RedMaterial = nullptr;

	UPROPERTY()
	UStaticMeshComponent* FirstLetterStaticMesh = nullptr;

	UPROPERTY()
	UStaticMeshComponent* SecondLetterStaticMesh = nullptr;

	UPROPERTY()
	UStaticMeshComponent* ThirdLetterStaticMesh = nullptr;

	UPROPERTY()
	UStaticMeshComponent* ForthLetterStaticMesh = nullptr;

	UPROPERTY()
	bool CloseDoorSoundPlayed = true;

	UPROPERTY()
	bool OpenDoorSoundPlayed = false;

	void CloseLeftDoor(float DeltaTime);
	void OpenLeftDoor(float DeltaTime);
	void SlideLeft(float DeltaTime, float slideMove, bool open);
	void CloseRightDoor(float DeltaTime);
	void OpenRightDoor(float DeltaTime);
	void SlideRight(float DeltaTime, float slideMove, bool open);
	void FindAudioComponent();

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.f;	

	UPROPERTY(EditAnywhere)
	float OpenSpeed = 1.f;

	UPROPERTY(EditAnywhere)
	float CloseSpeed = 1.f;

	UPROPERTY(EditAnywhere)
	float SlideLeftToX = 0.f;

	UPROPERTY(EditAnywhere)
	float SlideLeftToY = 0.f;

	UPROPERTY(EditAnywhere)
	float SlideLeftToZ = 0.f;

	UPROPERTY(EditAnywhere)
	float SlideRightToX = 0.f;

	UPROPERTY(EditAnywhere)
	float SlideRightToY = 0.f;

	UPROPERTY(EditAnywhere)
	float SlideRightToZ = 0.f;

	FVector StartingLocationLeft;
	float SlideMovementLeft;
	float InitialSlideLeft;
	float MovementSlideLeft;

	FVector StartingLocationRight;
	float SlideMovementRight;
	float InitialSlideRight;
	float MovementSlideRight;

	UPROPERTY()
	TArray<ALetterCube*> Cubes;

	UPROPERTY()
	APawn* Pawn = nullptr;

	UPROPERTY()
	UGameStats* GameStats = nullptr;

	const TArray<FString> Words = {TEXT("ABLE"),
TEXT("ALSO"),
TEXT("AREA"),
TEXT("AWAY"),
TEXT("BABY"),
TEXT("BACK"),
TEXT("BALL"),
TEXT("BANK"),
TEXT("BASE"),
TEXT("BEAT"),
TEXT("BEST"),
TEXT("BILL"),
TEXT("BLUE"),
TEXT("BODY"),
TEXT("BOOK"),
TEXT("BORN"),
TEXT("BOTH"),
TEXT("CALL"),
TEXT("CARD"),
TEXT("CARE"),
TEXT("CASE"),
TEXT("CELL"),
TEXT("CITY"),
TEXT("COLD"),
TEXT("COME"),
TEXT("COST"),
TEXT("DARK"),
TEXT("DATA"),
TEXT("DEAD"),
TEXT("DEAL"),
TEXT("DEEP"),
TEXT("DOOR"),
TEXT("DOWN"),
TEXT("DRAW"),
TEXT("DROP"),
TEXT("DRUG"),
TEXT("EACH"),
TEXT("EAST"),
TEXT("EASY"),
TEXT("EDGE"),
TEXT("ELSE"),
TEXT("EVEN"),
TEXT("EVER"),
TEXT("FACE"),
TEXT("FACT"),
TEXT("FAIL"),
TEXT("FALL"),
TEXT("FAST"),
TEXT("FEAR"),
TEXT("FEEL"),
TEXT("FILL"),
TEXT("FILM"),
TEXT("FIND"),
TEXT("FINE"),
TEXT("FIRE"),
TEXT("FIRM"),
TEXT("FISH"),
TEXT("FIVE"),
TEXT("FOOD"),
TEXT("FOOT"),
TEXT("FORM"),
TEXT("FOUR"),
TEXT("FREE"),
TEXT("FROM"),
TEXT("FULL"),
TEXT("FUND"),
TEXT("GAME"),
TEXT("GIRL"),
TEXT("GIVE"),
TEXT("GOAL"),
TEXT("GOOD"),
TEXT("GROW"),
TEXT("HAIR"),
TEXT("HALF"),
TEXT("HAND"),
TEXT("HANG"),
TEXT("HARD"),
TEXT("HAVE"),
TEXT("HEAD"),
TEXT("HEAR"),
TEXT("HEAT"),
TEXT("HELP"),
TEXT("HERE"),
TEXT("HIGH"),
TEXT("HOLD"),
TEXT("HOME"),
TEXT("HOPE"),
TEXT("HOUR"),
TEXT("HUGE"),
TEXT("IDEA"),
TEXT("INTO"),
TEXT("ITEM"),
TEXT("JOIN"),
TEXT("JUST"),
TEXT("KEEP"),
TEXT("KILL"),
TEXT("KIND"),
TEXT("KNOW"),
TEXT("LAND"),
TEXT("LAST"),
TEXT("LATE"),
TEXT("LEAD"),
TEXT("LEFT"),
TEXT("LESS"),
TEXT("LIFE"),
TEXT("LIKE"),
TEXT("LINE"),
TEXT("LIST"),
TEXT("LIVE"),
TEXT("LONG"),
TEXT("LOOK"),
TEXT("LOSE"),
TEXT("LOSS"),
TEXT("LOVE"),
TEXT("MAIN"),
TEXT("MAKE"),
TEXT("MANY"),
TEXT("MEAN"),
TEXT("MEET"),
TEXT("MIND"),
TEXT("MISS"),
TEXT("MORE"),
TEXT("MOST"),
TEXT("MOVE"),
TEXT("MUCH"),
TEXT("MUST"),
TEXT("NAME"),
TEXT("NEAR"),
TEXT("NEED"),
TEXT("NEWS"),
TEXT("NEXT"),
TEXT("NICE"),
TEXT("NONE"),
TEXT("NOTE"),
TEXT("ONCE"),
TEXT("ONLY"),
TEXT("ONTO"),
TEXT("OPEN"),
TEXT("OVER"),
TEXT("PAGE"),
TEXT("PAIN"),
TEXT("PART"),
TEXT("PASS"),
TEXT("PAST"),
TEXT("PICK"),
TEXT("PLAN"),
TEXT("PLAY"),
TEXT("POOR"),
TEXT("PULL"),
TEXT("PUSH"),
TEXT("RACE"),
TEXT("RATE"),
TEXT("READ"),
TEXT("REAL"),
TEXT("REST"),
TEXT("RICH"),
TEXT("RISE"),
TEXT("RISK"),
TEXT("ROAD"),
TEXT("ROCK"),
TEXT("ROLE"),
TEXT("ROOM"),
TEXT("RULE"),
TEXT("SAFE"),
TEXT("SAME"),
TEXT("SAVE"),
TEXT("SEAT"),
TEXT("SEEK"),
TEXT("SEEM"),
TEXT("SELL"),
TEXT("SEND"),
TEXT("SHOT"),
TEXT("SHOW"),
TEXT("SIDE"),
TEXT("SIGN"),
TEXT("SING"),
TEXT("SITE"),
TEXT("SIZE"),
TEXT("SKIN"),
TEXT("SOME"),
TEXT("SONG"),
TEXT("SOON"),
TEXT("SORT"),
TEXT("STAR"),
TEXT("STAY"),
TEXT("STEP"),
TEXT("STOP"),
TEXT("SUCH"),
TEXT("SURE"),
TEXT("TAKE"),
TEXT("TALK"),
TEXT("TASK"),
TEXT("TEAM"),
TEXT("TELL"),
TEXT("TEND"),
TEXT("TERM"),
TEXT("TEST"),
TEXT("THAN"),
TEXT("THAT"),
TEXT("THEM"),
TEXT("THEN"),
TEXT("THEY"),
TEXT("THIS"),
TEXT("THUS"),
TEXT("TIME"),
TEXT("TOWN"),
TEXT("TREE"),
TEXT("TRIP"),
TEXT("TRUE"),
TEXT("TURN"),
TEXT("TYPE"),
TEXT("UNIT"),
TEXT("UPON"),
TEXT("VERY"),
TEXT("VIEW"),
TEXT("VOTE"),
TEXT("WAIT"),
TEXT("WALK"),
TEXT("WALL"),
TEXT("WANT"),
TEXT("WEAR"),
TEXT("WEEK"),
TEXT("WELL"),
TEXT("WEST"),
TEXT("WHAT"),
TEXT("WHEN"),
TEXT("WHOM"),
TEXT("WIDE"),
TEXT("WIFE"),
TEXT("WILL"),
TEXT("WIND"),
TEXT("WISH"),
TEXT("WITH"),
TEXT("WORD"),
TEXT("WORK"),
TEXT("YARD"),
TEXT("YEAH"),
TEXT("YEAR"),
TEXT("YOUR")};

};
