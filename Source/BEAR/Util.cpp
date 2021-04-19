// Fill out your copyright notice in the Description page of Project Settings.


#include "Util.h"

TArray<UActorComponent*> Util::GetComponentsByName(AActor* Actor, TSubclassOf<UActorComponent> ComponentClass, FString Name)
{
	TArray<UActorComponent*> result;

	auto components = Actor->GetComponents();

	for (auto component : components)
	{
		if (component->GetName() == Name)
		{
			result.Add(component);
		}
	}

	return result;
}

UActorComponent* Util::GetComponentByName(AActor* Actor, TSubclassOf<UActorComponent> ComponentClass, FString Name)
{
	TArray<UActorComponent*> components = GetComponentsByName(Actor, ComponentClass, Name);

	return components.Num() > 0 ? components[0] : nullptr;
}