#include "zviewport.h"

void ZViewport::Use()
{
	glViewport(X, Y, Width, Height);
}
