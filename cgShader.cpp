#include "cgShader.h"
#include <iostream>
 void getEffectParam( CGeffect effect,
					const char* semantic,
					CGparameter* param )
{
	(*param) = cgGetEffectParameterBySemantic(effect, semantic);
	if (!(*param))
	{
		std::cerr << "getCGParam: Couldn't find parameter with " << semantic << " semantic\n";
		exit(1);
	}
}