#include "PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AnimatorComponent.h"
#include "Kismet/GameplayStatics.h"

// 设置默认值
APlayerController::APlayerController()
{
	// 设置此 empty，以调用每个帧的 Tick()。如果不需要，可以禁用此功能以提高性能。
    // Set this character to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Create a camera component
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(RootComponent);
    Camera->SetRelativeLocation(FVector(-300.f, 0.f, 300.f));
    Camera->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

    // Set the default values
    Speed = 10.f;
    JumpForce = 300.f;
    GroundLayer = FCollisionResponseParams::DefaultResponseParam;
    IsGrounded = false;
    IsDead = false;
}

// 已在游戏开始或生成时调用
void APlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Get the character movement component
    Movement = GetCharacterMovement();

    // Get the animator component
    Animator = FindComponentByClass<UAnimatorComponent>();
}

// 已调用每个帧
void APlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsDead) return;

    // Move the player horizontally
    float h = InputComponent->GetAxisValue("Horizontal");
    Movement->Velocity = FVector(h * Speed, 0.f, Movement->Velocity.Z);

    // Flip the player sprite according to the direction
    if (h > 0) {
        SetActorScale3D(FVector(1.f, 1.f, 1.f));
    }
    else if (h < 0) {
        SetActorScale3D(FVector(-1.f, 1.f, 1.f));
    }

    // Check if the player is touching the ground
    IsGrounded = GetCapsuleComponent()->IsOverlappingComponent(GroundLayer);

    // Make the player jump if the screen is swiped up
    if (InputComponent->GetTouchState(ETouchIndex::Touch1, StartSwipe.X, StartSwipe.Y)) {
        StartSwipe = UGameplayStatics::DeprojectScreenToWorld(this, FVector2D(StartSwipe.X, StartSwipe.Y));
        if (StartSwipe.Z > GetActorLocation().Z && IsGrounded) {
            Movement->AddImpulse(FVector(0.f, 0.f, JumpForce));
        }
    }

    // Make the player attack if the screen is swiped down
    if (InputComponent->GetTouchState(ETouchIndex::Touch1, EndSwipe.X, EndSwipe.Y)) {
        EndSwipe = UGameplayStatics::DeprojectScreenToWorld(this, FVector2D(EndSwipe.X, EndSwipe.Y));
        if (EndSwipe.Z < GetActorLocation().Z) {
            Animator->SetTrigger("Attack");
        }
    }

    // Update the animator parameters
    Animator->SetFloat("Speed", FMath::Abs(h));
    Animator->SetBool("Grounded", IsGrounded);
}

// 调用以绑定要输出的功能

void APlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind the horizontal axis to move left and right
    PlayerInputComponent->BindAxis("Horizontal");

    // Bind the touch events to swipe up and down
    PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &APlayerController::OnTouchPressed);
    PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &APlayerController::OnTouchReleased);
}


void APlayerController::OnTouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
    // Store the start location of the swipe
    StartSwipe = Location;
}

void APlayerController::OnTouchReleased(ETouchIndex::Type FingerIndex, FVector Location)
{
    // Store the end location of the swipe
    EndSwipe = Location;
}

void APlayerController::OnTriggerEnter2D(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& Sweep)
    {
        // Check if the player collides with an enemy or a trap
        if (OtherActor->ActorHasTag("Enemy") || OtherActor->ActorHasTag("Trap")) {
            // Play the death animation and stop the movement
            Animator->SetTrigger("Die");
            Movement->Velocity = FVector::ZeroVector;
            Movement->DisableMovement();
            IsDead = true;
            // Restart the game after 3 seconds
            GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::Restart, 3.f, false);
        }
    }

    void APlayerController::Restart()
    {
        // Reload the current level
        UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
    }