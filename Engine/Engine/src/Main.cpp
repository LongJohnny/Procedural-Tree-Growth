#include "Engine.h"
#include <iostream>
#include "apps/TreeGrowth.h"
#include "apps/ShadowMapping.h"
#include "apps/Blending.h"
#include "apps/TextRendering.h"
#include "apps/CylinderGeneration.h"

int main(int argc, char* argv[])
{
	//Engine* eng = new Cylinder();
	//Engine* eng = new TextRendering();
	//Engine* eng = new ShadowMapping();
	//Engine* eng = new TreeGrowth();
	Engine* eng = new Blending();
	eng->run();
	delete eng;
	exit(EXIT_SUCCESS);
}
