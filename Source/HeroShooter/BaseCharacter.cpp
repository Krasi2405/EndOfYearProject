// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SkeletalMeshComponent.h"

#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

#include "HealthComponent.h"
#include "Weapon.h"
#include "CustomMacros.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(true);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("HealthComponent"));
	validate(IsValid(HealthComponent));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	if (validate(IsValid(SkeletalMesh)) == false) { return; }

	if (IsLocallyControlled()) {
		SkeletalMesh->HideBoneByName(HeadBoneName, EPhysBodyOp::PBO_MAX);
	}

	if (IsValid(StartingWeaponTemplate)) {
		if (HasAuthority() == false) { return; }

		UWorld* World = GetWorld();
		if (validate(IsValid(World)) == false) { return; }

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.Instigator = GetInstigator();

		Weapon = World->SpawnActor<AWeapon>(StartingWeaponTemplate, SpawnInfo);
		if (validate(IsValid(Weapon)) == false) { return; }
		Weapon->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GunSocket");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has no weapon spawned."), *GetName());
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled() && IsValid(GetController())) {
		float MoveForwardValue = GetInputAxisValue("MoveForward");
		MoveForward(MoveForwardValue);

		float MoveRightValue = GetInputAxisValue("MoveRight");
		MoveRight(MoveRightValue);

		float X = GetInputAxisValue("LookUp");
		float Y = GetInputAxisValue("LookRight");
		FRotator DeltaRotation = FRotator(X, Y, 0);

		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (validate(IsValid(PlayerController)) == false) { return; }

		FRotator NewRotation = PlayerController->GetControlRotation() + DeltaRotation;
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, MinControlPitch, MaxControlPitch);
		PlayerController->SetControlRotation(NewRotation);
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (validate(IsValid(PlayerInputComponent)) == false) { return; }

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABaseCharacter::StartFiring);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABaseCharacter::StopFiring);

	PlayerInputComponent->BindAxis("MoveForward");
	PlayerInputComponent->BindAxis("MoveRight");

	PlayerInputComponent->BindAxis("LookUp");
	PlayerInputComponent->BindAxis("LookRight");

}


void ABaseCharacter::StartFiring() {
	Weapon->PullTrigger();
}


void ABaseCharacter::StopFiring() {
	Weapon->ReleaseTrigger();
}


void ABaseCharacter::MoveForward(float Value)
{
	if (validate(IsValid(Controller)) == false) { return; }
	if (Value == 0.0f) { return; }
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void ABaseCharacter::MoveRight(float Value)
{
	if (validate(IsValid(Controller)) == false) { return; }
	if (Value == 0.0f) { return; }

	// find out which way is right
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	// add movement in that direction
	AddMovementInput(Direction, Value);
}


void ABaseCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, Weapon);
}


void ABaseCharacter::Destroyed() {
	Super::Destroyed();

	if (IsValid(Weapon)) {
		Weapon->Destroy();
	}
}