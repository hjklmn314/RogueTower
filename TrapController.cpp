#include "TrapController.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AnimatorComponent.h"
#include "PlayerController.h"

// 设置默认值
ATrapController::ATrapController()
{
	// 设置此 empty，以调用每个帧的 Tick()。如果不需要，可以禁用此功能以提高性能。
	PrimaryActorTick.bCanEverTick = true;

	// Create a box component for collision detection
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetCollisionProfileName("Trap");
	Box->OnComponentBeginOverlap.AddDynamic(this, &ATrapController::OnTriggerEnter2D);
	RootComponent = Box;

	// Set the default values
	Damage = 20.f;
	Active = true;
}

// 已在游戏开始或生成时调用
void ATrapController::BeginPlay()
{
	Super::BeginPlay();

	// Get the animator component
	Animator = FindComponentByClass<UAnimatorComponent>();
}

// 已调用每个帧
void ATrapController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void ATrapController::OnTriggerEnter2D(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the trap collides with the player
	if (OtherActor->IsA(APlayerController::StaticClass()) && Active) {
		// Play the trigger animation
		Animator->SetTrigger("Trigger");
		// Apply damage to the player
		OtherActor->TakeDamage(Damage);
		// Deactivate the trap
		Active = false;
	}
}


/* Path: Source\RogueTower\TrapController.cpp
#include "TrapController.h"
#include "Components/BoxComponent.h"
#include "Components/AnimatorComponent.h"
#include "PlayerController.h"

// Sets default values
ATrapController::ATrapController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create the box component
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	// Create the animator component
	Animator = CreateDefaultSubobject<UAnimatorComponent>(TEXT("Animator"));
	Animator->SetupAttachment(RootComponent);
	// Set the damage of the trap
	Damage = 1.0f;
	// Set the trap to be active
	Active = true;
}

// Called when the game starts or when spawned
void ATrapController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATrapController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the trap overlaps with another component
void ATrapController::OnTriggerEnter2D_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the trap is active
	if (Active)
	{
		// Check if the other actor is a player
		if (OtherActor->IsA(APlayerController::StaticClass()))
		{
			// Cast the other actor to a player
			APlayerController* Player = Cast<APlayerController>(OtherActor);
			// Check if the player is not null
			if (Player)
			{
				// Deal damage to the player
				Player->TakeDamage(Damage);
			}
		}
	}
}*/