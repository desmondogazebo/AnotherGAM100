/******************************************************************************
filename    BattleScene.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu

Created on 10 November 2017

Brief Description:
A template on creating a customized state manager

******************************************************************************/
#include "BattleScene.h"

#include <stdio.h>

// Inclusion of utility functions
#include "../Utilities/TextDataLoader.h"
#include "../Utilities/Utilities.h"

// Included for Rendering
#include "../Engine/BaseEngine.h"

// Inclusion of Enemies
#include "../Enemy/EnemyDataLoader.h"

///****************************************************************************
// Private Variables
///****************************************************************************
// Loaders
TextDataLoader BattleScene_Loader_Layout;
TextDataLoader BattleScene_Loader_ShieldIcon;
TextDataLoader BattleScene_Loader_WinIcon;
TextDataLoader BattleScene_Loader_LoseIcon;

// Player Stats
short PlayerCurrentHealth;

// Enemy Data
Enemy CurrentEnemy;
enum Enemies CurrentEnemyType;
int EnemyCurrentHealth;
char ScreenLineInfoBuffer[100];
int PlayerTurnChoiceSelector;

// A meter for handling just attacks, just guards & just escapes.
float JustMeterPercent;
float BattleScene_Timer;
int BarIncrementationDirection;
int MeterActive;
int BarLength = 30;
Vector2 AttackThreshold;

// Animation Values
short AttackAnimationRunning;
float AttackAnimationTime = 0.5f;
short AttackFailedPlayer;

// Enemy Attack Values
short EnemyIsAttacking;
float PerfectGuardTimeFrame = 0.3f;
short RenderShield;

// Player XP
short DidPlayerLevel;
short XPScalingFactor = 10;

///****************************************************************************
// Private Function Prototypes
///****************************************************************************
// State Manager Functions
// Linked Initiallize function that will be set to the struct's Initiallize
void BattleScene_LinkedInitiallize(BattleScene* Self);
// Linked Update function that will be set to the struct's Update
void BattleScene_LinkedUpdate(BattleScene* Self, Engine* BaseEngine, double Delta);
// Linked Render function that will be set to the struct's Render
void BattleScene_LinkedRender(BattleScene* Self, Engine* BaseEngine);
// Linked Exit function that will be set to the struct's Exit
void BattleScene_LinkedExit(BattleScene* Self);

// Internal State Manager Functions
// Linked initiallize function that will be set to the InternalStateManager.Initiallize
void BattleScene_LinkedInternalInitiallize(BattleScene* Self);
// Linked Update function that will be set to the InternalStateManager.Update
void BattleScene_LinkedInternalUpdate(BattleScene* Self, Engine* BaseEngine, double Delta);
// Linked Render function that will be set to the InternalStateManager.Render
void BattleScene_LinkedInternalRender(BattleScene* Self, Engine* BaseEngine);
// Linked Exit function that will be set to the InternalStateManager.Exit
void BattleScene_LinkedInternalExit(BattleScene* Self);

// Scene Based Functions
void RenderBattle(BattleScene* Self, Engine* BaseEngine);
void RenderAttackMeter(BattleScene* Self, Engine* BaseEngine);
void RenderAttackAnimation(BattleScene* Self, Engine* BaseEngine);
void RenderResults(BattleScene* Self, Engine* BaseEngine);

// Gameplay Logic Code
void BarLogic(float BarSpeed, double Delta);
Vector2 RandomizeOptimalBarThresholds();

///****************************************************************************
// Function Definitions
///****************************************************************************
void BattleScene_Setup(BattleScene* Self)
{
	// Set up the InternalStateManager
	Self->InternalStateManager.Initiallize = BattleScene_LinkedInternalInitiallize;
	Self->InternalStateManager.Update = BattleScene_LinkedInternalUpdate;
	Self->InternalStateManager.Render = BattleScene_LinkedInternalRender;
	Self->InternalStateManager.Exit = BattleScene_LinkedInternalExit;

	// Set the current state
	Self->InternalState = BS_Loading;

	// Set up the functions of this object
	Self->Initiallize = BattleScene_LinkedInitiallize;
	Self->Update = BattleScene_LinkedUpdate;
	Self->Render = BattleScene_LinkedRender;
	Self->Exit = BattleScene_LinkedExit;
}

// Linked Initiallize function that will be set to the struct's Initiallize
void BattleScene_LinkedInitiallize(BattleScene* Self)
{
	Self->InternalStateManager.Initiallize(Self);
}

// Linked Update function that will be set to the struct's Update
void BattleScene_LinkedUpdate(BattleScene* Self, Engine* BaseEngine, double Delta)
{
	Self->InternalStateManager.Update(Self, BaseEngine, Delta);
}

// Linked Render function that will be set to the struct's Render
void BattleScene_LinkedRender(BattleScene* Self, Engine* BaseEngine)
{
	Self->InternalStateManager.Render(Self, BaseEngine);
}

// Linked Exit function that will be set to the struct's Exit
void BattleScene_LinkedExit(BattleScene* Self)
{
	Self->InternalStateManager.Exit(Self);
}

// Linked Initiallize function that will be set to the InternalStateManager
void BattleScene_LinkedInternalInitiallize(BattleScene* Self)
{
	// Here I will initiallize the internal state manager
	// Setup the loader that I am about to use.
	TextDataLoader_Setup(&BattleScene_Loader_Layout);
	TextDataLoader_Setup(&BattleScene_Loader_ShieldIcon);
	TextDataLoader_Setup(&BattleScene_Loader_WinIcon);
	TextDataLoader_Setup(&BattleScene_Loader_LoseIcon);
	// Load the sprites that will be used in the battle scene
	BattleScene_Loader_Layout.LoadResource(&BattleScene_Loader_Layout, "Resources/Battle/Borders.txt");
	BattleScene_Loader_ShieldIcon.LoadResource(&BattleScene_Loader_ShieldIcon, "Resources/Battle/Guard.txt");
	BattleScene_Loader_WinIcon.LoadResource(&BattleScene_Loader_WinIcon, "Resources/Battle/Win.txt");
	BattleScene_Loader_LoseIcon.LoadResource(&BattleScene_Loader_LoseIcon, "Resources/Battle/Lose.txt");
	
	// Set up base variables
	RenderShield = EnemyIsAttacking = AttackFailedPlayer = AttackAnimationRunning = PlayerTurnChoiceSelector = 0;
	BattleScene_Timer = 0.f;
	BarIncrementationDirection = 1;
		
	// Set the current state
	Self->InternalState = BS_Loading;
}

// Linked Update function that will be set to the InternalStateManager
void BattleScene_LinkedInternalUpdate(BattleScene* Self, Engine* BaseEngine, double Delta)
{
	// Do some state logic for the internal state manager
	// Clearing the input buffer before handling checks
	isKeyPressed(VK_SPACE);
	isKeyPressed(VK_RETURN);
	float BarSpeedRatio = ((float)CurrentEnemy.spd) / (float)Get_PlayerSPD(&BaseEngine->playerData);
	// Clamp the speed ratio
	if (BarSpeedRatio > 2)
		BarSpeedRatio = 2;
	else if (BarSpeedRatio < 0.5f)
		BarSpeedRatio = 0.5f;
	switch (Self->InternalState)
	{
	case BS_Loading:
		// Load the enemy
		CurrentEnemy = *BaseEngine->InternalSceneSystem.InternalEncounterHandler.CurrentEnemy;
		CurrentEnemyType = BaseEngine->InternalSceneSystem.InternalEncounterHandler.CurrentEnemyType;
		EnemyCurrentHealth = CurrentEnemy.hp;
		Self->InternalState = BS_PlayerTurnChoice;

		// Load the player
		PlayerCurrentHealth = Get_PlayerHP(&BaseEngine->playerData);
		break;
	case BS_PlayerTurnChoice:
		// Handle choice selection
		if ((isKeyPressed('W') || isKeyPressed(VK_UP)) && PlayerTurnChoiceSelector)
			PlayerTurnChoiceSelector = 0;
		else if ((isKeyPressed('S') || isKeyPressed(VK_DOWN)) && !PlayerTurnChoiceSelector)
			PlayerTurnChoiceSelector = 1;
		// Lock in the player's selection
		else if (isKeyPressed(VK_RETURN))
		{
			// Check the selected choice, do something
			switch (PlayerTurnChoiceSelector)
			{
			case 0:
				// I will attack
				Self->InternalState = BS_PlayerTurnAttack;
				// Reset the timer
				JustMeterPercent = BattleScene_Timer = 0;
				MeterActive = 1;
				AttackThreshold = RandomizeOptimalBarThresholds();
				{
					int RandomDirection = IntRandomizeRange(0, 1);
					if (RandomDirection > 0)
						BarIncrementationDirection = 1;
					else BarIncrementationDirection = -1;
				}
				JustMeterPercent = (float)IntRandomizeRange(0,99);
				break;
			case 1:
				// I will Run
				Self->InternalState = BS_PlayerTurnRun;
				// Reset the timer
				JustMeterPercent = BattleScene_Timer = 0;
				MeterActive = 1;
				AttackThreshold = RandomizeOptimalBarThresholds();
				{
					int RandomDirection = IntRandomizeRange(0, 1);
					if (RandomDirection > 0)
						BarIncrementationDirection = 1;
					else BarIncrementationDirection = -1;
				}
				JustMeterPercent = (float)IntRandomizeRange(0, 99);
				break;
			}
		}
		break;
	case BS_PlayerTurnAttack:
		if (MeterActive)
		{
			BarLogic((float)CurrentEnemy.spd * 50 * BarSpeedRatio, Delta);
			AttackAnimationRunning = 0;
		}
		else if (!AttackAnimationRunning && !AttackFailedPlayer){
			// It's time to do damage!
			int CurrentIndex = (int)(JustMeterPercent * 0.01f * BarLength);
			if (CurrentIndex >= AttackThreshold.x && CurrentIndex <= AttackThreshold.y)
			{
				AttackAnimationRunning = 1;
				EnemyCurrentHealth -= Get_PlayerATK(&BaseEngine->playerData);
				if (EnemyCurrentHealth <= 0)
				{
					DidPlayerLevel = Gain_PlayerExp(&BaseEngine->playerData, CurrentEnemy.lvl * XPScalingFactor);
					EnemyCurrentHealth = 0;
					// Increase boss flag based on boss
					int FlagValueForCurrentBoss = 0;
					switch (CurrentEnemyType)
					{
						case Boss_DatBoiLv1:
							FlagValueForCurrentBoss = 1;
							break;
						case Boss_DatBoiLv2:
							FlagValueForCurrentBoss = 2;
							break;
						case Boss_DatBoiLv3:
							FlagValueForCurrentBoss = 3;
							break;
						case Boss_DatBoiLv4:
							FlagValueForCurrentBoss = 4;
							break;
					}
					if (FlagValueForCurrentBoss > 0 && BaseEngine->playerData.bossFlag < FlagValueForCurrentBoss)
						BaseEngine->playerData.bossFlag = FlagValueForCurrentBoss;
					Self->InternalState = BS_Results;
				}
			}
			else AttackFailedPlayer = 1;
			BattleScene_Timer = 0.f;
		}
		else
		{
			// Do some attack animation before changing states
			BattleScene_Timer += (float)Delta;
			if (BattleScene_Timer > AttackAnimationTime)
			{
				EnemyIsAttacking = AttackFailedPlayer = AttackAnimationRunning = 0;
				BattleScene_Timer = 0.f;
				RenderShield = 1;
				Self->InternalState = BS_EnemyTurn;
			}
		}
		break;
	case BS_PlayerTurnRun:
		if (MeterActive)
		{
			BarLogic((float)CurrentEnemy.spd * 100 * BarSpeedRatio, Delta);
			AttackAnimationRunning = 0;
		}
		else if (!AttackAnimationRunning && !AttackFailedPlayer) {
			// It's time to do damage!
			int CurrentIndex = (int)(JustMeterPercent * 0.01f * BarLength);
			if (CurrentIndex >= AttackThreshold.x && CurrentIndex <= AttackThreshold.y)
			{
				if (CurrentEnemyType == Boss_DatBoiLv1 ||
					CurrentEnemyType == Boss_DatBoiLv2 || 
					CurrentEnemyType == Boss_DatBoiLv3 || 
					CurrentEnemyType == Boss_DatBoiLv4 || 
					CurrentEnemyType == Boss_DatBoiLv5)
				BaseEngine->InternalSceneSystem.SetCurrentScene(&BaseEngine->InternalSceneSystem, SS_WorldView);
				else
				{
					if (BaseEngine->InternalSceneSystem.InternalEncounterHandler.PreviousSceneWasDungeon)
						BaseEngine->InternalSceneSystem.SetCurrentScene(&BaseEngine->InternalSceneSystem, SS_Dungeon);
					else BaseEngine->InternalSceneSystem.SetCurrentScene(&BaseEngine->InternalSceneSystem, SS_WorldView);
				}
			}
			else AttackFailedPlayer = 1;
			BattleScene_Timer = 0.f;
		}
		else
		{
			// Do some attack animation before changing states
			BattleScene_Timer += (float)Delta;
			if (BattleScene_Timer > AttackAnimationTime)
			{
				EnemyIsAttacking = AttackFailedPlayer = AttackAnimationRunning = 0;
				BattleScene_Timer = 0.f;
				RenderShield = 1;
				Self->InternalState = BS_EnemyTurn;
			}
		}
		break;
	case BS_EnemyTurn:
		BattleScene_Timer += (float)Delta;
		if (EnemyIsAttacking)
		{
			if (BattleScene_Timer > PerfectGuardTimeFrame * BarSpeedRatio)
			{
				// Player takes damage as he failed to perfect guard
				PlayerCurrentHealth -= CurrentEnemy.atk;
				AttackFailedPlayer = 1;
				EnemyIsAttacking = 0;
				BattleScene_Timer = 0.f;
				RenderShield = 0;
				if (PlayerCurrentHealth <= 0)
				{
					PlayerCurrentHealth = 0;
					Self->InternalState = BS_Results;
				}
			}
			else if (isKeyPressed(VK_SPACE))
			{
				// Perfect Guard
				AttackAnimationRunning = 1;
				EnemyIsAttacking = 0;
				BattleScene_Timer = 0.f;
				RenderShield = 0;
			}
		}
		else if (AttackAnimationRunning || AttackFailedPlayer)
		{
			if (BattleScene_Timer > AttackAnimationTime * 2)
			{
				// Time to change states!
				AttackFailedPlayer = AttackAnimationRunning = 0;
				BattleScene_Timer = 0.f;
				Self->InternalState = BS_PlayerTurnChoice;
			}
		}
		else if (BattleScene_Timer > (float)IntRandomizeRange(1, 5))
		{
			// Enemy Has Decided To Attack
			EnemyIsAttacking = 1;
			BattleScene_Timer = 0;
		}
		else if (isKeyPressed(VK_SPACE))
		{
			// Player takes damage as he failed to perfect guard
			PlayerCurrentHealth -= CurrentEnemy.atk;
			if (PlayerCurrentHealth <= 0)
			{
				PlayerCurrentHealth = 0;
				Self->InternalState = BS_Results;
			}
			// I'm guarding for no reason guard failed
			AttackFailedPlayer = 1;
			RenderShield = 0;
			BattleScene_Timer = 0.f;
		}
		break;
	case BS_Results:
		if (isKeyPressed(VK_RETURN))
		{
			if (CurrentEnemyType == Boss_DatBoiLv1 ||
				CurrentEnemyType == Boss_DatBoiLv2 ||
				CurrentEnemyType == Boss_DatBoiLv3 ||
				CurrentEnemyType == Boss_DatBoiLv4 ||
				CurrentEnemyType == Boss_DatBoiLv5)
				BaseEngine->InternalSceneSystem.SetCurrentScene(&BaseEngine->InternalSceneSystem, SS_WorldView);
			else
			{
				if (BaseEngine->InternalSceneSystem.InternalEncounterHandler.PreviousSceneWasDungeon)
					BaseEngine->InternalSceneSystem.SetCurrentScene(&BaseEngine->InternalSceneSystem, SS_Dungeon);
				else BaseEngine->InternalSceneSystem.SetCurrentScene(&BaseEngine->InternalSceneSystem, SS_WorldView);
			}
		}
		break;
	}
}

// Linked Render function that will be set to the InternalStateManager
void BattleScene_LinkedInternalRender(BattleScene* Self, Engine* BaseEngine)
{
	// Renders the appropriate scene
	switch (Self->InternalState)
	{
	case BS_Loading:
		break;
	case BS_PlayerTurnChoice:
		RenderBattle(Self, BaseEngine);
		Vector2 ChoiceStartPosition = Vec2((short)(BaseEngine->g_console->consoleSize.X*0.44f), (short)(BaseEngine->g_console->consoleSize.Y*0.75f));
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(ChoiceStartPosition.x, ChoiceStartPosition.y), "[ Fight ]", getColor(c_black, c_green));
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(ChoiceStartPosition.x, ChoiceStartPosition.y + 2), "[ Run ]", getColor(c_black, c_green));

		Vector2 PointerPosition;
		if (!PlayerTurnChoiceSelector)
			PointerPosition = Vec2(ChoiceStartPosition.x - 2, ChoiceStartPosition.y);
		else PointerPosition = Vec2(ChoiceStartPosition.x - 2, ChoiceStartPosition.y + 2);
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, PointerPosition, ">", getColor(c_black, c_aqua));
		break;
	case BS_PlayerTurnAttack:
		RenderBattle(Self, BaseEngine);
		RenderAttackMeter(Self, BaseEngine);
		if (AttackAnimationRunning)
			RenderAttackAnimation(Self, BaseEngine);
		else if (AttackFailedPlayer)
		{
			ResetCharArray(ScreenLineInfoBuffer);
			strcpy(ScreenLineInfoBuffer, "< Attack Failed! >");
			Vector2 RenderStartPosition = Vec2((short)(BaseEngine->g_console->consoleSize.X * 0.5f - (float)strlen(ScreenLineInfoBuffer) * 0.5f), (short)(BaseEngine->g_console->consoleSize.Y * 0.9f));
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, RenderStartPosition, ScreenLineInfoBuffer, getColor(c_black, c_red));
		}
		else {
			ResetCharArray(ScreenLineInfoBuffer);
			strcpy(ScreenLineInfoBuffer, "Hit <SPACE> to stop the meter! [Green Zone - Attack]");
			Vector2 RenderStartPosition = Vec2((short)(BaseEngine->g_console->consoleSize.X * 0.5f - (float)strlen(ScreenLineInfoBuffer) * 0.5f), (short)(BaseEngine->g_console->consoleSize.Y * 0.9f));
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, RenderStartPosition, ScreenLineInfoBuffer, getColor(c_black, c_lgrey));
		}
		break;
	case BS_PlayerTurnRun:
		RenderBattle(Self, BaseEngine);
		RenderAttackMeter(Self, BaseEngine);
		if (AttackFailedPlayer)
		{
			ResetCharArray(ScreenLineInfoBuffer);
			strcpy(ScreenLineInfoBuffer, "< Escape Failed! >");
			Vector2 RenderStartPosition = Vec2((short)(BaseEngine->g_console->consoleSize.X * 0.5f - (float)strlen(ScreenLineInfoBuffer) * 0.5f), (short)(BaseEngine->g_console->consoleSize.Y * 0.9f));
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, RenderStartPosition, ScreenLineInfoBuffer, getColor(c_black, c_red));
		}
		else {
			ResetCharArray(ScreenLineInfoBuffer);
			strcpy(ScreenLineInfoBuffer, "Hit <SPACE> to stop the meter! [Green Zone - Escape]");
			Vector2 RenderStartPosition = Vec2((short)(BaseEngine->g_console->consoleSize.X * 0.5f - (float)strlen(ScreenLineInfoBuffer) * 0.5f), (short)(BaseEngine->g_console->consoleSize.Y * 0.9f));
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, RenderStartPosition, ScreenLineInfoBuffer, getColor(c_black, c_lgrey));
		}
		break;
	case BS_EnemyTurn:
		RenderBattle(Self, BaseEngine);
		// Render the Shield
		{
			Vector2 ShieldPosition = Vec2((short)(BaseEngine->g_console->consoleSize.X * 0.5f), (short)(BaseEngine->g_console->consoleSize.Y * 0.75f));
			if (RenderShield == 1)
			{
				if (EnemyIsAttacking == 1)
					BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, Vec2(ShieldPosition.x - (short)(BattleScene_Loader_ShieldIcon.NumberOfColumns * 0.5f), ShieldPosition.y - (short)(BattleScene_Loader_ShieldIcon.NumberOfRows * 0.5f)), BattleScene_Loader_ShieldIcon.TextData, BattleScene_Loader_ShieldIcon.NumberOfRows, BattleScene_Loader_ShieldIcon.NumberOfColumns, getColor(c_black, c_red));
				else {
					if ((int)(BattleScene_Timer * 100) % 2 > 0)
						BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, Vec2(ShieldPosition.x - (short)(BattleScene_Loader_ShieldIcon.NumberOfColumns * 0.5f), ShieldPosition.y - (short)(BattleScene_Loader_ShieldIcon.NumberOfRows * 0.5f)), BattleScene_Loader_ShieldIcon.TextData, BattleScene_Loader_ShieldIcon.NumberOfRows, BattleScene_Loader_ShieldIcon.NumberOfColumns, getColor(c_black, c_white));
					else BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, Vec2(ShieldPosition.x - (short)(BattleScene_Loader_ShieldIcon.NumberOfColumns * 0.5f), ShieldPosition.y - (short)(BattleScene_Loader_ShieldIcon.NumberOfRows * 0.5f)), BattleScene_Loader_ShieldIcon.TextData, BattleScene_Loader_ShieldIcon.NumberOfRows, BattleScene_Loader_ShieldIcon.NumberOfColumns, getColor(c_black, c_blue));
				}
			}
		}
		{
			ResetCharArray(ScreenLineInfoBuffer);
			Vector2 RenderStartPosition;
			if (AttackAnimationRunning){
				strcpy(ScreenLineInfoBuffer, "< Perfect Guard! >");
				RenderStartPosition = Vec2((short)(BaseEngine->g_console->consoleSize.X * 0.5f - (float)strlen(ScreenLineInfoBuffer) * 0.5f), (short)(BaseEngine->g_console->consoleSize.Y * 0.775f));
			}	
			else if (AttackFailedPlayer){
				strcpy(ScreenLineInfoBuffer, "< Guard Failed... >");
				RenderStartPosition = Vec2((short)(BaseEngine->g_console->consoleSize.X * 0.5f - (float)strlen(ScreenLineInfoBuffer) * 0.5f), (short)(BaseEngine->g_console->consoleSize.Y * 0.775f));
			}
			else{
				strcpy(ScreenLineInfoBuffer, "When the shield is <RED>, hit <SPACE> to guard!");
				RenderStartPosition = Vec2((short)(BaseEngine->g_console->consoleSize.X * 0.5f - (float)strlen(ScreenLineInfoBuffer) * 0.5f), (short)(BaseEngine->g_console->consoleSize.Y * 0.9f));
			}
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, RenderStartPosition, ScreenLineInfoBuffer, getColor(c_black, c_lgrey));
		}
		
		break;
	case BS_Results:
		RenderResults(Self, BaseEngine);
		break;
	}
}

// Linked Exit function that will be set to the InternalStateManager
void BattleScene_LinkedInternalExit(BattleScene* Self)
{
	// Free the stuff initiallized in the Internal State Manager
	BattleScene_Loader_Layout.Exit(&BattleScene_Loader_Layout);
	BattleScene_Loader_ShieldIcon.Exit(&BattleScene_Loader_ShieldIcon);
	BattleScene_Loader_WinIcon.Exit(&BattleScene_Loader_WinIcon);
	BattleScene_Loader_LoseIcon.Exit(&BattleScene_Loader_LoseIcon);
	//FreeEnemy(&CurrentEnemy);
}

// Scene Based Functions
void RenderBattle(BattleScene* Self, Engine* BaseEngine)
{
	ResetCharArray(ScreenLineInfoBuffer);
	char TempCharArray[10];
	// Render the enemy
	BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, Vec2((short)(BaseEngine->g_console->consoleSize.X * 0.5f) - (short)(CurrentEnemy.spriteColumns * 0.5f), (short)(BaseEngine->g_console->consoleSize.Y * 0.5f) - (short)(CurrentEnemy.spriteRows*0.75f)), CurrentEnemy.sprite, CurrentEnemy.spriteRows, CurrentEnemy.spriteColumns, getColor(c_black, c_red));
	// Render the layout
	BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, Vec2(0, 0), BattleScene_Loader_Layout.TextData, BattleScene_Loader_Layout.NumberOfRows, BattleScene_Loader_Layout.NumberOfColumns, getColor(c_black, c_dgrey));
	BaseEngine->g_console->replace_withColor(BaseEngine->g_console, '#', 219, getColor(c_black, c_dgrey));
	// Render the text on screen
	// Enemy Name
	strcpy(ScreenLineInfoBuffer, CurrentEnemy.name);
	strcat(ScreenLineInfoBuffer, " (Level: ");
	sprintf(TempCharArray, "%d", CurrentEnemy.lvl);
	strcat(ScreenLineInfoBuffer, TempCharArray);
	strcat(ScreenLineInfoBuffer, ")");
	BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2((short)(BaseEngine->g_console->consoleSize.X * 0.35f) - (short)(strlen(ScreenLineInfoBuffer) * 0.5f), (short)(BaseEngine->g_console->consoleSize.Y * 0.1f)), ScreenLineInfoBuffer, getColor(c_black, c_yellow));
	// Enemy Health
	sprintf(TempCharArray, "%d", EnemyCurrentHealth);
	strcpy(ScreenLineInfoBuffer, "Health: ");
	strcat(ScreenLineInfoBuffer, TempCharArray);
	strcat(ScreenLineInfoBuffer, " / ");
	sprintf(TempCharArray, "%d", CurrentEnemy.hp);
	strcat(ScreenLineInfoBuffer, TempCharArray);
	BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2((short)(BaseEngine->g_console->consoleSize.X * 0.65f) - (short)(strlen(ScreenLineInfoBuffer) * 0.5f), (short)(BaseEngine->g_console->consoleSize.Y * 0.1f)), ScreenLineInfoBuffer, getColor(c_black, c_yellow));

	ResetCharArray(ScreenLineInfoBuffer);
	ResetCharArray(TempCharArray);
	// Player Stats
	// Player Name
	strcpy(ScreenLineInfoBuffer, "Player");
	strcat(ScreenLineInfoBuffer, " (Level: ");
	sprintf(TempCharArray, "%d", BaseEngine->playerData.lvl);
	strcat(ScreenLineInfoBuffer, TempCharArray);
	strcat(ScreenLineInfoBuffer, ")");
	BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2((short)(BaseEngine->g_console->consoleSize.X * 0.05f + 1), (short)(BaseEngine->g_console->consoleSize.Y * 0.675f)), ScreenLineInfoBuffer, getColor(c_black, c_aqua));
	
	ResetCharArray(ScreenLineInfoBuffer);
	ResetCharArray(TempCharArray);

	// Player Health
	sprintf(TempCharArray, "%d", PlayerCurrentHealth);
	strcpy(ScreenLineInfoBuffer, "HP: ");
	strcat(ScreenLineInfoBuffer, TempCharArray);
	strcat(ScreenLineInfoBuffer, " / ");
	sprintf(TempCharArray, "%d", Get_PlayerHP(&BaseEngine->playerData));
	strcat(ScreenLineInfoBuffer, TempCharArray);
	BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2((short)(BaseEngine->g_console->consoleSize.X * 0.05f + 1), (short)(BaseEngine->g_console->consoleSize.Y * 0.675f + 2)), ScreenLineInfoBuffer, getColor(c_black, c_aqua));

	ResetCharArray(ScreenLineInfoBuffer);
	ResetCharArray(TempCharArray);

	// Player Attack
	strcat(ScreenLineInfoBuffer, "Atk: ");
	sprintf(TempCharArray, "%d", Get_PlayerATK(&BaseEngine->playerData));
	strcat(ScreenLineInfoBuffer, TempCharArray);
	BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2((short)(BaseEngine->g_console->consoleSize.X * 0.05f + 1), (short)(BaseEngine->g_console->consoleSize.Y * 0.675f + 4)), ScreenLineInfoBuffer, getColor(c_black, c_aqua));

}

void RenderAttackMeter(BattleScene* Self, Engine* BaseEngine)
{
	ResetCharArray(ScreenLineInfoBuffer);

	// The index at which the bar is at now
	int CurrentIndex = (int)(JustMeterPercent * 0.01f * BarLength);
	int NumberOfCharactersToRender = BarLength + 4; // I want to render brackets and spaces before and after the bar
	Vector2 RenderStartPosition = Vec2((short)(BaseEngine->g_console->consoleSize.X*0.5f - (float)NumberOfCharactersToRender * 0.5f), (short)(BaseEngine->g_console->consoleSize.Y*0.75f));

	// Render my braces and spaces
	BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, RenderStartPosition, "[ ", getColor(c_black, c_white));
	BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(RenderStartPosition.x + BarLength + 2, RenderStartPosition.y), " ]", getColor(c_black, c_white));

	// Render the bar
	for (int i = 0; i < BarLength; ++i)
	{
		Vector2 RenderPosition = Vec2(RenderStartPosition.x + 2 + i, RenderStartPosition.y);
		if (i == CurrentIndex)
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, RenderPosition, "X", getColor(c_black, c_aqua));
		else
		{
			if (i >= AttackThreshold.x && i <= AttackThreshold.y)
				BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, RenderPosition, "-", getColor(c_black, c_dgreen));
			else BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, RenderPosition, "-", getColor(c_black, c_dred));
		}
		if (i == AttackThreshold.x || i == AttackThreshold.y)
		{
			// Draw Threshold
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(RenderPosition.x, RenderPosition.y + 1), "|", getColor(c_black, c_green));
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(RenderPosition.x, RenderPosition.y - 1), "|", getColor(c_black, c_green));
		}
	}
}

void RenderAttackAnimation(BattleScene* Self, Engine* BaseEngine)
{
	Vector2 RenderStartPosition = Vec2((short)(BaseEngine->g_console->consoleSize.X*0.5f - (float)CurrentEnemy.spriteColumns), (short)(BaseEngine->g_console->consoleSize.Y * 0.5f) - (short)(CurrentEnemy.spriteRows*0.25f));
	int NumberOfDashesToDraw = (int)((BattleScene_Timer / AttackAnimationTime) * CurrentEnemy.spriteColumns* 2);
	// Render Attack Animation
 	for (int i = 0; i < NumberOfDashesToDraw - 1; ++i)
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(RenderStartPosition.x + i, RenderStartPosition.y), "-", getColor(c_black, c_lime));
	BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(RenderStartPosition.x + NumberOfDashesToDraw - 1, RenderStartPosition.y), ">", getColor(c_black, c_green));

}

void RenderResults(BattleScene* Self, Engine* BaseEngine)
{
	ResetCharArray(ScreenLineInfoBuffer);
	char TempCharArray[10];

	// Render the layout
	BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, Vec2(0, 0), BattleScene_Loader_Layout.TextData, BattleScene_Loader_Layout.NumberOfRows, BattleScene_Loader_Layout.NumberOfColumns, getColor(c_black, c_dgrey));
	BaseEngine->g_console->replace_withColor(BaseEngine->g_console, '#', 219, getColor(c_black, c_dgrey));
	// Render the appropriate icon & text
	Vector2 IconPosition = Vec2((short)(BaseEngine->g_console->consoleSize.X*0.5f), (short)(BaseEngine->g_console->consoleSize.Y * 0.35f));
	Vector2 TextRenderStartPosition = Vec2((short)(BaseEngine->g_console->consoleSize.X*0.5f), (short)(BaseEngine->g_console->consoleSize.Y * 0.7f));
	// Player Lost
	if (PlayerCurrentHealth <= 0)
	{
		// Lose Icon
		BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, Vec2(IconPosition.x - (short)((float)BattleScene_Loader_LoseIcon.NumberOfColumns * 0.5f), IconPosition.y - (short)((float)BattleScene_Loader_LoseIcon.NumberOfRows * 0.5f)), BattleScene_Loader_LoseIcon.TextData, BattleScene_Loader_LoseIcon.NumberOfRows, BattleScene_Loader_LoseIcon.NumberOfColumns, getColor(c_black, c_red));
		BaseEngine->g_console->replace_withColor(BaseEngine->g_console, '#', 219, getColor(c_black, c_red));
		// Loss Text
		ResetCharArray(ScreenLineInfoBuffer);
		strcpy(ScreenLineInfoBuffer, "< You have lost all your earned XP for your current level... >");
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2((short)((float)TextRenderStartPosition.x - (float)strlen(ScreenLineInfoBuffer) * 0.5f), (short)((float)TextRenderStartPosition.y)), ScreenLineInfoBuffer, getColor(c_black, c_red));

		// XP Text
		ResetCharArray(ScreenLineInfoBuffer);
		strcpy(ScreenLineInfoBuffer, "< You need ");
		sprintf(TempCharArray, "%d", Get_PlayerRequiredEXPForLevel(&BaseEngine->playerData));
		strcat(ScreenLineInfoBuffer, TempCharArray);
		strcat(ScreenLineInfoBuffer, " more XP to level up... >");
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2((short)((float)TextRenderStartPosition.x - (float)strlen(ScreenLineInfoBuffer) * 0.5f), (short)((float)TextRenderStartPosition.y + 2)), ScreenLineInfoBuffer, getColor(c_black, c_dgrey));
	
		// Input Text
		ResetCharArray(ScreenLineInfoBuffer);
		strcpy(ScreenLineInfoBuffer, "Press <ENTER> to return to the world!");
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2((short)((float)TextRenderStartPosition.x - (float)strlen(ScreenLineInfoBuffer) * 0.5f), (short)((float)TextRenderStartPosition.y + 5)), ScreenLineInfoBuffer, getColor(c_black, c_lgrey));

	}
	// Player Won
	else if (EnemyCurrentHealth <= 0)
	{
		BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, Vec2(IconPosition.x - (short)((float)BattleScene_Loader_WinIcon.NumberOfColumns * 0.5f), IconPosition.y - (short)((float)BattleScene_Loader_WinIcon.NumberOfRows * 0.5f)), BattleScene_Loader_WinIcon.TextData, BattleScene_Loader_WinIcon.NumberOfRows, BattleScene_Loader_WinIcon.NumberOfColumns, getColor(c_black, c_green));
		BaseEngine->g_console->replace_withColor(BaseEngine->g_console, '#', 219, getColor(c_black, c_green));
		// Win Text
		ResetCharArray(ScreenLineInfoBuffer);
		strcpy(ScreenLineInfoBuffer, "< You have defeated the enemy! >");
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2((short)((float)TextRenderStartPosition.x - (float)strlen(ScreenLineInfoBuffer) * 0.5f), (short)((float)TextRenderStartPosition.y)), ScreenLineInfoBuffer, getColor(c_black, c_green));

		if (DidPlayerLevel == 0)
		{
			// XP Text
			ResetCharArray(ScreenLineInfoBuffer);
			strcpy(ScreenLineInfoBuffer, "< ");
			sprintf(TempCharArray, "%d", Get_PlayerRemainingEXPForLevel(&BaseEngine->playerData));
			strcat(ScreenLineInfoBuffer, TempCharArray);
			strcat(ScreenLineInfoBuffer, " more XP to level up, earned ");
			sprintf(TempCharArray, "%d", CurrentEnemy.lvl * XPScalingFactor);
			strcat(ScreenLineInfoBuffer, TempCharArray);
			strcat(ScreenLineInfoBuffer, " XP this battle! > ");
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2((short)((float)TextRenderStartPosition.x - (float)strlen(ScreenLineInfoBuffer) * 0.5f), (short)((float)TextRenderStartPosition.y + 2)), ScreenLineInfoBuffer, getColor(c_black, c_dgrey));
		}
		else {
			// Player Leveled Up!
			ResetCharArray(ScreenLineInfoBuffer);
			strcpy(ScreenLineInfoBuffer, "< You leveled up to Level: ");
			sprintf(TempCharArray, "%d", BaseEngine->playerData.lvl);
			strcat(ScreenLineInfoBuffer, TempCharArray);
			strcat(ScreenLineInfoBuffer, ", and earned ");
			sprintf(TempCharArray, "%d", CurrentEnemy.lvl * XPScalingFactor);
			strcat(ScreenLineInfoBuffer, TempCharArray);
			strcat(ScreenLineInfoBuffer, " XP this battle! > ");
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2((short)((float)TextRenderStartPosition.x - (float)strlen(ScreenLineInfoBuffer) * 0.5f), (short)((float)TextRenderStartPosition.y + 2)), ScreenLineInfoBuffer, getColor(c_black, c_dgrey));
		}
		// Input Text
		ResetCharArray(ScreenLineInfoBuffer);
		strcpy(ScreenLineInfoBuffer, "Press <ENTER> to return to the world!");
		BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2((short)((float)TextRenderStartPosition.x - (float)strlen(ScreenLineInfoBuffer) * 0.5f), (short)((float)TextRenderStartPosition.y + 6)), ScreenLineInfoBuffer, getColor(c_black, c_lgrey));

	}
}

// Gameplay Logic Code
void BarLogic(float BarSpeed, double Delta)
{
	// I'm triggering the attack now!
	if (isKeyPressed(VK_SPACE))
	{
		MeterActive = 0;
	}
	else {
		// Update the bar's percent accordingly
		JustMeterPercent += BarSpeed * (float)Delta * (float)BarIncrementationDirection;

		// I'm exceeding a hundred percent time to decrement
		if (JustMeterPercent > 99.f)
		{
			JustMeterPercent = 99.f;
			BarIncrementationDirection = -1;
		}
		else if (JustMeterPercent < 0.f)
		{
			JustMeterPercent = 0.f;
			BarIncrementationDirection = 1;
		}
	}
}

Vector2 RandomizeOptimalBarThresholds()
{
	Vector2 Range = Vec2(IntRandomizeRange(0, BarLength - 1), IntRandomizeRange(0, BarLength - 1));
	if (Range.x > Range.y)
		Range = Vec2(Range.y, Range.x);
	return Range;
}