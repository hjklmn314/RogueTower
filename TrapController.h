#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrapController.generated.h"

UCLASS(BlueprintType)
class ROGUETOWER_API ATrapController : public ACharacter
{
	GENERATED_BODY()

public:
	// 设置此 empty 属性的默认值
	ATrapController();

	// The box component for collision detection
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
		class UBoxComponent* Box;

	// The animator component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
		class UAnimatorComponent* Animator;

	// The damage of the trap
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float Damage;

	// The flag for the trap's active state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		bool Active;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the trap overlaps with another component
	UFUNCTION(BlueprintNativeEvent, Category = "Collision")
		void OnTriggerEnter2D(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);
	void OnTriggerEnter2D_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);
};






