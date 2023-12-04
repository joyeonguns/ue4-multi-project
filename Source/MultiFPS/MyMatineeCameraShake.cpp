// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMatineeCameraShake.h"


UMyMatineeCameraShake::UMyMatineeCameraShake()
{
    OscillationDuration = 0.1f;
    OscillationBlendInTime = 0.02f;
    OscillationBlendOutTime = 0.02f;

    RotOscillation.Pitch.Amplitude = 1.0f; // FMath::RandRange(0.0f, 1.0f);
    RotOscillation.Pitch.Frequency = 1.0f; //  FMath::RandRange(-1.0f, 1.0f);

    RotOscillation.Yaw.Amplitude = 1.0f; // FMath::RandRange(0.0f, 1.0f);
    RotOscillation.Yaw.Frequency = 1.0f; //  FMath::RandRange(-1.0f, 1.0f);
}

void UMyMatineeCameraShake::SetShakeVal(float ShakeVal)
{
    RotOscillation.Pitch.Amplitude = ShakeVal;

    RotOscillation.Yaw.Amplitude = ShakeVal;
}
