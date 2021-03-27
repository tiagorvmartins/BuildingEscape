// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LetterChosenCube.h"
#include "Materials/Material.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Engine/Texture.h"
#include "Math/Color.h"
#include "LetterCube.generated.h"

UCLASS()
class BUILDINGESCAPE_API ALetterCube : public AActor, public ILetterChosenCube
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALetterCube();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	UStaticMeshComponent* CubeMesh = nullptr;

	void Init(FString word, FString mustContainLetter);

	virtual bool IsTheRightCube_Implementation(int32 index);

	UPROPERTY()
	bool Moving = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	// Called every frame
	
	UPROPERTY()
	UMaterial* LetterMaterial = nullptr;

	UFUNCTION()
	void DrawToCanvasRenderTarget1(UCanvas* Canvas, int32 Width, int32 Height);

	UPROPERTY()
	UCanvasRenderTarget2D* CanvasRenderTarget1 = nullptr;

	UFUNCTION()
	void DrawToCanvasRenderTarget2(UCanvas* Canvas, int32 Width, int32 Height);

	UPROPERTY()
	UCanvasRenderTarget2D* CanvasRenderTarget2 = nullptr;

	UFUNCTION()
	void DrawToCanvasRenderTarget3(UCanvas* Canvas, int32 Width, int32 Height);

	UPROPERTY()
	UCanvasRenderTarget2D* CanvasRenderTarget3 = nullptr;

	UFUNCTION()
	void DrawToCanvasRenderTarget4(UCanvas* Canvas, int32 Width, int32 Height);

	UPROPERTY()
	UCanvasRenderTarget2D* CanvasRenderTarget4 = nullptr;

	UFUNCTION()
	void DrawToCanvasRenderTarget5(UCanvas* Canvas, int32 Width, int32 Height);

	UPROPERTY()
	UCanvasRenderTarget2D* CanvasRenderTarget5 = nullptr;

	UFUNCTION()
	void DrawToCanvasRenderTarget6(UCanvas* Canvas, int32 Width, int32 Height);

	UPROPERTY()
	UCanvasRenderTarget2D* CanvasRenderTarget6 = nullptr;

	UPROPERTY()
	UFont* MyFont = nullptr;


	UPROPERTY()
	TArray<FString> Letters;

	float GetNormalizedDegree(float rotationValue);

	

	const TArray<FString> alphabet = {TEXT("A"),TEXT("B"),TEXT("C"),TEXT("D"),TEXT("E"),TEXT("F"),TEXT("G"),TEXT("H"),TEXT("I"),TEXT("J"),TEXT("K"),TEXT("L"),TEXT("M"),TEXT("N"),TEXT("O"),
	TEXT("P"),TEXT("Q"),TEXT("R"),TEXT("S"),TEXT("T"),TEXT("U"),TEXT("V"),TEXT("W"),TEXT("X"),TEXT("Y"),TEXT("Z")};

	void StartRender();

	UPROPERTY()
	FString Word;

	FString GetCharacterOnTop(int32 index);

	UPROPERTY()
	float InitialSpawnTime;

	
};
