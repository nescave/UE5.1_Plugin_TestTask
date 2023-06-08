#include "VolumeSpawnerTest.h"
#include "AssetToolsModule.h"
#include "DebugUtils.h"
#include "EditorAssetLibrary.h"
#include "SpawnedActorData.h"
#include "Engine/StaticMeshActor.h"
#include "Factories/DataTableFactory.h"
#include "Factories/MaterialFactoryNew.h"
#include "Factories/MaterialInstanceConstantFactoryNew.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialInstanceConstant.h"

#define LOCTEXT_NAMESPACE "FVolumeSpawnerTestModule"


//Print formated data to screen. Scale is uniform for each asset.  
void FVolumeSpawnerTestModule::PrintSpawnSrceenMsg(const FSpawnedActorData* SpawnedActorData)
{
	DebugUtils::Print("ActorName: " + SpawnedActorData->MeshName
		+ " ActorPosition: x=" + FString::FromInt(SpawnedActorData->MeshPosition.X)
		+ " y=" + FString::FromInt(SpawnedActorData->MeshPosition.Y)
		+ " z=" + FString::FromInt(SpawnedActorData->MeshPosition.Z)
		+ " ActorRotation: x=" + FString::FromInt(SpawnedActorData->MeshRotation.Roll)
		+ " y=" + FString::FromInt(SpawnedActorData->MeshRotation.Pitch)
		+ " z=" + FString::FromInt(SpawnedActorData->MeshRotation.Yaw)
		+ " ActorUniformScale: " + FString::FromInt(SpawnedActorData->MeshScale.X));
}

//if can't find Material under specified path (for convenience its hardcoded but it can be easily parametrized)
//creates and saves proper one
UMaterial* FVolumeSpawnerTestModule::GetValidMaterial() const
{
	const FString PathToMaterial = "/Game/M_UnLitColor";
	UMaterial* Material;
	if (UEditorAssetLibrary::DoesAssetExist(PathToMaterial))
	{
		Material = Cast<UMaterial>(UEditorAssetLibrary::FindAssetData(PathToMaterial).GetAsset());
	}
	else
	{
		const auto& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
		UMaterialFactoryNew* MaterialFactory = NewObject<UMaterialFactoryNew>();
		UMaterial* CreatedMaterial = Cast<UMaterial>(
			AssetToolsModule.Get().CreateAsset("M_UnLitColor", "/Game/", UMaterial::StaticClass(), MaterialFactory));
		
		if (!CreatedMaterial) return nullptr;
		CreatedMaterial->SetShadingModel(MSM_Unlit);
		UMaterialExpressionVectorParameter* VectorParameter =
			NewObject<UMaterialExpressionVectorParameter>(CreatedMaterial);
		
		VectorParameter->ParameterName = FName("Color");
		CreatedMaterial->GetExpressionCollection().AddExpression(VectorParameter);
		CreatedMaterial->GetExpressionInputForProperty(MP_EmissiveColor)->Connect(0, VectorParameter);
		CreatedMaterial->PostEditChange();
		UEditorAssetLibrary::SaveAsset(PathToMaterial);
		Material = CreatedMaterial;
	}
	return Material;
}

//if can't find final MaterialInterface creates one from Material gotten from GetValidMaterial()
UMaterialInterface* FVolumeSpawnerTestModule::GetMaterialInterface()
{
	const FString PathToMaterial = "/Game/MI_UnLitColor";
	UMaterialInstanceConstant* MaterialInstance;
	if (UEditorAssetLibrary::DoesAssetExist(PathToMaterial))
	{
		MaterialInstance = Cast<UMaterialInstanceConstant>(
			UEditorAssetLibrary::FindAssetData(PathToMaterial).GetAsset());
	}
	else
	{
		UMaterial* const Material = GetValidMaterial();
		const auto& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
		const auto MIConstantFactory = NewObject<UMaterialInstanceConstantFactoryNew>();
		MIConstantFactory->InitialParent = Cast<UMaterialInterface>(Material);
		const auto CreatedInstance = Cast<UMaterialInstanceConstant>(
			AssetToolsModule.Get().CreateAsset("MI_UnLitColor", "/Game/", UMaterialInstanceConstant::StaticClass(),
			                                   MIConstantFactory));
		if (!CreatedInstance) return nullptr;
		CreatedInstance->PostEditChange();
		UEditorAssetLibrary::SaveAsset(PathToMaterial);
		MaterialInstance = CreatedInstance;
	}
	return Cast<UMaterialInterface>(MaterialInstance);
}

//creating new or replacing old DataTable that was constructed based on same mesh type
UDataTable* FVolumeSpawnerTestModule::GetValidDataTable(const FString& AssetName, FString& OutPathToDataTable)
{
	const auto& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

	const FString PathToDataTableFolder = "/Game/";
	FString DataTableName;
	AssetName.Split("_", &DataTableName, nullptr); //Remove possible preffix (SM_)
	DataTableName += "_SpawnDataTable";

	UDataTableFactory* const DataTableFactory = NewObject<UDataTableFactory>();
	DataTableFactory->Struct = FSpawnedActorData::StaticStruct();
	UDataTable* const DataTable = Cast<UDataTable>(
		AssetToolsModule.Get().CreateAsset(DataTableName, PathToDataTableFolder, UDataTable::StaticClass(),
		                                   DataTableFactory));
	OutPathToDataTable = PathToDataTableFolder + DataTableName;
	return DataTable;
}

//spawned meshes get parsed to the acquired DataTable
void FVolumeSpawnerTestModule::PushToDataTable(const TArray<AStaticMeshActor*>& SpawnedMeshActors)
{
	FString PathToDataTable;
	UDataTable* const DataTable = GetValidDataTable(SpawnedMeshActors[0]->GetActorLabel(), PathToDataTable);

	if (!DataTable)
	{
		DebugUtils::DisplayWarningMsg(TEXT("Unable to get valid DataTable"));
		return;
	}
	for (const auto& SpawnedMesh : SpawnedMeshActors)
	{
		const FVector MeshLocation = SpawnedMesh->GetActorLocation();
		const FRotator MeshRotation = SpawnedMesh->GetActorRotation();
		const double MeshUniformScale = SpawnedMesh->GetActorScale().X;

		FSpawnedActorData ActorsData;
		ActorsData.MeshName = SpawnedMesh->GetActorLabel();
		ActorsData.MeshPosition = MeshLocation;
		ActorsData.MeshRotation = MeshRotation;
		ActorsData.MeshScale = FVector(MeshUniformScale);
		ActorsData.StaticMesh = SpawnedMesh->GetStaticMeshComponent()->GetStaticMesh();

		PrintSpawnSrceenMsg(&ActorsData);
		DataTable->AddRow(FName(ActorsData.MeshName), ActorsData);
	}
	UEditorAssetLibrary::SaveAsset(PathToDataTable);
}

void FVolumeSpawnerTestModule::StartupModule()
{
}

void FVolumeSpawnerTestModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FVolumeSpawnerTestModule, VolumeSpawnerTest)
