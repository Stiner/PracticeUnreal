// Practice Unreal by Stiner

#include "NetPlayGameMode.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NetPlayerController.h"

ANetPlayGameMode::ANetPlayGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANetPlayGameMode::OnDeadPlayerCharacter(AController* deadPlayer)
{
    ANetPlayerController* playerController = Cast<ANetPlayerController>(deadPlayer);
    if (IsValid(playerController))
    {
        UKismetSystemLibrary::QuitGame(this, playerController, EQuitPreference::Type::Quit, false);
    }
}

void ANetPlayGameMode::OnPostLogin(AController* NewPlayer)
{
    const FString msg = FString::Printf(TEXT("Logged in : %s"), *(NewPlayer->GetName()));
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, msg);
}
