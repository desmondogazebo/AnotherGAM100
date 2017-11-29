/******************************************************************************
filename    MenuScene.h
author      Keith Cheng
DP email    keith.cheng@digipen.edu

Brief Description:
The menu screen that occurs after the splash screen.
It is used to either transit to the game or exit

******************************************************************************/
#include "MenuScene.h"

#include <stdio.h>

// Inclusion of utility functions
#include "../Utilities/TextDataLoader.h"
#include "../Utilities/Utilities.h"

// Included for Rendering
#include "../Engine/BaseEngine.h"

///****************************************************************************
// Private Variables
///****************************************************************************
TextDataLoader MenuScene_Loader;


short menu_transitionCount = 0;
double menu_transitionTimer = 0;
double menu_transitionTimerDelay = 0.01;
short menu_waitToggle = 0;
double menu_transitionWaitDelay = 0.5;
TextDataLoader GameTransition_Loader;

short menu_instructionCount = 0;
short menu_instructionToggle = 0;
double menu_instructionTimer = 0;
double menu_instructionDelay = 0.01;
TextDataLoader Instructions_Loader;

///****************************************************************************
// Private Function Prototypes
///****************************************************************************
// State Manager Functions
// Linked Initiallize function that will be set to the struct's Initiallize
void MenuScene_LinkedInitiallize(MenuScene* Self);
// Linked Update function that will be set to the struct's Update
void MenuScene_LinkedUpdate(MenuScene* Self, Engine* BaseEngine, double Delta);
// Linked Render function that will be set to the struct's Render
void MenuScene_LinkedRender(MenuScene* Self, Engine* BaseEngine);
// Linked Exit function that will be set to the struct's Exit
void MenuScene_LinkedExit(MenuScene* Self);

// Internal State Manager Functions
// Linked initiallize function that will be set to the InternalStateManager.Initiallize
void MenuScene_LinkedInternalInitiallize(MenuScene* Self);
// Linked Update function that will be set to the InternalStateManager.Update
void MenuScene_LinkedInternalUpdate(MenuScene* Self, Engine* BaseEngine, double Delta);
// Linked Render function that will be set to the InternalStateManager.Render
void MenuScene_LinkedInternalRender(MenuScene* Self, Engine* BaseEngine);
// Linked Exit function that will be set to the InternalStateManager.Exit
void MenuScene_LinkedInternalExit(MenuScene* Self);

// Functions to split code up
void Menu_Controls(MenuScene* Self, Engine* BaseEngine);

void Menu_Transition(MenuScene* Self, Engine* BaseEngine, double Delta);

void Menu_InstructionsUpdate(MenuScene* Self, Engine* BaseEngine, double Delta);

///****************************************************************************
// Function Definitions
///****************************************************************************
void MenuScene_Setup(MenuScene* Self)
{
	// Set up the InternalStateManager
	Self->InternalStateManager.Initiallize = MenuScene_LinkedInternalInitiallize;
	Self->InternalStateManager.Update = MenuScene_LinkedInternalUpdate;
	Self->InternalStateManager.Render = MenuScene_LinkedInternalRender;
	Self->InternalStateManager.Exit = MenuScene_LinkedInternalExit;

	// Set the current state
	Self->InternalState = CSM_MENU;
	Self->selectedMenuState = SEL_START;

	// Set up the functions of this object
	Self->Initiallize = MenuScene_LinkedInitiallize;
	Self->Update = MenuScene_LinkedUpdate;
	Self->Render = MenuScene_LinkedRender;
	Self->Exit = MenuScene_LinkedExit;
}

// Linked Initiallize function that will be set to the struct's Initiallize
void MenuScene_LinkedInitiallize(MenuScene* Self)
{
	Self->InternalStateManager.Initiallize(Self);
}

// Linked Update function that will be set to the struct's Update
void MenuScene_LinkedUpdate(MenuScene* Self, Engine* BaseEngine, double Delta)
{
	Self->InternalStateManager.Update(Self, BaseEngine, Delta);
}

// Linked Render function that will be set to the struct's Render
void MenuScene_LinkedRender(MenuScene* Self, Engine* BaseEngine)
{
	Self->InternalStateManager.Render(Self, BaseEngine);
}

// Linked Exit function that will be set to the struct's Exit
void MenuScene_LinkedExit(MenuScene* Self)
{
	Self->InternalStateManager.Exit(Self);
}

// Linked Initiallize function that will be set to the InternalStateManager
void MenuScene_LinkedInternalInitiallize(MenuScene* Self)
{
	// Here I will initiallize the internal state manager
	// Setup the loader that I am about to use.
	TextDataLoader_Setup(&MenuScene_Loader);
	TextDataLoader_Setup(&GameTransition_Loader);
	TextDataLoader_Setup(&Instructions_Loader);
	// Load the sprites that will be used in the battle scene
	MenuScene_Loader.LoadResource(&MenuScene_Loader, "Resources/MainMenuLogo.txt");
	GameTransition_Loader.LoadResource(&GameTransition_Loader, "Resources/GameTransition.txt");
	Instructions_Loader.LoadResource(&Instructions_Loader, "Resources/Information/Instructions.txt");

	// Initialize variables
	Self->wKeyPressed = 0;
	Self->sKeyPressed = 0;
}

// Linked Update function that will be set to the InternalStateManager
void MenuScene_LinkedInternalUpdate(MenuScene* Self, Engine* BaseEngine, double Delta)
{
	// Do some state logic for the internal state manager
	switch (Self->InternalState)
	{
	case CSM_MENU:
		Menu_Controls(Self, BaseEngine);
		break;
	case CSM_TRANSITION:
		Menu_Transition(Self, BaseEngine, Delta);
		break;
	case CSM_INSTRUCTION:
		Menu_InstructionsUpdate(Self, BaseEngine, Delta);
	default: 
		break;
	}
}

// Linked Render function that will be set to the InternalStateManager
void MenuScene_LinkedInternalRender(MenuScene* Self, Engine* BaseEngine)
{
	// Renders the appropriate scene
	// Render in every state
	BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, Vec2(0, 0), MenuScene_Loader.TextData, MenuScene_Loader.NumberOfRows, MenuScene_Loader.NumberOfColumns, getColor(c_black, c_aqua));
	{
		WORD selectedColor = getColor(c_aqua, c_black);
		WORD normalColor = getColor(c_black, c_white);
		if (Self->selectedMenuState == SEL_START)
		{
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(20, 13), "Start", selectedColor);
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(20, 15), "Instructions", normalColor);
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(20, 17), "Exit", normalColor);
		}
		else if (Self->selectedMenuState == SEL_EXIT)
		{
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(20, 13), "Start", normalColor);
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(20, 15), "Instructions", normalColor);
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(20, 17), "Exit", selectedColor);
		}
		else if (Self->selectedMenuState == SEL_INSTR)
		{
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(20, 13), "Start", normalColor);
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(20, 15), "Instructions", selectedColor);
			BaseEngine->g_console->text_WriteToBuffer(BaseEngine->g_console, Vec2(20, 17), "Exit", normalColor);
		}
	}

	switch (Self->InternalState)
	{
	case CSM_MENU:

		break;
	case CSM_TRANSITION:
		{
			Vector2 location = { --BaseEngine->g_console->consoleSize.X + menu_transitionCount, 0 };
			BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, location, GameTransition_Loader.TextData, GameTransition_Loader.NumberOfRows, GameTransition_Loader.NumberOfColumns, getColor(c_black, c_white));
		}
		break;
	case CSM_INSTRUCTION:
	{
		Vector2 location = { --BaseEngine->g_console->consoleSize.X + menu_instructionCount, 0 };
		BaseEngine->g_console->sprite_WriteToBuffer(BaseEngine->g_console, location, Instructions_Loader.TextData, Instructions_Loader.NumberOfRows, Instructions_Loader.NumberOfColumns, getColor(c_black, c_white));
	}
	default:
		break;
	}
}

// Linked Exit function that will be set to the InternalStateManager
void MenuScene_LinkedInternalExit(MenuScene* Self)
{
	// Free the stuff initiallized in the Internal State Manager
	MenuScene_Loader.Exit(&MenuScene_Loader);
	GameTransition_Loader.Exit(&GameTransition_Loader);
	Instructions_Loader.Exit(&Instructions_Loader);
}

void Menu_Controls(MenuScene* Self, Engine* BaseEngine)
{
	if (isKeyPressed('W'))
	{
		if (Self->wKeyPressed == 0)
		{
			Self->wKeyPressed = 1;
			if ((int)Self->selectedMenuState == 0)
			{
				Self->selectedMenuState = (SEL_TOTAL - 1);
			}
			else
			{
				Self->selectedMenuState--;
			}
		}
	}
	else
	{
		Self->wKeyPressed = 0;
	}

	if (isKeyPressed('S'))
	{
		if (Self->sKeyPressed == 0)
		{
			Self->sKeyPressed = 1;
			if ((int)Self->selectedMenuState == (SEL_TOTAL - 1))
			{
				Self->selectedMenuState = 0;
			}
			else
			{
				Self->selectedMenuState++;
			}
		}
	}
	else
	{
		Self->sKeyPressed = 0;
	}

	if (isKeyPressed(VK_RETURN))
	{
		if (Self->selectedMenuState == SEL_START)
		{
			Self->InternalState = CSM_TRANSITION;
			//BaseEngine->InternalSceneSystem.SetCurrentScene(&BaseEngine->InternalSceneSystem, SS_WorldView);
		}
		else if (Self->selectedMenuState == SEL_EXIT)
		{
			BaseEngine->g_quitGame = 1;
		}
		else if (Self->selectedMenuState == SEL_INSTR)
		{
			Self->InternalState = CSM_INSTRUCTION;
		}
	}
}

void Menu_Transition(MenuScene* Self, Engine* BaseEngine, double Delta)
{
	menu_transitionTimer += Delta;

	if (menu_waitToggle == 0)
	{
		if (menu_transitionTimer > menu_transitionTimerDelay)
		{
			menu_transitionCount++;
			menu_transitionTimer = 0;
			if (menu_transitionCount == 80)
			{
				menu_waitToggle = 1;
			}
		}
	}
	else if (menu_waitToggle == 1)
	{
		if (menu_transitionTimer > menu_transitionWaitDelay)
		{
			menu_transitionCount = 0;
			menu_transitionTimer = 0;
			menu_waitToggle = 0;
			BaseEngine->InternalSceneSystem.SetCurrentScene(&BaseEngine->InternalSceneSystem, SS_WorldView);
		}
	}
}

void Menu_InstructionsUpdate(MenuScene* Self, Engine* BaseEngine, double Delta)
{

	short tempToggle = 0;
	if (isKeyPressed(VK_RETURN))
	{
		tempToggle = 1;
	}

	if (menu_instructionToggle == 0)
	{
		if ((menu_instructionTimer += Delta) > menu_instructionDelay)
		{
			menu_instructionTimer = 0;
			if (menu_instructionCount < 80)
			{
				menu_instructionCount++;
			}
			else
			{
				menu_instructionToggle = tempToggle;
			}
		}
	}
	else if (menu_instructionToggle == 1)
	{
		if ((menu_instructionTimer += Delta) > menu_instructionDelay)
		{
			menu_instructionTimer = 0;
			menu_instructionCount--;
			if (menu_instructionCount <= 0)
			{
				menu_instructionToggle = 0;
				Self->InternalState = CSM_MENU;
			}
		}
	}
}