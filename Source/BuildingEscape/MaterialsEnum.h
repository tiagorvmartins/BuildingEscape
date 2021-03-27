#pragma once

#include "CoreMinimal.h"
#include "Materials/Material.h"
#include "MaterialsEnum.generated.h"

USTRUCT()
struct FPositionItem
{
    GENERATED_USTRUCT_BODY();    

    UPROPERTY()
    int32 x;

    UPROPERTY()
      int32 y;

    UPROPERTY()
      int32 z;

    UPROPERTY()
      int32 face;

    UPROPERTY()
    bool normalize_x;

    UPROPERTY()
    bool normalize_y;

    UPROPERTY()
    bool normalize_z;

    FPositionItem(int32 _x, int32 _y, int32 _z, int32 _face, bool normalizeX, bool normalizeY, bool normalizeZ)
    {
      x = _x;
      y = _y;
      z = _z;
      face = _face;
      normalize_x = normalizeX;
      normalize_y = normalizeY;
      normalize_z = normalizeZ;
    }

    FPositionItem() {}
};


USTRUCT()
struct FMaterials
{
    GENERATED_USTRUCT_BODY();

    UPROPERTY()
    TArray<UMaterial*> MaterialEntries;

    UPROPERTY()
    FString Name;

    FMaterials(TArray<UMaterial*> Materials, FString name)
    {
      MaterialEntries = Materials;
      Name = name;
    }

    FMaterials() {}
};

USTRUCT()
struct FMaterialsInstances
{
    GENERATED_USTRUCT_BODY();

    UPROPERTY()
    TArray<UMaterialInstanceDynamic*> MaterialEntries;

    UPROPERTY()
    int32 FaceNumber;    

    FMaterialsInstances(TArray<UMaterialInstanceDynamic*> MaterialInstances, int32 Number)
    {
      MaterialEntries = MaterialInstances;
      FaceNumber = Number;      
    }

    FMaterialsInstances() {}
};