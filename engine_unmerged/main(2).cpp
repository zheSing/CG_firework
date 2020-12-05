#include <iostream>
#include <fstream>
#include <string>

#include <cstdio>

#include <glm/glm.hpp>


#include <chrono>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "firework.h"




int main()
{
	firework *fw = firework_new(mudan_t);

	firework_update(fw, 0.1);

	delete fw;
	return 0;
}

