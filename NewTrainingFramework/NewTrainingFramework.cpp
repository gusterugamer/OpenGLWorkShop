#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Globals.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include <iostream>

ResourceManager* RM = ResourceManager::GetInstance();
SceneManager* SM = SceneManager::GetInstance();

int Init(ESContext *esContext)
{
	RM->Init();
	SM->Init();		
	
	return 0;
}

void Draw(ESContext *esContext)
{		
	GLCall(glEnable(GL_DEPTH_TEST));	
	GLCall(glEnable(GL_BLEND));		
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));	
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	SM->Draw();		
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);	
	
}

void Update(ESContext *esContext, float deltaTime)
{	
	SM->Update(esContext, deltaTime);
}

void Key ( ESContext *esContext, unsigned char key,bool bIsPressed)
{
}

void CleanUp()
{	
}	

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Gusteru Engine", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();


	printf("Press any key...\n");

	
	return 0;
}

