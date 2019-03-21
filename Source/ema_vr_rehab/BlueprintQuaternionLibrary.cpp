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
	/*(x, y, z, w) = (q[0], q[1], q[2], q[3])
    t0 = +2.0 * (w * x + y * z)
    t1 = +1.0 - 2.0 * (x * x + y * y)
    roll = math.atan2(t0, t1)
    t2 = +2.0 * (w * y - z * x)
    t2 = +1.0 if t2 > +1.0 else t2
    t2 = -1.0 if t2 < -1.0 else t2
    pitch = math.asin(t2)
    t3 = +2.0 * (w * z + x * y)
    t4 = +1.0 - 2.0 * (y * y + z * z)
    yaw = math.atan2(t3, t4)
    return [roll, pitch, yaw]*/
	float t[5];
	t[0] = 2.0 * (Quat1.W * Quat1.X + Quat1.Y * Quat1.Z);
	t[1] = 1.0 - 2.0 * (Quat1.X * Quat1.X + Quat1.Y * Quat1.Y);
	t[2] = 2.0 * (Quat1.W * Quat1.Y - Quat1.Z * Quat1.X);
	t[2] = fabsf(t[2]) > 1.0 ? powf(-1.0, signbit(t[2]))/*t[2]/fabsf(t[2])*/ : t[2];
	t[3] = 2.0 * (Quat1.W * Quat1.Z + Quat1.X * Quat1.Y);
	t[4] = 1.0 - 2.0 * (Quat1.Y * Quat1.Y + Quat1.Z * Quat1.Z);
	FVector EulerOut = FVector(atan2f(t[0], t[1]), asinf(t[2]), atan2f(t[3], t[4])) * 180.0 / PI;
	//FVector EulerOut = Quat1.Euler();
	return EulerOut;
}

FQuat UBlueprintQuaternionLibrary::MakeFromEuler(FVector Vect1)
{
	FQuat QuatOut = FQuat::MakeFromEuler(Vect1);
	return QuatOut;
}