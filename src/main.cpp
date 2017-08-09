//
//  main.cpp
//  
//
//  Created by Bret Jackson on 1/29/17.
//
//

#include <stdio.h>

#include "App.h"

using namespace basicgraphics;

int main(int argc, char** argv)
{

	App *app = new App(argc, argv, "Physically Based Shaders", 1024, 768);
	app->run();
	delete app;

	return 0;

}
