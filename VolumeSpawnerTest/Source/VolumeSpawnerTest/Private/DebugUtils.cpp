#include "DebugUtils.h"
#include "Misc/MessageDialog.h"

DebugUtils::DebugUtils()
{
}

DebugUtils::~DebugUtils()
{
}

void DebugUtils::Print(const FString& Message, const FColor& Color)
{
	if (!GEngine) return;

	GEngine->AddOnScreenDebugMessage(-1, 5, Color, Message);
}

EAppReturnType::Type DebugUtils::DisplayWarningMsg(const FString& Msg)
{
	const FText MsgTitle = FText::FromString(TEXT("Warning!"));
	Print(Msg, FColor::Red);
	return FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Msg), &MsgTitle);
}
