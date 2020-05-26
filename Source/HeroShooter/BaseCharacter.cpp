// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"

#include "UI/Healthbar.h"
#include "HeroAnimInstance.h"
#include "HealthComponent.h"
#include "Weapon.h"
#include "CustomGameplayAbility.h"
#include "PassiveGameplayAbility.h"
#include "PassiveAbilityComponent.h"
#include "ActiveGameplayAbility.h"
#include "AttributeSetBase.h"
#include "CustomMacros.h"
#include "HeroPlayerState.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(true);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("HealthComponent"));
	validate(IsValid(HealthComponent));

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(FName("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);


	AttributeSet = CreateDefaultSubobject<UAttributeSetBase>(FName("Attribute Set"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	if (validate(IsValid(SkeletalMesh)) == false) { return; }

	HeroAnimInstance = Cast<UHeroAnimInstance>(SkeletalMesh->GetAnimInstance());
	if (validate(IsValid(HeroAnimInstance)) == false) { return; }

	if (validate(IsValid(HealthbarWidgetComponent)) == false) { return; }
	ExternalHealthbar = Cast<UHealthbar>(HealthbarWidgetComponent->GetUserWidgetObject());
	if (validate(IsValid(ExternalHealthbar)) == false) { return; }
	if (validate(IsValid(HealthComponent)) == false) { return; }
	ExternalHealthbar->Setup(HealthComponent->GetMaxHealth());
	HealthComponent->OnHealthChanged.AddDynamic(this, &ABaseCharacter::UpdateExternalHealthbar);
	HealthComponent->OnDeath.AddDynamic(this, &ABaseCharacter::Die);

	if (validate(IsValid(AttributeSet)) == false) { return; }
	AttributeSet->Setup();
	
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
		SetupEquippedWeapon();

		HeroAnimInstance->OnReloadFinished.AddDynamic(this, &ABaseCharacter::Reload);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has no weapon spawned."), *GetName());
	}
}


void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(Cast<APlayerController>(GetController())) && IsLocallyControlled()) {
		float MoveForwardValue = GetInputAxisValue("MoveForward");
		MoveForward(MoveForwardValue);

		float MoveRightValue = GetInputAxisValue("MoveRight");
		MoveRight(MoveRightValue);

		float X = GetInputAxisValue("LookUp");
		float Y = GetInputAxisValue("LookRight");
		FRotator DeltaRotation = FRotator(X, Y, 0);

		APlayerController* PlayerController = Cast<APlayerController>(GetController());

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

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABaseCharacter::AttemptReload);
	PlayerInputComponent->BindAction("CancelReload", IE_Pressed, this, &ABaseCharacter::CancelReload);

	for (TSubclassOf<UCustomGameplayAbility> Ability : Abilities) {
		AcquireAbility(Ability);
	}
}


void ABaseCharacter::OnRep_WeaponSpawn() {
	SetupEquippedWeapon();
}

void ABaseCharacter::CancelReload() {
	if (bReloading == false) { return; }

	if (validate(IsValid(HeroAnimInstance)) == false) { return; }
	HeroAnimInstance->CancelReload();

	bReloading = false;
}

void ABaseCharacter::AttemptReload() {
	if (bReloading) { return; }

	if (validate(IsValid(HeroAnimInstance)) == false) { return; }
	HeroAnimInstance->StartReload();
	ServerReload();

	bReloading = true;
}


void ABaseCharacter::Reload() {
	if (validate(IsValid(Weapon)) == false) { return; }
	Weapon->Reload();
	bReloading = false;
}

void ABaseCharacter::ServerReload_Implementation() {
	if (validate(IsValid(HeroAnimInstance)) == false) { return; }
	HeroAnimInstance->StartReload();

	NetMulticastReload();
}

void ABaseCharacter::NetMulticastReload_Implementation() {
	if (Role == ROLE_AutonomousProxy || Role == ROLE_Authority) { return; } // Already done in attempt reload

	if (validate(IsValid(HeroAnimInstance)) == false) { return; }
	HeroAnimInstance->StartReload();
}



void ABaseCharacter::ServerCancelReload_Implementation() {
	if (validate(IsValid(HeroAnimInstance)) == false) { return; }
	HeroAnimInstance->CancelReload();

	NetMulticastCancelReload();
}

void ABaseCharacter::NetMulticastCancelReload_Implementation() {
	if (validate(IsValid(HeroAnimInstance)) == false) { return; }
	HeroAnimInstance->CancelReload();
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

AWeapon* ABaseCharacter::GetEquippedWeapon() {
	return Weapon;
}


void ABaseCharacter::HideHead() {
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	if (validate(IsValid(SkeletalMesh)) == false) { return; }
	int BoneIndex = SkeletalMesh->GetBoneIndex(HeadBoneName);
	if (validate(BoneIndex != INDEX_NONE) == false) { return; }
	SkeletalMesh->HideBoneByName(HeadBoneName, EPhysBodyOp::PBO_MAX);
}


void ABaseCharacter::UpdateExternalHealthbar(int NewHealth) {
	ExternalHealthbar->Update(NewHealth);
}


void ABaseCharacter::SetHealthWidgetComponent(UWidgetComponent* HealthbarWidgetComponent) {
	this->HealthbarWidgetComponent = HealthbarWidgetComponent;
}


void ABaseCharacter::Die() {
	UE_LOG(LogTemp, Warning, TEXT("Die"))
	if (validate(IsValid(HeroAnimInstance)) == false) { return; }
	HeroAnimInstance->OnDeath();
}


UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}


void ABaseCharacter::AddGameplayTag(FGameplayTag Tag) {
	GetAbilitySystemComponent()->SetLooseGameplayTagCount(Tag, 1);
}


void ABaseCharacter::RemoveGameplayTag(FGameplayTag Tag) {
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(Tag);
}


void ABaseCharacter::AcquireAbility(TSubclassOf<UCustomGameplayAbility> AbilityToAcquire) {
	if (validate(IsValid(AbilitySystemComponent)) == false) { return; }
	if (validate(AbilityToAcquire != nullptr) == false) { return; }

	if (HasAuthority() == false) { return; }

	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToAcquire));
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->ReplicationMode = EGameplayEffectReplicationMode::Mixed;

	TSubclassOf<UActiveGameplayAbility> ActiveAbilityToAcquire(AbilityToAcquire);
	if (IsValid(ActiveAbilityToAcquire)) {

		if (ActiveAbilities.Contains(ActiveAbilityToAcquire) == false) {
			ActiveAbilities.Add(ActiveAbilityToAcquire);
		}
		int AbilityIndex = ActiveAbilities.Find(ActiveAbilityToAcquire);

		AController* Controller = GetController();
		if (validate(IsValid(Controller)) == false) { return; }

		UInputComponent* InputComponent = Controller->InputComponent;
		if (IsValid(InputComponent)) {
			UActiveGameplayAbility* ActiveGameplayAbility = Cast<UActiveGameplayAbility>(ActiveAbilityToAcquire.GetDefaultObject());
			if (validate(IsValid(ActiveGameplayAbility)) == false) { return; }

			FInputActionBinding ActionBinding(
				ActiveGameplayAbility->GetInputActionName(), ActiveGameplayAbility->GetActivationInputEvent()
			);

			FInputActionHandlerSignature Delegate;
			Delegate.BindUFunction(this, FName("ActivateAbility"), AbilityIndex);
			ActionBinding.ActionDelegate = Delegate;

			InputComponent->AddActionBinding(ActionBinding);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Active ability not bound to input!"))
		}
	}
	else
	{
		TSubclassOf<UPassiveGameplayAbility> PassiveAbilityToAcquire(AbilityToAcquire);
		if (validate(IsValid(PassiveAbilityToAcquire)) == false) {
			UE_LOG(LogTemp, Error, TEXT("Acquired ability is neither active nor passive!"))
			return;
		}

		UPassiveGameplayAbility* DefaultAbility = PassiveAbilityToAcquire.GetDefaultObject();
		if (validate(IsValid(DefaultAbility)) == false) { return; }

		TSubclassOf<UPassiveAbilityComponent> PassiveComponentTemplate = DefaultAbility->GetPassiveAbilityComponentTemplate();
		if (validate(IsValid(PassiveComponentTemplate)) == false) { return; };

		UPassiveAbilityComponent* DefaultComponent = Cast<UPassiveAbilityComponent>(PassiveComponentTemplate->GetDefaultObject());
		if (validate(IsValid(DefaultComponent)) == false) { return; }

		UPassiveAbilityComponent* InstantiatedComponent = Cast<UPassiveAbilityComponent>(CreateComponentFromTemplate(
			DefaultComponent, DefaultComponent->GetFName()
		));
		if (validate(IsValid(InstantiatedComponent)) == false) { return; }
		InstantiatedComponent->SetAbility(PassiveAbilityToAcquire);
	}
}


void ABaseCharacter::ActivateAbility(int AbilityIndex) {
	if (HasAuthority() == false) { return; }

	if (validate(ActiveAbilities.Num() >= AbilityIndex) == false) { return; }
	TSubclassOf<UGameplayAbility> Ability = ActiveAbilities[AbilityIndex];
	if (validate(Ability != nullptr) == false) { return; }
	GetAbilitySystemComponent()->TryActivateAbilityByClass(Ability);
}

UBehaviorTree* ABaseCharacter::GetAIBehaviorTreeForCurrentGamemode() {
	return BehaviourTree;
}


void ABaseCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	// ASC MixedMode replication requires that the ASC Owner's Owner be the Controller.
	SetOwner(NewController);
}


void ABaseCharacter::SetupEquippedWeapon() {
	AWeapon* NewWeapon = GetEquippedWeapon();
	if (IsValid(NewWeapon) == false) { return; }
	if (validate(IsValid(HeroAnimInstance)) == false) { return; }
	OnWeaponChange.Broadcast(NewWeapon);
	HeroAnimInstance->OnReloadFinished.AddDynamic(NewWeapon, &AWeapon::Reload);
}



int ABaseCharacter::GetTeamIndex() {
	AController* OwnerController = GetController();
	if (validate(IsValid(OwnerController)) == false) { return -1; }

	AHeroPlayerState* HeroPlayerState = OwnerController->GetPlayerState<AHeroPlayerState>();
	if (validate(IsValid(HeroPlayerState)) == false) { return -1; }

	return HeroPlayerState->GetTeamIndex();
}