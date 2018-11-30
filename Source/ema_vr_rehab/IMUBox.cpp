// Fill out your copyright notice in the Description page of Project Settings.

#include "IMUBox.h"


// Sets default values
AIMUBox::AIMUBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AIMUBox::SubscribeOnTopic(FString ATopic)
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
			//UE_LOG(LogTemp, Warning, TEXT("Incoming string was: %s"), (*(Concrete->_Data)));


			TArray<FString> Tokens;
			FString MyString(*(Concrete->_Data));
			const TCHAR* Delims[] = { TEXT(" ") };
			MyString.ParseIntoArray(Tokens, *Delims, true);
			if (Tokens.Num() >= 5)
			{
				OrientationMap = FQuat(FQuat(FCString::Atof(*(Tokens[1])),
					FCString::Atof(*(Tokens[2])),
					FCString::Atof(*(Tokens[3])),
					FCString::Atof(*(Tokens[4])))).Euler();
				//UE_LOG(LogTemp, Warning, TEXT("Object orientation: %f %f %f"), (OrientationMap.X), (OrientationMap.Y), (OrientationMap.Z));
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

void AIMUBox::PublishOnTopic(FString ATopic)
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

// Called when the game starts or when spawned
void AIMUBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIMUBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

