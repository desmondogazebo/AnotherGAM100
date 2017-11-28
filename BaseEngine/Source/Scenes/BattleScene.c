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
TextDataLoader BattleScene_Loader_Layout;
Enemy CurrentEnemy;
int EnemyCurrentHealth;
char ScreenLineInfoBuffer[100];
char TempCharArray[10];
int PlayerTurnChoiceSelector;
// A meter for handling just attacks, just guards & just escapes.
float JustMeterPercent;
float BattleScene_Timer;
int BarIncrementationDirection;
int MeterActive;
int BarLength = 30;
Vector2 AttackThreshold;
short AttackAnimationRunning;
float AttackAnimationTime = 0.5f;

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
void RenderLoading(BattleScene* Self, Engine* BaseEngine);
void RenderBattle(BattleScene* Self, Engine* BaseEngine);
void RenderAttackMeter(BattleScene* Self, Engine* BaseEngine);
void RenderAttackAnimation(BattleScene* Self, Engine* BaseEngine);
void RenderResults(BattleScene* Self, Engine* BaseEngine);

// Gameplay Logic Code
void BarLogic(float BarSpeed, double Delta);
Vector2 RandomizeOptimalBarThresholds();

// Utility Functions
void ResetCharArray(char* Array)
{
	memset(Array, 0, sizeof(char) * strlen(Array));
}

int IntRandomizeRange(int Lower, int Upper) {
	return((rand() % (Upper - Lower + 1)) + Lower);
}

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
	Self->InternalState = BS_PlayerTurnChoice;

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
	// Load the sprites that will be used in the battle scene
	BattleScene_Loader_Layout.LoadResource(&BattleScene_Loader_Layout, "Resources/Battle/Borders.txt");
	// Load the enemy
	PopulateEnemy(&CurrentEnemy, "Resources/Enemy/Goblin.txt");
	EnemyCurrentHealth = CurrentEnemy.hp;
	AttackAnimationRunning = PlayerTurnChoiceSelector = 0;
	BattleScene_Timer = 0.f;
	BarIncrementationDirection = 1;
}

// Linked Update function that will be set to the InternalStateManager
void BattleScene_LinkedInternalUpdate(BattleScene* Self, Engine* BaseEngine, double Delta)
{
	// Do some state logic for the internal state manager
	// Clearing the input buffer before handling checks
	isKeyPressed(VK_SPACE);
	switch (Self->InternalState)
	{
	case BS_Loading:
		//Self->InternalState = ;
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
				break;
			case 1:
				// I will Run
				Self->InternalState = BS_PlayerTurnRun;
				break;
			}
		}
		break;
	case BS_PlayerTurnAttack:
		if (MeterActive)
		{
			BarLogic((float)CurrentEnemy.spd * 50, Delta);
			AttackAnimationRunning = 0;
		}
		else if (!AttackAnimationRunning){
			// It's time to do damage!
			int CurrentIndex = (int)(JustMeterPercent * 0.01f * BarLength);
			if (CurrentIndex >= AttackThreshold.x && CurrentIndex <= AttackThreshold.y)
				EnemyCurrentHealth -= 1;
			else Self->InternalState = BS_PlayerTurnChoice;
			BattleScene_Timer = 0.f;
			AttackAnimationRunning = 1;
		}
		else if (AttackAnimationRunning)
		{
			// Do some attack animation before changing states
			BattleScene_Timer += (float)Delta;
			if (BattleScene_Timer > AttackAnimationTime)
			{
				AttackAnimationRunning = 0;
				Self->InternalState = BS_PlayerTurnChoice;
			}
		}
		break;
	case BS_PlayerTurnRun:
		break;
	case BS_EnemyTurn:
		break;
	case BS_BattleSequence:
		break;
	case BS_Results:
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
		{
			RenderAttackAnimation(Self, BaseEngine);
		}
		break;
	case BS_PlayerTurnRun:
		RenderBattle(Self, BaseEngine);
		break;
	case BS_EnemyTurn:
		RenderBattle(Self, BaseEngine);
		break;
	case BS_BattleSequence:
		RenderBattle(Self, BaseEngine);
		break;
	case BS_Results:
		break;
	}
}

// Linked Exit function that will be set to the InternalStateManager
void BattleScene_LinkedInternalExit(BattleScene* Self)
{
	// Free the stuff initiallized in the Internal State Manager
	BattleScene_Loader_Layout.Exit(&BattleScene_Loader_Layout);
	FreeEnemy(&CurrentEnemy);
}

// Scene Based Functions
void RenderLoading(BattleScene* Self, Engine* BaseEngine)
{
	ResetCharArray(ScreenLineInfoBuffer);
	ResetCharArray(TempCharArray);

}

void RenderBattle(BattleScene* Self, Engine* BaseEngine)
{
	ResetCharArray(ScreenLineInfoBuffer);
	ResetCharArray(TempCharArray);

	// Render the enemy
	BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, Vec2((short)(BaseEngine->g_console->consoleSize.X * 0.5f) - (short)(CurrentEnemy.spriteColumns * 0.5f), (short)(BaseEngine->g_console->consoleSize.Y * 0.5f) - (short)(CurrentEnemy.spriteRows*0.75f)), CurrentEnemy.sprite, CurrentEnemy.spriteRows, CurrentEnemy.spriteColumns, getColor(c_black, c_red));
	// Render the layout
	BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, Vec2(0, 0), BattleScene_Loader_Layout.TextData, BattleScene_Loader_Layout.NumberOfRows, BattleScene_Loader_Layout.NumberOfColumns, getColor(c_black, c_dgrey));
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
}

void RenderAttackMeter(BattleScene* Self, Engine* BaseEngine)
{
	ResetCharArray(ScreenLineInfoBuffer);
	ResetCharArray(TempCharArray);

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
	ResetCharArray(TempCharArray);

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