#include "RTSOwnerComponent.h"

#include "GameFramework/Controller.h"
#include "Net/UnrealNetwork.h"

#include "RTSPlayerState.h"
#include "RTSTeamInfo.h"


URTSOwnerComponent::URTSOwnerComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);

    InitialOwnerPlayerIndex = ARTSPlayerState::PLAYER_INDEX_NONE;
}

void URTSOwnerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTSOwnerComponent, PlayerOwner);
}


ARTSPlayerState* URTSOwnerComponent::GetPlayerOwner() const
{
	return PlayerOwner;
}

void URTSOwnerComponent::SetPlayerOwner(AController* Controller)
{
	ARTSPlayerState* PreviousOwner = PlayerOwner;

	if (!Controller)
	{
		PlayerOwner = nullptr;
	}
	else
	{
		PlayerOwner = Cast<ARTSPlayerState>(Controller->PlayerState);
	}

	if (PlayerOwner != PreviousOwner)
	{
		// Notify listeners.
		OnOwnerChanged.Broadcast(GetOwner(), Controller);
	}
}

bool URTSOwnerComponent::IsSameTeamAsActor(AActor* Other) const
{
	if (!Other)
	{
		return false;
	}

	ARTSPlayerState* MyOwner = GetPlayerOwner();

	if (!MyOwner)
	{
		return false;
	}

	URTSOwnerComponent* OtherOwnerComponent = Other->FindComponentByClass<URTSOwnerComponent>();

	if (!OtherOwnerComponent)
	{
		return false;
	}

	ARTSPlayerState* OtherOwner = OtherOwnerComponent->GetPlayerOwner();

	return MyOwner->IsSameTeamAs(OtherOwner);
}

bool URTSOwnerComponent::IsSameTeamAsController(AController* C) const
{
	ARTSPlayerState* MyOwner = GetPlayerOwner();
	ARTSPlayerState* OtherPlayer = Cast<ARTSPlayerState>(C->PlayerState);

	return MyOwner && MyOwner->IsSameTeamAs(OtherPlayer);
}

uint8 URTSOwnerComponent::GetInitialOwnerPlayerIndex()
{
    return InitialOwnerPlayerIndex;
}
