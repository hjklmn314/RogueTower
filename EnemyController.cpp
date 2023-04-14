#include "EnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AnimatorComponent.h"
#include "PlayerController.h"

// 设置默认值
AEnemyController::AEnemyController()
{
	// 设置此 empty，以调用每个帧的 Tick()。如果不需要，可以禁用此功能以提高性能。
	PrimaryActorTick.bCanEverTick = true;

	// Set the default values
	Speed = 5.f;
	Health = 100.f;
	Damage = 10.f;
	AttackRange = 100.f;
	AttackInterval = 1.f;
	IsDead = false;
}

// 已在游戏开始或生成时调用
void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	// Get the character movement component
	Movement = GetCharacterMovement();

	// Get the animator component
	Animator = FindComponentByClass<UAnimatorComponent>();

	// Get the player reference
	Player = Cast<APlayerController>(UGameplayStatics::GetPlayerCharacter(this, 0));

	// Start the attack timer
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemyController::Attack, AttackInterval, true);
}

// 已调用每个帧
void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead) return;

	// Move the enemy towards the player
	FVector Direction = Player->GetActorLocation() - GetActorLocation();
	Direction.Z = 0.f;
	Direction.Normalize();
	Movement->Velocity = Direction * Speed;

	// Flip the enemy sprite according to the direction
	if (Direction.X > 0) {
		SetActorScale3D(FVector(1.f, 1.f, 1.f));
	}
	else if (Direction.X < 0) {
		SetActorScale3D(FVector(-1.f, 1.f, 1.f));
	}

	// Update the animator parameters
	Animator->SetFloat("Speed", Movement->Velocity.Size());
}

// 调用以绑定要输出的功能
void AEnemyController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AEnemyController::OnTriggerEnter2D(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the enemy collides with the player's attack
	if (OtherActor == Player && OtherComp->ComponentHasTag("Attack")) {
		// Reduce the health by the player's damage
		Health -= Player->GetDamage();
		// Play the hurt animation
		Animator->SetTrigger("Hurt");
		// Check if the health is zero or below
		if (Health <= 0) {
			// Play the death animation and stop the movement
			Animator->SetTrigger("Die");
			Movement->Velocity = FVector::ZeroVector;
			Movement->DisableMovement();
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetWorldTimerManager().ClearTimer(AttackTimer);
			IsDead = true;
			// Destroy the enemy after 3 seconds
			SetLifeSpan(3.f);
		}
	}
}

void AEnemyController::Attack()
{
	// Check if the player is within the attack range
	float Distance = FVector::Distance(GetActorLocation(), Player->GetActorLocation());
	if (Distance <= AttackRange) {
		// Play the attack animation
		Animator->SetTrigger("Attack");
		// Apply damage to the player
		Player->TakeDamage(Damage);
	}
}
