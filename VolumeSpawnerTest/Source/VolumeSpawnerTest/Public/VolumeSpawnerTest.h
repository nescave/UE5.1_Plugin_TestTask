
#pragma once

#include "CoreMinimal.h"
struct FSpawnedActorData;
class FVolumeSpawnerTestModule : public IModuleInterface
{
	UMaterial* GetValidMaterial() const;
	void PrintSpawnSrceenMsg(const FSpawnedActorData* SpawnedActorData);
	UDataTable* GetValidDataTable(const FString& AssetName, FString& OutPathToDataTable);
public:

	UMaterialInterface* GetMaterialInterface();
	void PushToDataTable(const TArray<AStaticMeshActor*>& SpawnedMeshes);

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
