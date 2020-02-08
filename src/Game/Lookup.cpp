#include "Lookup.h"

#include "Game.h"
#include "Config.h"

// Заполнение выигрышных состояний
void Lookup::InitializeWinningStates(unsigned int count) {
  winning_states_.clear();
  // Строки и колонки
  for (int i = 0; i < count; i++) {
    std::vector<std::pair<int, int>> row;
    std::vector<std::pair<int, int>> column;
    for (int j = 0; j < count; j++) {
      row.push_back(std::make_pair(i, j));
      column.push_back(std::make_pair(j, i));
    }
    winning_states_.push_back(row);
    winning_states_.push_back(column);
  }

  // Диагонали
  std::vector<std::pair<int, int>> main_diagonal;
  std::vector<std::pair<int, int>> anti_diagonal;
  for (int i = 0; i < count; i++) {
    main_diagonal.push_back(std::make_pair(i, i));
    anti_diagonal.push_back(std::make_pair(count - i - 1, i));
  }
  winning_states_.push_back(main_diagonal);
  winning_states_.push_back(anti_diagonal);
}

// Получение всех свободных ячеек
std::vector<std::pair<int, int>> Lookup::GetLegalMoves(
    std::vector<std::vector<StateCell>>& board) {
  std::vector<std::pair<int, int>> legal_moves;
  for (int i = 0; i < board.size(); i++)
    for (int j = 0; j < board.size(); j++)
      if (board[i][j] != ai_marker && board[i][j] != player_marker)
        legal_moves.push_back(std::make_pair(i, j));

  return legal_moves;
}

// Получение всех ячеек, занятых крестиком или ноликом
std::vector<std::pair<int, int>> Lookup::GetOccupiedPositions(
    std::vector<std::vector<StateCell>>& board, StateCell marker) {
  std::vector<std::pair<int, int>> occupied_positions;

  for (int i = 0; i < board.size(); i++)
    for (int j = 0; j < board.size(); j++)
      if (marker == board[i][j])
        occupied_positions.push_back(std::make_pair(i, j));

  return occupied_positions;
}

// Проверка, что на доске нет больше мест
bool Lookup::IsBoardFull(std::vector<std::vector<StateCell>>& board) {
  std::vector<std::pair<int, int>> legal_moves = GetLegalMoves(board);

  return (0 == legal_moves.size());
}

// Проверка, что игра была выиграна
bool Lookup::IsGameWon(std::vector<std::vector<StateCell>>& board,
                       std::vector<std::pair<int, int>>& occupied_positions) {
  bool game_won;

  for (int i = 0; i < winning_states_.size(); i++) {
    game_won = true;
    std::vector<std::pair<int, int>> curr_win_state = winning_states_[i];
    for (int j = 0; j < board.size(); j++) {
      // Если ни одна выигрышная позиция не найдена, игра "не выиграна"
      auto iter = std::find(
        std::begin(occupied_positions),
        std::end(occupied_positions),
        curr_win_state[j]
      );
      if (iter == std::end(occupied_positions)) {
        game_won = false;
        break;
      }
    }

    if (game_won) break;
  }

  return game_won;
}

StateCell Lookup::GetOpponentMarker(StateCell marker) {
  StateCell opponent_marker;
  if (marker == player_marker)
    opponent_marker = ai_marker;
  else
    opponent_marker = player_marker;

  return opponent_marker;
}

// Проверка на выигрыш или проигрыш
int Lookup::GetBoardState(std::vector<std::vector<StateCell>>& board,
                          StateCell marker) {
  StateCell opponent_marker = GetOpponentMarker(marker);

  std::vector<std::pair<int, int>> occupied_positions =
    GetOccupiedPositions(board, marker);

  bool is_won = IsGameWon(board, occupied_positions);
  if (is_won) return kWin;

  occupied_positions = GetOccupiedPositions(board, opponent_marker);

  bool is_lost = IsGameWon(board, occupied_positions);
  if (is_lost) return kLoss;

  bool is_full = IsBoardFull(board);
  if (is_full) return kDraw;

  return kDraw;
}

// Минимакс алгоритм
std::pair<int, std::pair<int, int>> Lookup::MinimaxOptimization(
    std::vector<std::vector<StateCell>>& board, StateCell marker, int depth,
    int alpha, int beta) {
  // Инициализаиция лучшего хода
  std::pair<int, int> best_move = std::make_pair(-1, -1);
  int best_score = (marker == ai_marker) ? kLoss : kWin;

  // Если достигнут конец дерева, возврат лучшего результата и хода
  int board_state = GetBoardState(board, ai_marker);
  if (IsBoardFull(board) || kDraw != board_state || depth >= kMaxDepth)
    return std::make_pair(board_state, best_move);

  std::vector<std::pair<int, int>> legal_moves = GetLegalMoves(board);

  for (int i = 0; i < legal_moves.size(); i++) {
    std::pair<int, int> curr_move = legal_moves[i];
    board[curr_move.first][curr_move.second] = marker;

    // Максимизация стратегии игрока
    if (marker == ai_marker) {
      int score =
        MinimaxOptimization(board, player_marker, depth + 1, alpha, beta).first;

      if (best_score < score) {
        best_score = score - depth * 10;
        best_move = curr_move;

        // Если лучший ход этой ветви хуже, чем лучший предыдущей,
        // то откидываем её
        alpha = std::max(alpha, best_score);
        board[curr_move.first][curr_move.second] = StateCell::None;
        if (beta <= alpha) break;
      }

    } // Минимизация стратегии бота
    else {
      int score =
        MinimaxOptimization(board, ai_marker, depth + 1, alpha, beta).first;

      if (best_score > score) {
        best_score = score + depth * 10;
        best_move = curr_move;

        // Если лучший ход этой ветви хуже, чем лучший предыдущей,
        // то откидываем её
        beta = std::min(beta, best_score);
        board[curr_move.first][curr_move.second] = StateCell::None;
        if (beta <= alpha) break;
      }

    }

    board[curr_move.first][curr_move.second] = StateCell::None; // Отмена хода

  }

  return std::make_pair(best_score, best_move);
}

// Проверка, что игра завершена
bool Lookup::IsGameDone(std::vector<std::vector<StateCell>>& board) {
  if (IsBoardFull(board))
    return true;

  if (kDraw != GetBoardState(board, ai_marker))
    return true;

  return false;
}
