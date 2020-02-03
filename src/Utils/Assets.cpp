#include "Assets.h"
#include "Config.h"

void Assets::Load() {
	if (!font.loadFromFile(FONT_PATH)) throw;
}
