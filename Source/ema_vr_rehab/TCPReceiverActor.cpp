// Fill out your copyright notice in the Description page of Project Settings.

#include "TCPReceiverActor.h"


// Sets default values
ATCPReceiverActor::ATCPReceiverActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATCPReceiverActor::BeginPlay()
{
	Super::BeginPlay();
	
}

/*void ATCPReceiverActor::BeginDestroy() //clean up to prevent crashing the Editor
{
	// Call the base class
	Super::BeginDestroy();
	
	UE_LOG(LogTemp, Warning, TEXT("Closing Sockets"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Closing Sockets")));
	
	GetWorldTimerManager().ClearTimer(TCPSocketListenerTimerHandle);
	GetWorldTimerManager().ClearTimer(TCPConnectionListenerTimerHandle);
	
	if (ListenerSocket != nullptr) {
		ListenerSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenerSocket);
	}

	if (ConnectionSocket != nullptr) {
		ConnectionSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectionSocket);
	}
}*/

void ATCPReceiverActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UE_LOG(LogTemp, Warning, TEXT("Closing Sockets"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Closing Sockets")));
	
	GetWorldTimerManager().ClearTimer(TCPSocketListenerTimerHandle);
	GetWorldTimerManager().ClearTimer(TCPConnectionListenerTimerHandle);

	if (ConnectionSocket != NULL) {
		ConnectionSocket->Close();
		//ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectionSocket);
	}
	if (ListenerSocket != NULL) {
		ListenerSocket->Close();
		//ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenerSocket);
	}

}

// Called every frame
void ATCPReceiverActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATCPReceiverActor::Launch(FString IP, int32 Port)
{
	if (ConnectionSocket != NULL) {
		ConnectionSocket->Close();
		//ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectionSocket);
	}
	if (ListenerSocket != NULL) {
		ListenerSocket->Close();
		//ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenerSocket);
	}

	if (!StartTCPReceiver("RamaSocketListener", IP, Port))
	{
		//UE_LOG  "TCP Socket Listener Created!"
		return;
	}

	//UE_LOG  "TCP Socket Listener Created! Yay!"
}

//Rama's Start TCP Receiver
bool ATCPReceiverActor::StartTCPReceiver(
	const FString& YourChosenSocketName,
	const FString& TheIP,
	const int32 ThePort
) {
	//Rama's CreateTCPConnectionListener
	ListenerSocket = CreateTCPConnectionListener(YourChosenSocketName, TheIP, ThePort);

	//Not created?
	if (!ListenerSocket)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("StartTCPReceiver>> Listen socket could not be created! ~> %s %d"), *TheIP, ThePort));
		UE_LOG(LogTemp, Warning, TEXT("StartTCPReceiver>> Listen socket could not be created! ~> %s %d"), *TheIP, ThePort);
		return false;
	}

	
	//Start the Listener! //thread this eventually
	/*GetWorldTimerManager().ClearTimer(TCPConnectionListenerTimerHandle);
	GetWorldTimerManager().SetTimer(TCPConnectionListenerTimerHandle, this,
		&ATCPReceiverActor::TCPConnectionListener, 0.01, true);*/
	Listener = new FTcpListener(*ListenerSocket);
	Listener->OnConnectionAccepted().BindUObject(this, &ATCPReceiverActor::TCPConnectionListener);

	return true;
}
//Format IP String as Number Parts
bool ATCPReceiverActor::FormatIP4ToNumber(const FString& TheIP, uint8(&Out)[4])
{
	//IP Formatting
	TheIP.Replace(TEXT(" "), TEXT(""));

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//						   IP 4 Parts

	//String Parts
	TArray<FString> Parts;
	TheIP.ParseIntoArray(Parts, TEXT("."), true);
	if (Parts.Num() != 4)
		return false;

	//String to Number Parts
	for (int32 i = 0; i < 4; ++i)
	{
		Out[i] = FCString::Atoi(*Parts[i]);
	}

	return true;
}
//Rama's Create TCP Connection Listener
FSocket* ATCPReceiverActor::CreateTCPConnectionListener(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort, const int32 ReceiveBufferSize)
{
	uint8 IP4Nums[4];
	if (!FormatIP4ToNumber(TheIP, IP4Nums))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid IP! Expecting 4 parts separated by ."));
		return false;
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//Create Socket
	FIPv4Endpoint Endpoint(FIPv4Address(IP4Nums[0], IP4Nums[1], IP4Nums[2], IP4Nums[3]), ThePort);
	FSocket* ListenSocket = FTcpSocketBuilder(*YourChosenSocketName)
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.Listening(8);

	//Set Buffer Size
	int32 NewSize = 0;
	ListenSocket->SetReceiveBufferSize(ReceiveBufferSize, NewSize);

	//Done!
	return ListenSocket;
}
//Rama's TCP Connection Listener
bool ATCPReceiverActor::TCPConnectionListener(FSocket* ClientSocket, const FIPv4Endpoint& ClientEndpoint)
{
	//~~~~~~~~~~~~~
	if (!ListenerSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("Listener socket invalid!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Listener socket invalid!")));
		return false;
	}
	UE_LOG(LogTemp, Warning, TEXT("Listener socket OK!"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Listener socket OK!")));
	//~~~~~~~~~~~~~

	//Remote address
	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool Pending;

	// handle incoming connections
	if (ListenerSocket->HasPendingConnection(Pending) && Pending)
	{
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//Already have a Connection? destroy previous
		if (ConnectionSocket)
		{
			ConnectionSocket->Close();
			//ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectionSocket);
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		//New Connection receive!
		ConnectionSocket = ListenerSocket->Accept(*RemoteAddress, TEXT("RamaTCP Received Socket Connection"));

		if (ConnectionSocket != NULL)
		{
			//Global cache of current Remote Address
			RemoteAddressForConnection = FIPv4Endpoint(RemoteAddress);

			UE_LOG(LogTemp, Warning, TEXT("Accepted Connection!"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Accepted Connection!")));

			//can thread this too
			GetWorldTimerManager().ClearTimer(TCPSocketListenerTimerHandle);
			GetWorldTimerManager().SetTimer(TCPSocketListenerTimerHandle, this,
				&ATCPReceiverActor::TCPSocketListener, 0.01, true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Connection socket invalid!"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection socket invalid!")));

			return false;
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No incoming connections"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No incoming connections")));
		return false;
	}
	
	return true;
}

//Rama's String From Binary Array
FString ATCPReceiverActor::StringFromBinaryArray(TArray<uint8>& BinaryArray)
{
	BinaryArray.Add(0); // Add 0 termination. Even if the string is already 0-terminated, it doesn't change the results.
						// Create a string from a byte array. The string is expected to be 0 terminated (i.e. a byte set to 0).
						// Use UTF8_TO_TCHAR if needed.
						// If you happen to know the data is UTF-16 (USC2) formatted, you do not need any conversion to begin with.
						// Otherwise you might have to write your own conversion algorithm to convert between multilingual UTF-16 planes.
	return FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(BinaryArray.GetData())));
}

//Rama's TCP Socket Listener
void ATCPReceiverActor::TCPSocketListener()
{
	//~~~~~~~~~~~~~
	if (!ConnectionSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("Connection socket invalid!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection socket invalid!")));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Connection socket OK!"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection socket OK!")));
	//~~~~~~~~~~~~~


	//Binary Array!
	TArray<uint8> ReceivedData;

	uint32 Size;
	while (ConnectionSocket->HasPendingData(Size))
	{
		ReceivedData.Init(0,FMath::Min(Size, 65507u));

		int32 Read = 0;
		ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);

		UE_LOG(LogTemp, Warning, TEXT("Data Read! %d"), ReceivedData.Num());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data Read! %d"), ReceivedData.Num()));
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (ReceivedData.Num() <= 0)
	{
		//No Data Received
		UE_LOG(LogTemp, Warning, TEXT("No Data Received"), ReceivedData.Num());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No Data Received")));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Data Bytes Read ~> %d"), ReceivedData.Num());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data Bytes Read ~> %d"), ReceivedData.Num()));


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//						Rama's String From Binary Array
	const FString ReceivedUE4String = StringFromBinaryArray(ReceivedData);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	UE_LOG(LogTemp, Warning, TEXT("As String Data ~> %s"), *ReceivedUE4String);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("As String Data ~> %s"), *ReceivedUE4String));
}

