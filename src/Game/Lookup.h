#pragma once

#include "Game.h"

#include <vector>

class Lookup {
protected:
  std::vector<std::vector<std::pair<int, int>>> winningStates;
  std::vector<std::pair<int, int>> getLegalMoves(
    std::vector<std::vector<StateCell>> board);
  bool isPositionOccupied(
    std::vector<std::vector<StateCell>> board, std::pair<int, int> pos);
  std::vector<std::pair<int, int>> getOccupiedPositions(
    std::vector<std::vector<StateCell>> board, StateCell marker);
  bool isBoardFull(std::vector<std::vector<StateCell>> board);
  bool isGameWon(std::vector<std::pair<int, int>> occupied_positions);
  StateCell getOpponentMarker(StateCell marker);
  int getBoardState(
    std::vector<std::vector<StateCell>> board, StateCell marker);
public:
  Lookup() {};
  void fillWinningStates(unsigned int count);
  std::pair<int, std::pair<int, int>> minimaxOptimization(
    std::vector<std::vector<StateCell>> board,
    StateCell marker, int depth, int alpha, int beta);
  bool isGameDone(std::vector<std::vector<StateCell>> board);
};
