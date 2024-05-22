// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
/**
 * 
 */

class ZHUN_TEST_API ZhunUtils
{

public:
	ZhunUtils();
	~ZhunUtils() = default;

	static bool IsLocationReachable(const FVector& StartLocation, const FVector& TargetLocation, UWorld* World);

	static FVector GetRandomReachablePointInRadius(const FVector& Origin, float Radius, UWorld* World);

	static bool IsAngleLessThan90Degrees(const FVector& VectorA, const FVector& VectorB);
};
