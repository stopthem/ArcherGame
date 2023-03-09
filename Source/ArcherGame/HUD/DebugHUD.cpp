// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugHUD.h"

ADebugHUD::ADebugHUD()
{
	static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("/Engine/EngineFonts/Roboto"));

	MainFont = Font.Object;
}
