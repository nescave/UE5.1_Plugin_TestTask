// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/VolumeSpawnerActor.h"
#include "DebugUtils.h"
#include "VolumeSpawnerTest.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Subsystems/EditorActorSubsystem.h"

FVector AVolumeSpawnerActor::GetRandomLocationWithinBounds() const
{
	const auto Extrema = Brush.Get()->Points[0];
	FVector RandomOffset;
	RandomOffset.X += FMath::FRandRange(-Extrema.X, Extrema.X);
	RandomOffset.Y += FMath::FRandRange(-Extrema.Y, Extrema.Y);
	RandomOffset.Z += FMath::FRandRange(-Extrema.Z, Extrema.Z);

	//transforming local offset from SpawnerActor to global position of each mesh
	const auto RandomLocation = GetTransform().TransformPosition(RandomOffset);

	return RandomLocation;
}

UEditorActorSubsystem* AVolumeSpawnerActor::GetActorSubsystem()
{
	if (!EditorActorSubsystem)
		EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();

	return EditorActorSubsystem;
}

//make sure that previous set of meshes is cleaned
void AVolumeSpawnerActor::EmptySpawnedMeshActorsArray()
{
	for (const auto& MeshActor : SpawnedMeshActors)
	{
		MeshActor->Destroy();
	}
	SpawnedMeshActors.Empty();
}

void AVolumeSpawnerActor::SetColor(const AStaticMeshActor* SpawnedMesh) const
{
	auto VolumeSpawnerModule =
		FModuleManager::LoadModuleChecked<FVolumeSpawnerTestModule>("VolumeSpawnerTest");

	UMaterialInterface* const MaterialInterface = VolumeSpawnerModule.GetMaterialInterface();
	if (!MaterialInterface)
	{
		DebugUtils::Print(TEXT("Unable to get valid MaterialInterface"), FColor::Red);
		return;
	}

	const auto DynamicMaterial =
		SpawnedMesh->GetStaticMeshComponent()->CreateDynamicMaterialInstance(0, MaterialInterface);

	DynamicMaterial->SetVectorParameterValue(
		DynamicMaterial->VectorParameterValues[0].ParameterInfo.Name,
		FLinearColor::MakeRandomColor());
}

void AVolumeSpawnerActor::SpawnMeshes()
{
	EmptySpawnedMeshActorsArray();
	auto VolumeSpawnerModule =
		FModuleManager::LoadModuleChecked<FVolumeSpawnerTestModule>("VolumeSpawnerTest");

	if (!GetActorSubsystem())
	{
		DebugUtils::DisplayWarningMsg(TEXT("Unable to load ActorSubsystem"));
		return;
	}
	if (!MeshToSpawn)
	{
		DebugUtils::DisplayWarningMsg(TEXT("You must provide mesh asset to spawn with"));
		return;
	}
	if (NumberOfSpawns <= 0)
	{
		DebugUtils::DisplayWarningMsg(TEXT("Number of spawns must be greater than 0"));
		return;
	}

	//if no name is specified then MeshAsset name will do
	if (InitialName.IsEmpty())
	{
		InitialName = MeshToSpawn->GetName();
	}
	for (int i = 0; i < NumberOfSpawns; ++i)
	{
		const FVector RandomLocation = GetRandomLocationWithinBounds();
		const FRotator RandomRotator = UKismetMathLibrary::RandomRotator();
		const float RandomUniformScale = FMath::FRandRange(.4f, 2.5f);

		const auto SpawnedMesh =
			Cast<AStaticMeshActor>(EditorActorSubsystem->SpawnActorFromClass(
				AStaticMeshActor::StaticClass(),
				RandomLocation,
				bRandomizeRotation ? RandomRotator : FRotator()));

		if (!SpawnedMesh)
		{
			DebugUtils::Print("Mesh number " 
			+ FString::FromInt(i)
			+ " didn't manage to spawn and will be omited!",
			FColor::Red);
			
			continue;;
		}
		SpawnedMesh->SetActorScale3D(FVector(RandomUniformScale) * ScaleFactor);
		SpawnedMesh->SetActorLabel(InitialName + "_" + FString::FromInt(i));
		SpawnedMesh->GetStaticMeshComponent()->SetStaticMesh(MeshToSpawn);
		SetColor(SpawnedMesh);

		SpawnedMeshActors.Add(SpawnedMesh);
	}
	VolumeSpawnerModule.PushToDataTable(SpawnedMeshActors);
}

AVolumeSpawnerActor::AVolumeSpawnerActor()
{
}
