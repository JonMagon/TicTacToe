#include "Lookup.h"

#include "Game.h"

#include "Config.h"

#define AI_MARKER StateCell::O
#define PLAYER_MARKER StateCell::X

// Заполнение выигрышных состояний
void Lookup::fillWinningStates(unsigned int count) {
  // Строки и колонки
  for (int i = 0; i < count; i++) {
    std::vector<std::pair<int, int>> row;
    std::vector<std::pair<int, int>> column;
    for (int j = 0; j < count; j++) {
      row.push_back(std::make_pair(i, j));
      column.push_back(std::make_pair(j, i));
    }
    winningStates.push_back(row);
    winningStates.push_back(column);
  }

  // Диагонали
  std::vector<std::pair<int, int>> mainDiagonal;
  std::vector<std::pair<int, int>> antiDiagonal;
  for (int i = 0; i < count; i++) {
    mainDiagonal.push_back(std::make_pair(i, i));
    antiDiagonal.push_back(std::make_pair(count - i - 1, i));
  }
  winningStates.push_back(mainDiagonal);
  winningStates.push_back(antiDiagonal);
}

// Получение всех свободных ячеек
std::vector<std::pair<int, int>> Lookup::getLegalMoves
  (std::vector<std::vector<StateCell>> board) {

  std::vector<std::pair<int, int>> legalMoves;
  for (int i = 0; i < board.size(); i++)
    for (int j = 0; j < board.size(); j++)
      if (board[i][j] != AI_MARKER && board[i][j] != PLAYER_MARKER)
        legalMoves.push_back(std::make_pair(i, j));

  return legalMoves;
}

// Получение всех ячеек, занятых крестиком или ноликом
std::vector<std::pair<int, int>> Lookup::getOccupiedPositions
  (std::vector<std::vector<StateCell>> board, StateCell marker) {

  std::vector<std::pair<int, int>> occupiedPositions;

  for (int i = 0; i < board.size(); i++)
    for (int j = 0; j < board.size(); j++)
      if (marker == board[i][j])
        occupiedPositions.push_back(std::make_pair(i, j));

  return occupiedPositions;
}

// Проверка, что на доске нет больше мест
bool Lookup::isBoardFull(std::vector<std::vector<StateCell>> board) {
  std::vector<std::pair<int, int>> legalMoves = getLegalMoves(board);

  return (0 == legalMoves.size());
}

// Проверка, что игра была выиграна
bool Lookup::isGameWon(std::vector<std::pair<int, int>> occupiedPositions) {
  bool gameWon;

  for (int i = 0; i < winningStates.size(); i++) {
    gameWon = true;
    std::vector<std::pair<int, int>> currWinState = winningStates[i];
    for (int j = 0; j < 3; j++) {
      auto found = std::find(
        std::begin(occupiedPositions),
        std::end(occupiedPositions),
        currWinState[j]
      );
      if (found == std::end(occupiedPositions)) {
        gameWon = false;
        break;
      }
    }

    if (gameWon) break;
  }
  return gameWon;
}

StateCell Lookup::getOpponentMarker(StateCell marker) {
  StateCell opponentMarker;
  if (marker == PLAYER_MARKER)
    opponentMarker = AI_MARKER;
  else
    opponentMarker = PLAYER_MARKER;

  return opponentMarker;
}


// Проверка на выигрыш или проигрыш
int Lookup::getBoardState
  (std::vector<std::vector<StateCell>> board, StateCell marker) {

  StateCell opponentMarker = getOpponentMarker(marker);

  std::vector<std::pair<int, int>> occupiedPositions =
    getOccupiedPositions(board, marker);

  bool is_won = isGameWon(occupiedPositions);
  if (is_won) return WIN;

  occupiedPositions = getOccupiedPositions(board, opponentMarker);

  bool is_lost = isGameWon(occupiedPositions);
  if (is_lost) return LOSS;

  bool is_full = isBoardFull(board);
  if (is_full) return DRAW;

  return DRAW;
}

// Применение минимакс алгоритма
std::pair<int, std::pair<int, int>> Lookup::minimaxOptimization
  (std::vector<std::vector<StateCell>> board, StateCell marker, int depth,
  int alpha, int beta) {

  // Инициализаиция лучшего хода
  std::pair<int, int> bestMove = std::make_pair(-1, -1);
  int bestScore = (marker == AI_MARKER) ? LOSS : WIN;

  // Если достигнут конец дерева, возврат лучшего результата и хода
  if (isBoardFull(board) || DRAW != getBoardState(board, AI_MARKER)) {
    bestScore = getBoardState(board, AI_MARKER);
    return std::make_pair(bestScore, bestMove);
  }

  std::vector<std::pair<int, int>> legalMoves = getLegalMoves(board);

  for (int i = 0; i < legalMoves.size(); i++) {
    std::pair<int, int> currMove = legalMoves[i];
    board[currMove.first][currMove.second] = marker;

    // Максимизация стратегии игрока
    if (marker == AI_MARKER) {
      int score =
        minimaxOptimization(board, PLAYER_MARKER, depth + 1, alpha, beta).first;

      if (bestScore < score) {
        bestScore = score - depth * 10;
        bestMove = currMove;

        // Если лучший ход этой ветви хуже, чем лучший предыдущей,
        // то откидываем её
        alpha = std::max(alpha, bestScore);
        board[currMove.first][currMove.second] = StateCell::None;
        if (beta <= alpha) break;
      }

    } // Минимизация стратегии бота
    else {
      int score =
        minimaxOptimization(board, AI_MARKER, depth + 1, alpha, beta).first;

      if (bestScore > score) {
        bestScore = score + depth * 10;
        bestMove = currMove;

        // Если лучший ход этой ветви хуже, чем лучший предыдущей,
        // то откидываем её
        beta = std::min(beta, bestScore);
        board[currMove.first][currMove.second] = StateCell::None;
        if (beta <= alpha) break;
      }

    }

    board[currMove.first][currMove.second] = StateCell::None; // Отмена хода

  }

  return std::make_pair(bestScore, bestMove);
}

// Проверка, что игра завершена
bool Lookup::isGameDone(std::vector<std::vector<StateCell>> board) {
  if (isBoardFull(board))
    return true;

  if (DRAW != getBoardState(board, AI_MARKER))
    return true;

  return false;
}
