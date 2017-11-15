/******************************************************************************
filename    CustomStateManager.h
author      Rui An Ryan Lim
DP email    l.ruianryan@digipen.edu

Created on 7 November 2017

Brief Description:
A template on creating a customized state manager

******************************************************************************/
#include "CustomStateManager.h"
#include <stdio.h>

///****************************************************************************
// Private Variables
///****************************************************************************


///****************************************************************************
// Private Function Prototypes
///****************************************************************************
// State Manager Functions
// Linked Initiallize function that will be set to the struct's Initiallize
void CustomStateManager_LinkedInitiallize(CustomStateManager* Self);
// Linked Update function that will be set to the struct's Update
void CustomStateManager_LinkedUpdate(CustomStateManager* Self, double Delta);
// Linked Render function that will be set to the struct's Render
void CustomStateManager_LinkedRender(CustomStateManager* Self);
// Linked Exit function that will be set to the struct's Exit
void CustomStateManager_LinkedExit(CustomStateManager* Self);

// Internal State Manager Functions
// Linked initiallize function that will be set to the InternalStateManager.Initiallize
void CustomStateManager_LinkedInternalInitiallize(CustomStateManager* Self);
// Linked Update function that will be set to the InternalStateManager.Update
void CustomStateManager_LinkedInternalUpdate(CustomStateManager* Self, double Delta);
// Linked Render function that will be set to the InternalStateManager.Render
void CustomStateManager_LinkedInternalRender(CustomStateManager* Self);
// Linked Exit function that will be set to the InternalStateManager.Exit
void CustomStateManager_LinkedInternalExit(CustomStateManager* Self);

///****************************************************************************
// Function Definitions
///****************************************************************************
void CustomStateManager_Setup(CustomStateManager* Self)
{
	// Set up the InternalStateManager
	Self->InternalStateManager.Initiallize = CustomStateManager_LinkedInternalInitiallize;
	Self->InternalStateManager.Update = CustomStateManager_LinkedInternalUpdate;
	Self->InternalStateManager.Exit = CustomStateManager_LinkedInternalExit;

	// Set the current state
	Self->InternalState = CSM_Idle;

	// Set up the functions of this object
	Self->Update = CustomStateManager_LinkedUpdate;
	Self->Exit = CustomStateManager_LinkedExit;
}

// Linked Initiallize function that will be set to the struct's Initiallize
void CustomStateManager_LinkedInitiallize(CustomStateManager* Self)
{
	Self->InternalStateManager.Initiallize(Self);
}

// Linked Update function that will be set to the struct's Update
void CustomStateManager_LinkedUpdate(CustomStateManager* Self, double Delta)
{
	Self->InternalStateManager.Update(Self, Delta);
}

// Linked Render function that will be set to the struct's Render
void CustomStateManager_LinkedRender(CustomStateManager* Self)
{
	Self->InternalStateManager.Render(Self);
}

// Linked Exit function that will be set to the struct's Exit
void CustomStateManager_LinkedExit(CustomStateManager* Self)
{
	Self->InternalStateManager.Exit(Self);
}

// Linked Initiallize function that will be set to the InternalStateManager
void CustomStateManager_LinkedInternalInitiallize(CustomStateManager* Self)
{
	// Here I will initiallize the internal state manager
	/// Miscellaneous stuff to initiallize
	/// Nothing here in the mean time as the base state manager has no internal entities
	/// This function should generally only be used if we are using some other modified state manager
}

// Linked Update function that will be set to the InternalStateManager
void CustomStateManager_LinkedInternalUpdate(CustomStateManager* Self, double Delta)
{
	// Do some state logic for the internal state manager
	// Testing code state cycling
	switch (Self->InternalState)
	{
	case CSM_Idle:
		Self->InternalState = CSM_Active;
		break;
	case CSM_Active:
		Self->InternalState = CSM_Inactive;
		break;
	case CSM_Inactive:
		Self->InternalState = CSM_Idle;
		break;
	}
}

// Linked Render function that will be set to the InternalStateManager
void CustomStateManager_LinkedInternalRender(CustomStateManager* Self)
{
	// Renders the appropriate scene
	switch (Self->InternalState)
	{
	case CSM_Idle:
		break;
	case CSM_Active:
		break;
	case CSM_Inactive:
		break;
	}
}

// Linked Exit function that will be set to the InternalStateManager
void CustomStateManager_LinkedInternalExit(CustomStateManager* Self)
{
	// Free the stuff initiallized in the Internal State Manager
}