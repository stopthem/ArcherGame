// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CanvasItem.h"
#include "Engine/Canvas.h"
#include "GameFramework/HUD.h"
#include "DebugHUD.generated.h"

/**
 * ADebugHUD
 *
 * Debugging HUD for Archer Game, a base class for deriving implementations from.
 */
UCLASS()
class ADebugHUD : public AHUD
{
	GENERATED_BODY()

protected:
	/**
	 * @brief Construct the debugging HUD, mainly establishing a font to use for display.
	 */
	ADebugHUD();

	/**
	 * @brief Add a FText to the HUD for rendering.
	 * @param title 
	 * @param value 
	 */
	void AddText(const TCHAR* title, const FText& value)
	{
		RenderStatistic(title, value);
	}

	void AddString(const TCHAR* title, const FString& value)
	{
		RenderStatistic(title, FText::AsCultureInvariant(value));
	}

	void AddFloat(const TCHAR* title, float value)
	{
		RenderStatistic(title, FText::AsNumber(value));
	}

	void AddInt(const TCHAR* title, int value)
	{
		RenderStatistic(title, FText::AsNumber(value));
	}

	void AddBool(const TCHAR* title, bool value)
	{
		RenderStatistic(title, BoolToText(value));
	}

	// Draw the HUD
	virtual void DrawHUD() override
	{
		X = 50.0f;
		Y = 200.0f;
	}

	// The horizontal offset to render the statistic values at.
	float HorizontalOffset = 150.0f;

private:
	// Convert a TCHAR pointer to FText.
	FText CStringToText(const TCHAR* text) const
	{
		return FText::FromString(text);
	}

	// Convert a bool to FText.
	FText BoolToText(bool value) const
	{
		return CStringToText(value ? TEXT("true") : TEXT("false"));
	}

	// Render a statistic onto the HUD canvas.
	void RenderStatistic(const TCHAR* title, const FText& value, const FLinearColor& valueColor = FLinearColor::White)
	{
		const auto actualCanvas = Canvas.Get();
		FCanvasTextItem item0(FVector2D(X, Y), CStringToText(title), MainFont, FLinearColor::White);

		item0.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));

		actualCanvas->DrawItem(item0);

		FCanvasTextItem item1(FVector2D(X + HorizontalOffset, Y), value, MainFont, valueColor);

		item1.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));

		actualCanvas->DrawItem(item1);

		Y += LineHeight;
	}

	// Font used to render the debug information.
	UPROPERTY(Transient)
	TObjectPtr<UFont> MainFont = nullptr;

	// The current X coordinate.
	float X = 50.0f;

	// The current Y coordinate.
	float Y = 50.0f;

	// The line height to separate each hud entry
	float LineHeight = 16.0f;
};
