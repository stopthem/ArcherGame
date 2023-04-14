// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArcherGame.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, ArcherGame, "ArcherGame");

static TAutoConsoleVariable CVarBehaviourTreeActive(
	TEXT("ai.AllBehaviorTrees.Activated"),
	0,
	TEXT("Sets all behavior tree logics in the scene whatever they are active or not")
	TEXT("0 : not active")
	TEXT("1 : active"),
	ECVF_Scalability | ECVF_RenderThreadSafe);
