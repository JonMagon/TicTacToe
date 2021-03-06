#pragma once

// Размеры окна
const int kWindowWidth = 1000;
const int kWindowHeight = 600;

const int kDefaultFieldSize = 3; // Размер игрового поля в квадратах
const int kFieldSizePx = 500;	// Размер игрового поля в пикселях
const int kLineWidth = 2; // Толщина линий игрового поля
const int kMarkerWidth = 5; // Толщина крестиков и ноликов

const int kIndentFieldX = 50; // Отступ перед игровым полем по X в пикселях
const int kIndentFieldY = 50; // Отступ перед игровым полем по Y в пикселях

// Ресурсы игры
const std::string kFontPath = "resources/calibri.ttf";
const std::string kLogoPath = "resources/logo.png";
const int kDefaultFontSize = 17; // Размер шрифта

// Стандартные размеры элементов
const int kButtonDefaultWidth = 100;
const int kButtonDefaultHeight = 30;
const int kCheckboxDefaultWidth = 20;
const int kCheckboxDefaultHeight = 20;

const int kMaxDepth = 4; // Максимальная глубина минимакса
const int kWin = 1000; // Значение победы
const int kDraw = 0; // Значение ничьи
const int kLoss = -1000; // Значение поражения
