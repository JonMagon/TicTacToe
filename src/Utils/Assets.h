#pragma once

#include <SFML/Graphics.hpp>

// Cинглтон Майерса
class Assets {
private:
	sf::Texture logo_texture_;
public:
	sf::Font font;
	sf::Sprite logo;
public:
	static Assets& Instance() {
		static Assets s;
		return s;
	}
	void Load();
private:
	Assets() {};
	~Assets() {};
	Assets(Assets const&) = delete;
	Assets& operator= (Assets const&) = delete;
};
