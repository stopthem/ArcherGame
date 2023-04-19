// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArcherGame.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, ArcherGame, "ArcherGame");

static TAutoConsoleVariable CVarBehaviourTreeActive(
	TEXT("ai.AllBehaviorTrees.Activated"),
	1,
	TEXT("Sets all behavior tree logics in the scene whatever they are active or not")
	TEXT("0 : not active")
	TEXT("1 : active"),
	ECVF_Scalability | ECVF_RenderThreadSafe);

static TAutoConsoleVariable CVarPlayerDamageImmunity(
	TEXT("player.DamageImmunity.Activated"),
	0,
	TEXT("Sets player damage immunity")
	TEXT("0 : not immune")
	TEXT("1 : immune"),
	ECVF_Scalability | ECVF_RenderThreadSafe);

static TAutoConsoleVariable CVarPlayerManaImmunity(
	TEXT("player.ManaImmunity.Activated"),
	0,
	TEXT("Sets player mana immunity")
	TEXT("0 : not immune")
	TEXT("1 : immune"),
	ECVF_Scalability | ECVF_RenderThreadSafe);

static TAutoConsoleVariable CVarShouldSpawnWave(
	TEXT("archergame.Wave.ShouldSpawn"),
	0,
	TEXT("Should game spawn wave?")
	TEXT("0 : spawn")
	TEXT("1 : dont spawn"),
	ECVF_Scalability | ECVF_RenderThreadSafe);
