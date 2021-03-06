// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ROSIntegration/Public/std_msgs/String.h"
#include "ROSIntegration/Classes/RI/Topic.h"
#include "ROSIntegration/Classes/ROSIntegrationGameInstance.h"
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "ema_vr_rehabCharacter.generated.h"

USTRUCT(BlueprintType)
struct FBoneOrientationStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FQuat BoneQuaternion;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector BoneOrientation;

	FBoneOrientationStruct()
	{
		BoneOrientation = FVector(0, 0, 0);
		BoneQuaternion = FQuat::MakeFromEuler(FVector(0, 0, 0));
	}

	FBoneOrientationStruct(FQuat ABoneQuaternion)
	{
		BoneQuaternion = ABoneQuaternion;
		BoneOrientation = ABoneQuaternion.Euler();
	}

	FBoneOrientationStruct(FVector ABoneOrientation)
	{
		BoneOrientation = ABoneOrientation;
		BoneQuaternion = FQuat::MakeFromEuler(ABoneOrientation);
	}
};

UCLASS(config=Game)
class Aema_vr_rehabCharacter : public ACharacter
{
	GENERATED_BODY()
	/** Camera boom positioning the camera behind the character */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class USpringArmComponent* CameraBoom;

	/** Follow camera */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UCameraComponent* FollowCamera;
public:
	Aema_vr_rehabCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UFUNCTION(BlueprintCallable)
	void SubscribeOnTopic(FString ATopic);

	UFUNCTION(BlueprintCallable)
	void PublishOnTopic(FString ATopic, FString Content);

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FString, FBoneOrientationStruct> OrientationMap;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<FString, FString> TopicString;

	/*float timeSinceReset;

	FString fileWriteString;*/

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	//FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

