#include "BaseEngine.h"
#include "..\Utilities\Utilities.h"
#include "..\Utilities\Vector2.h"
//PROTOTYPES
void Engine_init(Engine* ptr, unsigned short fps, Vec2 s_size, Vec2 f_size);
void Engine_update(Engine* ptr, Timer* t);
void Engine_render(Engine* ptr);
void Engine_exit(Engine* ptr);

/*
Function Name: init
Brief Description: initializer of the Engine
Parameters:
ptr : the engine pointer itself, to allow for internal referencing
fps : the desired fps to run the game
s_size : window size
f_size : font size
*/

void Engine_init(Engine* E, unsigned short fps, Vec2 s_size, Vec2 f_size)
{
	//Private function call
	E->g_console = MakeConsole();
	//Constructor call
	E->g_console->Init(E->g_console, s_size, "GAM100 PROJECT");
	//Set font of the console
	E->g_console->SetConsoleFont(E->g_console, f_size, L"Raster Fonts");

	//Initializing variables
	E->g_quitGame = 0;
	E->FPS = fps;
	E->frameTime = 1000 / E->FPS;

	//Creating the internal clock object
	E->g_timer = Timer_Create();
	//Initializing the clock object
	E->g_timer->Init(E->g_timer);

	/* How to use the TextDataLoader */
	/* Linking Example - Variable Order */
	TextDataLoader_Setup(&E->ldr);
	/* Load a file into a reference of the loader */
	E->ldr.LoadResource(&E->ldr, "Resources/DigiPenLogo(Unofficial).txt");

	//Test scene initialization
	E->testScene = Create_RoomTestScene();
	E->testScene.Init(&(E->testScene));

	// Added to showcase the custom state manager
	CustomStateManager_Setup(&CSM);
}

/*
Function Name: update
Brief Description: The main loop of the engine
Parameters:
ptr : the engine pointer itself, to allow for internal referencing
t : the pointer to the timer, should there be a need to use GetElapsedTime();
*/

void Engine_update(Engine* E, Timer* t)
{
	//TEST CODE
	if (isKeyPressed(VK_ESCAPE))
	{
		E->g_quitGame = 1;
	}
	// Added to showcase the custom state manager
	CSM.Update(&CSM, 0);

	//double dt = t->GetElapsedTime(t);

	E->testScene.Update(&(E->testScene), 0);
}

/*
Function Name: render
Brief Description: The render code
Parameters:
ptr : the engine pointer itself, to allow for internal referencing
*/
void Engine_render(Engine* E)
{
	//Clear the Screen every frame
	E->g_console->ClearBuffer(E->g_console, 0x0F);

	E->g_console->Ptr_writeToBuffer(E->g_console, E->testScene.currentRoom->mapToRender, E->testScene.currentRoom->Loader.NumberOfRows, E->testScene.currentRoom->Loader.NumberOfColumns, getColor(c_black, c_white));
	//TEST CODE
	Vec2 test = { 0, 0 };
	double i = 1 / E->g_timer->GetElapsedTime(E->g_timer);
	//NOTE THIS SECTION, THIS IS HOW YOU PRINT A DOUBLE IN CHAR*
	char* input = d_toString(i, 6); //PLEASE UNDERSTAND THAT THE DECIMAL IS 1 SF
	E->g_console->WriteToBuffer(E->g_console, test, input, getColor(c_black, c_purple));
	E->g_console->WriteToBuffer(E->g_console, E->testScene.player.position, "O", getColor(c_black, c_white));
	//free(input); //VERY IMPORTANT

				 //Send the new data to the Console
	E->g_console->FlushBufferToConsole(E->g_console);
}

/*
Function Name: m_shutdown
Brief Description: prefixed with an m, this is a supposed private function
to shutdown the Engine and free associated memory
Parameters:
ptr : the Engine pointer itself, to allow for internal referencing
*/
void Engine_exit(Engine* E)
{
	// exit the scene
	E->testScene.Exit(&E->testScene);

	E->ldr.Exit(&E->ldr);
	//stop the internal clock
	E->g_timer->Shutdown(E->g_timer);
	//release the memory
	free(E->g_timer);
	//ask the console to shutdown
	E->g_console->ShutdownConsole(E->g_console);
	//release the memory
	free(E->g_console);
	//finally, release the engine memory
	free(E);
}

/*
Function Name: MakeEngine
ACCESS LEVEL: GLOBAL
Brief Description: Allocates memory needed for the Engine entity and
binds the relevant functions to said entity.
*/
Engine* MakeEngine()
{
	//Allocation of memory
	Engine* e = (Engine*)malloc(sizeof(Engine));
	//Binding functions
	e->Init = &Engine_init;
	e->Render = &Engine_render;
	e->Update = &Engine_update;
	e->Shutdown = &Engine_exit;

	//Returns the modified Engine entity
	return e;
}