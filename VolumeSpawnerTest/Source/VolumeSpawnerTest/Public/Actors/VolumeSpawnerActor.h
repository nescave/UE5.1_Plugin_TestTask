#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "VolumeSpawnerActor.generated.h"

UCLASS()
class VOLUMESPAWNERTEST_API AVolumeSpawnerActor : public AVolume
{
	GENERATED_BODY()

	UPROPERTY()
	class UEditorActorSubsystem* EditorActorSubsystem;
	TArray<AStaticMeshActor*> SpawnedMeshActors;
	FVector GetRandomLocationWithinBounds() const;
	UEditorActorSubsystem* GetActorSubsystem();
	void EmptySpawnedMeshActorsArray();
	void SetColor(const AStaticMeshActor* SpawnedMesh) const;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnData")
	FString InitialName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnData")
	int NumberOfSpawns;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnData")
	float ScaleFactor = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnData")
	bool bRandomizeRotation = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnData")
	UStaticMesh* MeshToSpawn = nullptr;

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "SpawnData")
	void SpawnMeshes();
	
	AVolumeSpawnerActor();
};
