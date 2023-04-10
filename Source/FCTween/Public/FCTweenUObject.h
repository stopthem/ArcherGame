// MIT License - Copyright (c) 2022 Jared Cook
#pragma once
#include "FCTweenInstance.h"

#include "FCTweenUObject.generated.h"

USTRUCT(BlueprintType)
struct FTweenParams
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FCTween|Params")
	float Duration = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FCTween|Params")
	EFCEase Ease = EFCEase::InOutSine;
};

/*
 * FTweenParamsOneValue
 *
 * FTWeenParams but has a one extra variable
 */
USTRUCT(BlueprintType)
struct FTweenParamsOneValue : public FTweenParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FCTween|Params")
	float OneValue = 5.0f;
};

/**
 * @brief Use this to wrap an FCTweenInstance inside a UObject, so that it's destroyed when its outer object is destroyed
 */
UCLASS()
class UFCTweenUObject : public UObject
{
	GENERATED_BODY()

public:
	FCTweenInstance* Tween;

	UFCTweenUObject();
	virtual void BeginDestroy() override;

	void SetTweenInstance(FCTweenInstance* InTween);
	/**
	 * @brief Stop the tween immediately and mark this object for destruction
	 */
	void Destroy();
};
