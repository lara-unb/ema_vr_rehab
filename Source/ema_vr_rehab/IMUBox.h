// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ROSIntegration/Public/std_msgs/String.h"
#include "ROSIntegration/Classes/RI/Topic.h"
#include "ROSIntegration/Classes/ROSIntegrationGameInstance.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IMUBox.generated.h"

UCLASS()
class EMA_VR_REHAB_API AIMUBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIMUBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SubscribeOnTopic(FString ATopic);

	UFUNCTION(BlueprintCallable)
	void PublishOnTopic(FString ATopic);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector OrientationMap;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FQuat OrientationQuat;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* TCPReference;
	
};
