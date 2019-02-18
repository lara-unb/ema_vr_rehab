// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Networking
#include "Networking.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UDPReceiver.generated.h"

UCLASS()
class EMA_VR_REHAB_API AUDPReceiver : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	
	// Sets default values for this actor's properties
	//AUDPReceiver();
	//AUDPReceiver(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BPEvent_DataReceived(const FString& ReceivedData);

	FSocket* ListenSocket;

	FUdpSocketReceiver* UDPReceiver = nullptr;
	void Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);

	UFUNCTION(BlueprintCallable)
	bool StartUDPReceiver(
		const FString& YourChosenSocketName,
		const FString& TheIP,
		const int32 ThePort
	);

	//ScreenMsg
	FORCEINLINE void ScreenMsg(const FString& Msg)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *Msg);
	}
	FORCEINLINE void ScreenMsg(const FString& Msg, const float Value)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s %f"), *Msg, Value));
	}
	FORCEINLINE void ScreenMsg(const FString& Msg, const FString& Msg2)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s %s"), *Msg, *Msg2));
	}

	/** Called whenever this actor is being removed from a level */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
};
