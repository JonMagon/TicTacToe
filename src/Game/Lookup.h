#pragma once

#include "Game.h"

#include <vector>

class Lookup {
protected:
  std::vector<std::vector<std::pair<int, int>>> winning_states_;
  std::vector<std::pair<int, int>> GetLegalMoves(
    std::vector<std::vector<StateCell>>& board);
  std::vector<std::pair<int, int>> GetOccupiedPositions(
    std::vector<std::vector<StateCell>>& board, StateCell marker);
  bool IsBoardFull(std::vector<std::vector<StateCell>>& board);
  bool IsGameWon(std::vector<std::vector<StateCell>>&,
                 std::vector<std::pair<int, int>>& occupied_positions);
  StateCell GetOpponentMarker(StateCell marker);
  int GetBoardState(std::vector<std::vector<StateCell>>& board,
                    StateCell marker);
public:
  Lookup() {};
  StateCell player_marker = StateCell::X, ai_marker = StateCell::O;
  void InitializeWinningStates(unsigned int count);
  bool IsGameDone(std::vector<std::vector<StateCell>>& board);
  std::pair<int, std::pair<int, int>> MinimaxOptimization(
    std::vector<std::vector<StateCell>>& board, StateCell marker, int depth,
    int alpha, int beta);
};
