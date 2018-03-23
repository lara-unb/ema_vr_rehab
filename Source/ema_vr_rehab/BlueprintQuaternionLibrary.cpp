// Fill out your copyright notice in the Description page of Project Settings.

#include "BlueprintQuaternionLibrary.h"

FQuat UBlueprintQuaternionLibrary::Add(FQuat Quat1, FQuat Quat2)
{
	FQuat QuatOut = Quat1 + Quat2;
	return QuatOut;
}

FQuat UBlueprintQuaternionLibrary::Subtract(FQuat Quat1, FQuat Quat2)
{
	FQuat QuatOut = Quat1 - Quat2;
	return QuatOut;
}

FQuat UBlueprintQuaternionLibrary::Multiply(FQuat Quat1, FQuat Quat2)
{
	FQuat QuatOut = Quat1 * Quat2;
	return QuatOut;
}

FQuat UBlueprintQuaternionLibrary::Inverse(FQuat Quat1)
{
	FQuat QuatOut = Quat1.Inverse();
	return QuatOut;
}

FQuat UBlueprintQuaternionLibrary::Rotate(FQuat Quat1, FQuat Quat2)
{
	FQuat QuatOut = Quat2 * Quat1 * Quat2.Inverse();
	return QuatOut;
}

FQuat UBlueprintQuaternionLibrary::RelativeRotation(FQuat Quat1, FQuat Quat2)
{
	FQuat QuatOut = Quat1.Inverse() * Quat2;
	return QuatOut;
}

FVector UBlueprintQuaternionLibrary::Euler(FQuat Quat1)
{
	FVector EulerOut = Quat1.Euler();
	return EulerOut;
}