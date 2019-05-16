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
	
	
	UFUNCTION(BlueprintPure)
	static FQuat Add(FQuat Quat1, FQuat Quat2);

	UFUNCTION(BlueprintPure)
	static FQuat Subtract(FQuat Quat1, FQuat Quat2);

	UFUNCTION(BlueprintPure)
	static FQuat Multiply(FQuat Quat1, FQuat Quat2);

	UFUNCTION(BlueprintPure)
	static FQuat Inverse(FQuat Quat1);

	UFUNCTION(BlueprintPure)
	static FQuat Rotate(FQuat Quat1, FQuat Quat2);

	UFUNCTION(BlueprintPure)
	static FQuat RelativeRotation(FQuat Quat1, FQuat Quat2);

	UFUNCTION(BlueprintPure)
	static FVector Euler(FQuat Quat1);

	UFUNCTION(BlueprintPure)
	static FQuat MakeFromEuler(FVector Vect1);

	UFUNCTION(BlueprintPure)
	static FVector EulerYZX(FQuat Quat1);

	UFUNCTION(BlueprintPure)
	static FVector EulerZYX(FQuat Quat1);

	UFUNCTION(BlueprintPure)
	static FVector EulerZXY(FQuat Quat1);

	UFUNCTION(BlueprintPure)
	static FQuat MakeFromEulerZXY(FVector Quat1);
};
