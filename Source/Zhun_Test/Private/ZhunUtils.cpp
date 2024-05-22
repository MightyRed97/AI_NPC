// Fill out your copyright notice in the Description page of Project Settings.


#include "ZhunUtils.h"

#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

ZhunUtils::ZhunUtils()
{
}

bool ZhunUtils::IsLocationReachable(const FVector& StartLocation, const FVector& TargetLocation, UWorld* World)
{
    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
    if (!NavSystem) return false;

    FNavLocation StartNavLocation;
    FNavLocation EndNavLocation;

    // Get the current location as the start location
    if (NavSystem->ProjectPointToNavigation(StartLocation, StartNavLocation, FVector::ZeroVector, nullptr))
    {
        // Project the target location to the navigation mesh to get the end location
        if (NavSystem->ProjectPointToNavigation(TargetLocation, EndNavLocation, FVector::ZeroVector, nullptr))
        {
            FPathFindingQuery Query;
            Query.StartLocation = StartNavLocation.Location;
            Query.EndLocation = EndNavLocation.Location;

            return NavSystem->TestPathSync(Query, EPathFindingMode::Regular);
        }

        return false;
    }

    return false;
}

FVector ZhunUtils::GetRandomReachablePointInRadius(const FVector& Origin, float Radius, UWorld* World)
{
    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
    if (!NavSystem)
    {
        // Navigation system not available
        return FVector::ZeroVector;
    }

    FNavLocation RandomNavLocation;
    const int32 MaxAttempts = 100;
    FVector RandomPoint;

    for (int32 i = 0; i < MaxAttempts; ++i)
    {
        // Generate a random point within the specified radius
        RandomPoint = Origin + FMath::RandPointInBox(FBox(FVector(-Radius, -Radius, 0), FVector(Radius, Radius, 0)));

        // Project the random point to the navigation mesh
        if (NavSystem->ProjectPointToNavigation(RandomPoint, RandomNavLocation, FVector::ZeroVector, nullptr))
        {
            // Random point is reachable
            return RandomNavLocation.Location;
        }
    }

    // No reachable point found after maximum attempts
    return FVector::ZeroVector;
}

bool ZhunUtils::IsAngleLessThan90Degrees(const FVector& VectorA, const FVector& VectorB)
{
    // Calculate the dot product of the vectors
    float DotProduct = FVector::DotProduct(VectorA.GetSafeNormal(), VectorB.GetSafeNormal());

    // Calculate the angle in radians using the dot product
    float AngleRad = FMath::Acos(DotProduct);

    // Convert the angle from radians to degrees
    float AngleDeg = FMath::RadiansToDegrees(AngleRad);

    // Check if the angle is less than 90 degrees
    return AngleDeg < 90.0f;
}
