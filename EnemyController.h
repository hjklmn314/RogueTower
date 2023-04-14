#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyController.generated.h"

UCLASS(BlueprintType)
class ROGUETOWER_API AEnemyController : public ACharacter
{
	GENERATED_BODY()

public:
	// 设置此 empty 属性的默认值
	// Sets default values for this character's properties
	AEnemyController();

	// The character movement component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		class UCharacterMovementComponent* Movement;

	// The animator component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
		class UAnimatorComponent* Animator;

	// The player reference
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		class APlayerController* Player;

	// The speed of the enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float Speed;

	// The health of the enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float Health;

	// The damage of the enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float Damage;

	// The range of the enemy's attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float AttackRange;

	// The interval of the enemy's attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float AttackInterval;

	// The flag for the enemy's dead state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		bool IsDead;

protected:
	// 已在游戏开始或生成时调用
	virtual void BeginPlay() override;

	// 已调用每个帧
	virtual void Tick(float DeltaTime) override;

	// 调用以绑定要输出的功能
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when the character overlaps with another component
	UFUNCTION(BlueprintNativeEvent, Category = "Collision")
		void OnTriggerEnter2D(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);
	    void OnTriggerEnter2D_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);

	// Called when the enemy attacks
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void Attack();

private:
	// The timer handle for the attack interval
	FTimerHandle AttackTimer;
};

