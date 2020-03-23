// Fill out your copyright notice in the Description page of Project Settings.


#include "PassiveAbilityComponent.h"
#include "PassiveGameplayAbility.h"

// Sets default values for this component's properties
UPassiveAbilityComponent::UPassiveAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPassiveAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPassiveAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UPassiveAbilityComponent::SetAbility(TSubclassOf<UPassiveGameplayAbility> AbilityTemplate) {
	PassiveAbilityTemplate = AbilityTemplate;
}

