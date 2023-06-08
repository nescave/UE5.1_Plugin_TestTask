// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SpawnedActorData.generated.h"

USTRUCT(Blueprintable)
struct FSpawnedActorData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MeshName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MeshPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator MeshRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MeshScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;

	FSpawnedActorData()
	{
		MeshName = TEXT("");
		MeshPosition = FVector();
		MeshRotation = FRotator();
		MeshScale = FVector();
		StaticMesh = nullptr;
	}
};
