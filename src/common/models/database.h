#ifndef QUORIDOR_SRC_SERVER_SIDE_MODELS_DATABASE_H_
#define QUORIDOR_SRC_SERVER_SIDE_MODELS_DATABASE_H_

#include "../constants.h"
#include "Player.h"
#include "position.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sqlite3.h>
#include <cstdio>
#include <string>
#include <unordered_set>


class DataBase {
  sqlite3 *db_;
  std::string sql_;
  int last_sqlite3_exit_code_ = 0;
  char *messageError;
  static int friendsId;
  static int ranking_id_;
  static int board_id;

  std::mutex accessing_db_;

  DataBase() {}


public:
  virtual ~DataBase();
  static DataBase *GetInstance();

  void CreateTables();
  void InsertPlayer(const string &username, const string &password,
                    int64_t timestamp, uint32_t score);
  void InsertMessage(const int sender_id, const int receiver_id,
                     int64_t timestamp, const string content);
  void VerifyTable(const string &message);
  void InsertFriend(int user1_id, int user2_id);
  std::unordered_set<uint32_t> SearchFriends(object_id_t user_id);
  void InsertRanking(int firstPlaceId, int secondPlaceId, int thirdPlaceId,
                     int fourthPlaceId);
  void UpdateUser(uint32_t score, uint32_t id);
  void ReloadFile(std::string);
  std::vector<std::vector<string>> GetSelect(string statement);

  std::vector<object_id_t> GetAllGamesWhereUserIsAdmin(object_id_t user);
  std::vector<object_id_t> GetAllGamesForUser(object_id_t user);
  std::vector<object_id_t> GetAllParticipantsInGame(object_id_t game);
  object_id_t CreateGame(object_id_t current_user_id);
  void InviteUserToGame(object_id_t game_id, object_id_t userid);
  std::optional<uint32_t> GetAdminOfGame(object_id_t game_id);

  void RunSQL(const std::string &query);

  void InsertBoard(int nrOfPawns, std::string walls, std::string firstPlayerPawnPosition, int firstPlayerWallsLeft,
                             std::string secondPlayerPawnPosition, int secondPlayerWallsLeft,
                             std::string thirdPlayerPawnPosition, int thirdPlayerWallsLeft,
                             std::string fourthPlayerPawnPosition, int fourthPlayerWallsLeft, int lastPlayerToMove,
                             int gameId=board_id);

  void InsertBoard(int nrOfPawns, std::string walls, std::string firstPlayerPawnPosition, int firstPlayerWallsLeft,
                   std::string secondPlayerPawnPosition, int secondPlayerWallsLeft, int lastPlayerToMove, int gameId=board_id);

  void HandleSQLErr(int error_code);

};


#endif // QUORIDOR_SRC_SERVER_SIDE_MODELS_DATABASE_H_