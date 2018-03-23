// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintQuaternionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class EMA_VR_REHAB_API UBlueprintQuaternionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
	UFUNCTION(BlueprintCallable)
	static FQuat Add(FQuat Quat1, FQuat Quat2);

	UFUNCTION(BlueprintCallable)
	static FQuat Subtract(FQuat Quat1, FQuat Quat2);

	UFUNCTION(BlueprintCallable)
	static FQuat Multiply(FQuat Quat1, FQuat Quat2);

	UFUNCTION(BlueprintCallable)
	static FQuat Inverse(FQuat Quat1);

	UFUNCTION(BlueprintCallable)
	static FQuat Rotate(FQuat Quat1, FQuat Quat2);

	UFUNCTION(BlueprintCallable)
	static FQuat RelativeRotation(FQuat Quat1, FQuat Quat2);

	UFUNCTION(BlueprintCallable)
	static FVector Euler(FQuat Quat1);
	
};
