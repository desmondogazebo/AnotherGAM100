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
// Linked Update function that will be set to the struct's Update
void Linked_Update(CustomStateManager* Self, float Delta);
// Linked Update function that will be set to the struct's Exit
void Linked_Exit(CustomStateManager* Self);

// Internal State Manager Functions
// Linked initiallize function that will be set to the InternalStateManager.Initiallize
void Linked_InternalInitiallize(CustomStateManager* Self);
// Linked Update function that will be set to the InternalStateManager.Update
void Linked_InternalUpdate(CustomStateManager* Self, float Delta);
// Linked Update function that will be set to the InternalStateManager.Exit
void Linked_InternalExit(CustomStateManager* Self);

///****************************************************************************
// Function Definitions
///****************************************************************************
void CustomStateManager_Setup(CustomStateManager* Self)
{
	// Set up the InternalStateManager
	Self->InternalStateManager.Initiallize = Linked_InternalInitiallize;
	Self->InternalStateManager.Update = Linked_InternalUpdate;
	Self->InternalStateManager.Exit = Linked_InternalExit;

	// Set the current state
	Self->InternalState = CSM_Idle;

	// Set up the functions of this object
	Self->Update = Linked_Update;
	Self->Exit = Linked_Exit;
}

// Linked Update function that will be set to the struct's Update
void Linked_Update(CustomStateManager* Self, float Delta)
{
	Self->InternalStateManager.Update(Self, Delta);
}

// Linked Update function that will be set to the struct's Exit
void Linked_Exit(CustomStateManager* Self)
{
	Self->InternalStateManager.Exit(Self);
}

// Linked Initiallize function that will be set to the InternalStateManager
void Linked_InternalInitiallize(CustomStateManager* Self)
{
	// Here I will initiallize the internal state manager
	/// Miscellaneous stuff to initiallize
	/// Nothing here in the mean time as the base state manager has no internal entities
	/// This function should generally only be used if we are using some other modified state manager
}

// Linked Update function that will be set to the InternalStateManager
void Linked_InternalUpdate(CustomStateManager* Self, float Delta)
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

// Linked Exit function that will be set to the InternalStateManager
void Linked_InternalExit(CustomStateManager* Self)
{
	// Free the stuff initiallized in the Internal State Manager
}