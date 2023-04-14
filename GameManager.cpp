#include "GameManager.h"
#include "Kismet/GameplayStatics.h"

// 设置默认值
AGameManager::AGameManager()
{
	// 设置此 empty，以调用每个帧的 Tick()。如果不需要，可以禁用此功能以提高性能。
	PrimaryActorTick.bCanEverTick = true;

	// Set the default values
	Score = 0;
	HighScore = 0;
	ScoreInterval = 1.f;
}

// 已在游戏开始或生成时调用
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	// Load the high score from the save file
	USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot("SaveSlot", 0);
	if (SaveGame) {
		HighScore = Cast<UGameSave>(SaveGame)->HighScore;
	}

	// Start the score timer
	GetWorldTimerManager().SetTimer(ScoreTimer, this, &AGameManager::IncreaseScore, ScoreInterval, true);
}

// 已调用每个帧
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameManager::IncreaseScore()
{
	// Increase the score by one
	Score++;
}

int AGameManager::GetScore()
{
	// Return the current score
	return Score;
}

int AGameManager::GetHighScore()
{
	// Return the high score
	return HighScore;
}


void AGameManager::SaveHighScore()
{
	// Check if the current score is higher than the high score
	if (Score > HighScore) {
		// Update the high score and save it to the save file
		HighScore = Score;
		USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(UGameSave::StaticClass());
		Cast<UGameSave>(SaveGame)->HighScore = HighScore;
		UGameplayStatics::SaveGameToSlot(SaveGame, "SaveSlot", 0);
	}
}
