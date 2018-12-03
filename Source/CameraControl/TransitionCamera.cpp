// Fill out your copyright notice in the Description page of Project Settings.

#include "TransitionCamera.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATransitionCamera::ATransitionCamera()
	: CameraOne(NULL)
	, CameraTwo(NULL)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATransitionCamera::BeginPlay()
{
	Super::BeginPlay();

	for (auto camera: CameraList)
	{
		if (CameraOne == NULL)
		{
			CameraOne = camera;
		}
		else if (CameraTwo == NULL)
		{
			CameraTwo = camera;
			break;
		}
	}
}

// Called every frame
void ATransitionCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float TimeBetweenCameraChanges = 2.0f;
	const float SmoothBlendTime = 0.75f;
	TimeToNextCameraChange -= DeltaTime;
	if (TimeToNextCameraChange <= 0.0f)
	{
		TimeToNextCameraChange += TimeBetweenCameraChanges;

		// Find the actor that handles control for the local player.
		APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (OurPlayerController)
		{
			if ((OurPlayerController->GetViewTarget() != CameraOne) && (CameraOne != nullptr))
			{
				// Cut instantly to camera one.
				OurPlayerController->SetViewTarget(CameraOne);
			}
			else if ((OurPlayerController->GetViewTarget() != CameraTwo) && (CameraTwo != nullptr))
			{
				// Blend smoothly to camera two.
				OurPlayerController->SetViewTargetWithBlend(CameraTwo, SmoothBlendTime);
			}
		}
	}
}

