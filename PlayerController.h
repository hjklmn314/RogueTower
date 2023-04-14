#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerController.generated.h"

UCLASS(BlueprintType)
class ROGUETOWER_API APlayerController : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerController();

	// The camera component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		class UCameraComponent* Camera;

	// The character movement component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		class UCharacterMovementComponent* Movement;

	// The animator component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
		class UAnimatorComponent* Animator;

	// The speed of the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float Speed;

	// The force of the character's jump
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float JumpForce;

	// The layer mask for the ground
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FName GroundLayer;

	// The damage of the character's attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float Damage;

	// The range of the character's attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float AttackRange;

	// The interval of the character's attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float AttackInterval;

	// The flag for the character's grounded state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		bool IsGrounded;

	// The flag for the character's dead state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		bool IsDead;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when the screen is touched
	void OnTouchPressed(ETouchIndex::Type FingerIndex, FVector Location);

	// Called when the screen is released
	void OnTouchReleased(ETouchIndex::Type FingerIndex, FVector Location);

	// Called when the character overlaps with another component
	UFUNCTION(BlueprintNativeEvent, Category = "Collision")
		void OnTriggerEnter2D(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);
	void OnTriggerEnter2D_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// Called when the character attacks
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void Attack();

private:
	// The start location of the swipe
	FVector StartSwipe;

	// The end location of the swipe
	FVector EndSwipe;

	// The timer handle for the attack interval
	FTimerHandle AttackTimer;
};