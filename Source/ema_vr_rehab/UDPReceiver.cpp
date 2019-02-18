// Fill out your copyright notice in the Description page of Project Settings.

#include "UDPReceiver.h"


// Sets default values
AUDPReceiver::AUDPReceiver(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	ListenSocket = NULL;
}

// Called when the game starts or when spawned
/*void AUDPReceiver::BeginPlay()
{
	Super::BeginPlay();
	
}*/

// Called every frame
/*void AUDPReceiver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

void AUDPReceiver::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	delete UDPReceiver;
	UDPReceiver = nullptr;

	// Clear all sockets!
	// Makes sure repeat plays in Editor don't hold on to old sockets!
	if (ListenSocket)
	{
		ListenSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenSocket);
	}
}

//Rama's Start TCP Receiver
bool AUDPReceiver::StartUDPReceiver(
	const FString& YourChosenSocketName,
	const FString& TheIP,
	const int32 ThePort
) {

	ScreenMsg("RECEIVER INIT");

	FIPv4Address Addr;
	FIPv4Address::Parse(TheIP, Addr);

	//Create Socket
	FIPv4Endpoint Endpoint(Addr, ThePort);

	//BUFFER SIZE
	int32 BufferSize = 2 * 1024 * 1024;

	ListenSocket = FUdpSocketBuilder(*YourChosenSocketName)
		.AsNonBlocking()
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.WithReceiveBufferSize(BufferSize);
	;

	FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(100);
	UDPReceiver = new FUdpSocketReceiver(ListenSocket, ThreadWaitTime, TEXT("UDP RECEIVER"));
	UDPReceiver->OnDataReceived().BindUObject(this, &AUDPReceiver::Recv);

	return true;
}

void AUDPReceiver::Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt)
{
	ScreenMsg("Received bytes", ArrayReaderPtr->Num());

	FString Data;
	*ArrayReaderPtr << Data;		//Now de-serializing! See AnyCustomData.h

									//BP Event
	BPEvent_DataReceived(Data);
}