#include "Assets.h"

#include "Config.h"

void Assets::Load() {
	if (!font.loadFromFile(kFontPath)) throw;
	if (!logo_texture_.loadFromFile(kLogoPath)) throw;
	logo_texture_.setSmooth(true); // Сглаживание текстуры
	logo.setTexture(logo_texture_);
	logo.setPosition(
		sf::Vector2f(
			kIndentFieldX + kFieldSizePx + kIndentFieldX,
			kWindowHeight - 200
		)
	);
	logo.scale(0.8f, 0.8f); // Сжатие по сторонам
}
