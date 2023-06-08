#pragma once

#include "CoreMinimal.h"

class VOLUMESPAWNERTEST_API DebugUtils
{
public:
	DebugUtils();
	~DebugUtils();
	static void Print(const FString& Message, const FColor& Color = FColor::Cyan);
	static EAppReturnType::Type DisplayWarningMsg(const FString& Msg);
};
