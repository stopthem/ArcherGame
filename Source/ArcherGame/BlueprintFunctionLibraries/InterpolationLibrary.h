#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InterpolationLibrary.generated.h"

/**
 * Interpolation library
 * @see Unreal Engine built-in interpolations: https://api.unrealengine.com/INT/BlueprintAPI/Math/Interpolation/index.html
 */
UCLASS()
class UInterpolationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	* Critically Damped Spring Interpolation (ie: Similar to Unity's SmoothDamp, but will less operations)
	*/
	UFUNCTION(BlueprintPure, Category = "Math|Interpolation", meta = (Keywords = "interp vinterp vectorspringinterp lerp smoothdamp"))
	static FVector VectorSpringInterpCD(FVector Current, FVector Target, UPARAM(ref) FVector& Velocity, float DeltaTime, float InterpSpeed = 10.f, float MaxVelocity = 0.f);

	/**
	* Critically Damped Spring Interpolation (ie: Similar to Unity's SmoothDamp, but will less operations)
	*/
	UFUNCTION(BlueprintPure, Category = "Math|Interpolation", meta = (Keywords = "interp finterp floatspringinterp lerp smoothdamp"))
	static float FloatSpringInterpCD(float Current, float Target, UPARAM(ref) float& Velocity, float DeltaTime, float InterpSpeed = 10.f, float MaxVelocity = 0.f);

	/**
	* Critically Damped Spring Interpolation
	*/
	UFUNCTION(BlueprintCallable, Category = "Math|Interpolation", meta = (Keywords = "rinterp smoothdamp"))
	static FRotator RotatorSpringInterpCD(FRotator Current, FRotator Target, UPARAM(ref) FVector4& Velocity, float DeltaTime, float InterpSpeed = 10.f, float MaxVelocity = 0.f);

	/**
	* Critically Damped Spring Interpolation
	*/
	UFUNCTION(BlueprintPure, Category = "Math|Interpolation", meta = (Keywords = "fquat smoothdamp"))
	static FQuat QuatSpringInterpCD(FQuat Current, FQuat Target, UPARAM(ref) FVector4& Velocity, float DeltaTime, float InterpSpeed = 10.f, float MaxVelocity = 0.f);


	// Utility methods
private:
	FORCEINLINE static FVector4 QuatToVector4(const FQuat& Quat)
	{
		return FVector4(Quat.X, Quat.Y, Quat.Z, Quat.W);
	}

	FORCEINLINE static FVector4 ClampVector4(FVector4 Target, float MaxSize)
	{
		if (MaxSize < KINDA_SMALL_NUMBER)
		{
			return FVector4(0.f, 0.f, 0.f, 0.f);
		}

		const float VSq = Target.SizeSquared();
		if (VSq > FMath::Square(MaxSize))
		{
			const float Scale = MaxSize * FMath::InvSqrt(VSq);
			return FVector4(Target.X * Scale, Target.Y * Scale, Target.Z * Scale, Target.W * Scale);
		}
		else
		{
			return Target;
		}
	}
};
