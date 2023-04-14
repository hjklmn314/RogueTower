
#include "UIController.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController.h"



void UUIController::NativeConstruct()
{
    Super::NativeConstruct();

    // Get the text block components
    ScoreText = Cast<UTextBlock>(GetWidgetFromName("ScoreText"));
    HighScoreText = Cast<UTextBlock>(GetWidgetFromName("HighScoreText"));

    // Get the button components
    RestartButton = Cast<UButton>(GetWidgetFromName("RestartButton"));
    QuitButton = Cast<UButton>(GetWidgetFromName("QuitButton"));

    // Bind the button events
    RestartButton->OnClicked.AddDynamic(this, &UUIController::OnRestartClicked);
    QuitButton->OnClicked.AddDynamic(this, &UUIController::OnQuitClicked);

    // Hide the buttons initially
    RestartButton->SetVisibility(ESlateVisibility::Hidden);
    QuitButton->SetVisibility(ESlateVisibility::Hidden);

    // Get the player reference
    Player = Cast<APlayerController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    // Get the game manager reference
    GameManager = Cast<AGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass()));
}

void UUIController::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // Update the score text
    ScoreText->SetText(FText::FromString(FString::FromInt(GameManager->GetScore())));

    // Update the high score text
    HighScoreText->SetText(FText::FromString(FString::FromInt(GameManager->GetHighScore())));

    // Show the buttons if the player is dead
    if (Player->IsDead()) {
        RestartButton->SetVisibility(ESlateVisibility::Visible);
        QuitButton->SetVisibility(ESlateVisibility::Visible);
    }
}

void UUIController::OnRestartClicked()
{
    // Reload the current level
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void UUIController::OnQuitClicked()
{
    // Quit the game
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}
```