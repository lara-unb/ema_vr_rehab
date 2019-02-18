// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Networking.h"
#include "TCPReceiverActor.generated.h"

UCLASS()
class EMA_VR_REHAB_API ATCPReceiverActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATCPReceiverActor();
	FSocket* ListenerSocket;
	FSocket* ConnectionSocket;
	FIPv4Endpoint RemoteAddressForConnection;
	FTcpListener* Listener;

	FTimerHandle TCPSocketListenerTimerHandle;
	FTimerHandle TCPConnectionListenerTimerHandle;

	UFUNCTION(BlueprintCallable, Category = "TCPConnection")
	void Launch(FString IP, int32 Port);

	bool StartTCPReceiver(
		const FString& YourChosenSocketName,
		const FString& TheIP,
		const int32 ThePort
	);

	FSocket* CreateTCPConnectionListener(
		const FString& YourChosenSocketName,
		const FString& TheIP,
		const int32 ThePort,
		const int32 ReceiveBufferSize = 2 * 1024 * 1024
	);

	//Timer functions, could be threads
	bool TCPConnectionListener(FSocket* ClientSocket, const FIPv4Endpoint& ClientEndpoint); 	//can thread this eventually
	void TCPSocketListener();		//can thread this eventually
	
	//Format String IP4 to number array
	bool FormatIP4ToNumber(const FString& TheIP, uint8(&Out)[4]);

	//Rama's StringFromBinaryArray
	FString StringFromBinaryArray(TArray<uint8>& BinaryArray);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual void BeginDestroy() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
		
};
