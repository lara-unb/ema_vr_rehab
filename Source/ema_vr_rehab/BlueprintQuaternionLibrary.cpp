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
	// QuatOut.Inverse() = Quat2.Inverse() * Quat1;
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
	float t[5], roll, pitch, yaw, test, test_signal;
	test = Quat1.X * Quat1.Y + Quat1.Z * Quat1.W;

	if (fabsf(test) == 0.985 /*sin(85 degrees)*/) // Singularity test
	{
		test_signal = powf(-1.0, signbit(test)); /*test/fabsf(test)*/
		roll = test_signal * 2.0 * atan2f(Quat1.X, Quat1.Y);
		pitch = test_signal * PI / 2;
		yaw = 0;
	}
	else
	{
		t[0] = Quat1.W * Quat1.X + Quat1.Y * Quat1.Z;
		t[1] = 0.5 - (Quat1.X * Quat1.X + Quat1.Y * Quat1.Y);
		t[2] = 2.0 * (Quat1.W * Quat1.Y - Quat1.Z * Quat1.X);
		t[2] = fabsf(t[2]) >= 1 ? powf(-1.0, signbit(t[2])) /*t[2]/fabsf(t[2])*/ : t[2];
		t[3] = Quat1.W * Quat1.Z + Quat1.X * Quat1.Y;
		t[4] = 0.5 - (Quat1.Y * Quat1.Y + Quat1.Z * Quat1.Z);

		roll = atan2f(t[0], t[1]);
		pitch = asinf(t[2]);
		yaw = atan2f(t[3], t[4]);
	}
	FVector EulerOut = FVector(roll, pitch, yaw) * 180.0 / PI;
	//FVector EulerOut = Quat1.Euler();
	return EulerOut;
}

FQuat UBlueprintQuaternionLibrary::MakeFromEuler(FVector Vect1)
{
	FQuat QuatOut = FQuat::MakeFromEuler(Vect1);
	return QuatOut;
}

FVector UBlueprintQuaternionLibrary::EulerYZX(FQuat Quat1)
{
	float t[5], roll, pitch, yaw, test, test_signal;
	test = Quat1.W * Quat1.X + Quat1.Y * Quat1.Z;

	if (fabsf(test) == 0.985 /*sin(85 degrees)*/) // Singularity test
	{
		test_signal = powf(-1.0, signbit(test)); /*test/fabsf(test)*/
		pitch = test_signal * 2.0 * atan2f(Quat1.X, Quat1.Y);
		yaw = test_signal * PI / 2;
		roll = 0;
	}
	else
	{
		t[0] = Quat1.W * Quat1.Y + Quat1.X * Quat1.Z;
		t[1] = 0.5 - (Quat1.Y * Quat1.Y + Quat1.Z * Quat1.Z);
		t[2] = 2.0 * (Quat1.W * Quat1.Z - Quat1.X * Quat1.Y);
		t[2] = fabsf(t[2]) >= 1 ? powf(-1.0, signbit(t[2])) /*t[2]/fabsf(t[2])*/ : t[2];
		t[3] = Quat1.W * Quat1.X + Quat1.Y * Quat1.Z;
		t[4] = 0.5 - (Quat1.X * Quat1.X + Quat1.Z * Quat1.Z);

		roll = atan2f(t[0], t[1]);
		pitch = asinf(t[2]);
		yaw = atan2f(t[3], t[4]);
	}
	FVector EulerOut = FVector(roll, pitch, yaw) * 180.0 / PI;
	//FVector EulerOut = Quat1.Euler();
	return EulerOut;
}

FVector UBlueprintQuaternionLibrary::EulerZYX(FQuat Quat1)
{
	float t[5], roll, pitch, yaw, test, test_signal;
	test = Quat1.W * Quat1.X - Quat1.Y * Quat1.Z;

	if (fabsf(test) == 0.985 /*sin(85 degrees)*/) // Singularity test
	{
		test_signal = powf(-1.0, signbit(test)); /*test/fabsf(test)*/
		pitch = test_signal * 2.0 * atan2f(Quat1.X, Quat1.Y);
		yaw = test_signal * PI / 2;
		roll = 0;
	}
	else
	{
		t[0] = Quat1.W * Quat1.Z - Quat1.X * Quat1.Y;
		t[1] = 0.5 - (Quat1.Y * Quat1.Y + Quat1.Z * Quat1.Z);
		t[2] = 2.0 * (Quat1.W * Quat1.Y - Quat1.X * Quat1.Z);
		t[2] = fabsf(t[2]) >= 1 ? powf(-1.0, signbit(t[2])) /*t[2]/fabsf(t[2])*/ : t[2];
		t[3] = Quat1.W * Quat1.X - Quat1.Y * Quat1.Z;
		t[4] = 0.5 - (Quat1.X * Quat1.X + Quat1.Y * Quat1.Y);

		roll = atan2f(t[0], t[1]);
		pitch = asinf(t[2]);
		yaw = atan2f(t[3], t[4]);
	}
	FVector EulerOut = FVector(roll, pitch, yaw) * 180.0 / PI;
	//FVector EulerOut = Quat1.Euler();
	return EulerOut;
}

FVector UBlueprintQuaternionLibrary::EulerZXY(FQuat Quat1)
{
	float t[5], roll, pitch, yaw, test, test_signal;
	test = Quat1.W * Quat1.Y + Quat1.X * Quat1.Z;

	if (fabsf(test) == 0.985 /*sin(85 degrees)*/) // Singularity test
	{
		test_signal = powf(-1.0, signbit(test)); /*test/fabsf(test)*/
		pitch = test_signal * 2.0 * atan2f(Quat1.X, Quat1.Y);
		yaw = test_signal * PI / 2;
		roll = 0;
	}
	else
	{
		t[0] = Quat1.W * Quat1.Z + Quat1.X * Quat1.Y;
		t[1] = 0.5 - (Quat1.X * Quat1.X + Quat1.Z * Quat1.Z);
		t[2] = 2.0 * (Quat1.W * Quat1.X - Quat1.Y * Quat1.Z);
		t[2] = fabsf(t[2]) >= 1 ? powf(-1.0, signbit(t[2])) /*t[2]/fabsf(t[2])*/ : t[2];
		t[3] = Quat1.W * Quat1.Y + Quat1.X * Quat1.Z;
		t[4] = 0.5 - (Quat1.X * Quat1.X + Quat1.Y * Quat1.Y);

		roll = atan2f(t[0], t[1]);
		pitch = asinf(t[2]);
		yaw = atan2f(t[3], t[4]);
	}
	FVector EulerOut = FVector(roll, pitch, yaw) * 180.0 / PI;
	//FVector EulerOut = Quat1.Euler();
	return EulerOut;
}

FQuat UBlueprintQuaternionLibrary::MakeFromEulerZXY(FVector Vect1)
{
	FQuat QuatOut;

	QuatOut.W = cosf(Vect1.X * PI / 360)*cosf(Vect1.Y * PI / 360)*cosf(Vect1.Z * PI / 360) + sinf(Vect1.X * PI / 360)*sinf(Vect1.Y * PI / 360)*sinf(Vect1.Z * PI / 360);
	QuatOut.X = cosf(Vect1.X * PI / 360)*sinf(Vect1.Y * PI / 360)*cosf(Vect1.Z * PI / 360) + sinf(Vect1.X * PI / 360)*cosf(Vect1.Y * PI / 360)*sinf(Vect1.Z * PI / 360);
	QuatOut.Y = cosf(Vect1.X * PI / 360)*cosf(Vect1.Y * PI / 360)*sinf(Vect1.Z * PI / 360) - sinf(Vect1.X * PI / 360)*sinf(Vect1.Y * PI / 360)*cosf(Vect1.Z * PI / 360);
	QuatOut.Z = -cosf(Vect1.X * PI / 360)*sinf(Vect1.Y * PI / 360)*sinf(Vect1.Z * PI / 360) + sinf(Vect1.X * PI / 360)*cosf(Vect1.Y * PI / 360)*cosf(Vect1.Z * PI / 360);

	return QuatOut;
}