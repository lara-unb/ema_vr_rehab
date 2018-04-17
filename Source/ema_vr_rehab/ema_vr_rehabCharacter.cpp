// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ema_vr_rehabCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// Aema_vr_rehabCharacter

Aema_vr_rehabCharacter::Aema_vr_rehabCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	//FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void Aema_vr_rehabCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &Aema_vr_rehabCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &Aema_vr_rehabCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &Aema_vr_rehabCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &Aema_vr_rehabCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &Aema_vr_rehabCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &Aema_vr_rehabCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &Aema_vr_rehabCharacter::OnResetVR);
}


void Aema_vr_rehabCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void Aema_vr_rehabCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void Aema_vr_rehabCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void Aema_vr_rehabCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void Aema_vr_rehabCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void Aema_vr_rehabCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void Aema_vr_rehabCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void Aema_vr_rehabCharacter::SubscribeOnTopic(FString ATopic)
{
	UE_LOG(LogTemp, Warning, TEXT("Subscribing on topic %s"), *ATopic);
	// Initialize a topic
	UTopic *ExampleTopic = NewObject<UTopic>(UTopic::StaticClass());
	UROSIntegrationGameInstance* rosinst = Cast<UROSIntegrationGameInstance>(GetGameInstance());
	ExampleTopic->Init(rosinst->_Ric, ATopic, TEXT("std_msgs/String"));

	// Create a std::function callback object
	std::function<void(TSharedPtr<FROSBaseMsg>)> SubscribeCallback = [&](TSharedPtr<FROSBaseMsg> msg) -> void
	{
		auto Concrete = StaticCastSharedPtr<ROSMessages::std_msgs::String>(msg);
		if (Concrete.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Incoming string was: %s"), (*(Concrete->_Data)));


			TArray<FString> Tokens;
			FString MyString(*(Concrete->_Data));
			const TCHAR* Delims[] = { TEXT(" ") };
			MyString.ParseIntoArray(Tokens, *Delims, true);
			if (Tokens.Num() >= 5)
			{
				if (OrientationMap.Find(Tokens[0]) == NULL)
				{
					OrientationMap.Emplace(Tokens[0], FBoneOrientationStruct(FQuat(FCString::Atof(*(Tokens[1])),
						FCString::Atof(*(Tokens[2])),
						FCString::Atof(*(Tokens[3])),
						FCString::Atof(*(Tokens[4])))));
				}
				else
				{
					OrientationMap[Tokens[0]] = FBoneOrientationStruct(FQuat(FCString::Atof(*(Tokens[1])),
						FCString::Atof(*(Tokens[2])),
						FCString::Atof(*(Tokens[3])),
						FCString::Atof(*(Tokens[4]))));
				}

				UE_LOG(LogTemp, Warning, TEXT("Object orientation %s: %f %f %f"), (*Tokens[0]), (OrientationMap[Tokens[0]].BoneOrientation.X), (OrientationMap[Tokens[0]].BoneOrientation.Y), (OrientationMap[Tokens[0]].BoneOrientation.Z));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not get message content"));
		}
		return;
	};

	// Subscribe to the topic
	ExampleTopic->Subscribe(SubscribeCallback);
}

void Aema_vr_rehabCharacter::PublishOnTopic(FString ATopic)
{
	UE_LOG(LogTemp, Warning, TEXT("Publishing on topic..."));
	// Initialize a topic
	UTopic *ExampleTopic = NewObject<UTopic>(UTopic::StaticClass());
	UROSIntegrationGameInstance* rosinst = Cast<UROSIntegrationGameInstance>(GetGameInstance());
	ExampleTopic->Init(rosinst->_Ric, ATopic, TEXT("std_msgs/String"));

	// (Optional) Advertise the topic
	// Currently unimplemented in the plugin
	//ExamplePublishTopic->Advertise();

	// Publish a string to the topic
	TSharedPtr<ROSMessages::std_msgs::String> StringMessage(new ROSMessages::std_msgs::String("This is an example"));
	ExampleTopic->Publish(StringMessage);
	UE_LOG(LogTemp, Warning, TEXT("Published on topic"));
}