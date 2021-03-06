#ifndef QUORIDOR_SRC_SERVER_SIDE_MODELS_DATABASE_H_
#define QUORIDOR_SRC_SERVER_SIDE_MODELS_DATABASE_H_

#include "../constants.h"
#include "Player.h"
#include "position.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sqlite3.h>
#include <string>
#include <unordered_set>

class Board;
class Game;

class DataBase {
  sqlite3 *db_;
  std::string sql_;
  int last_sqlite3_exit_code_ = 0;
  char *messageError;
  static int friendsId;
  static int ranking_id_;

  std::mutex accessing_db_;

  DataBase() {}

public:
  virtual ~DataBase();
  static DataBase *GetInstance();

  void CreateTables();
  void InsertPlayer(const string &username, const string &password,
                    int64_t timestamp, uint32_t score);
  void InsertMessage(int sender_id, int receiver_id, int64_t timestamp,
                     const string& content);
  void VerifyTable(const string &message);
  void InsertFriend(int user1_id, int user2_id);
  std::unordered_set<uint32_t> SearchFriends(object_id_t user_id);
  void UpdateUser(uint32_t score, uint32_t id);
  std::vector<std::vector<string>> GetSelect(const string& statement);

  std::vector<std::tuple<uint32_t, std::string>>
  GetAllGamesWhereUserIsAdmin(object_id_t user);
  std::vector<std::tuple<uint32_t, std::string>>
  GetAllGamesForUser(object_id_t user);
  std::vector<object_id_t> GetAllParticipantsInGame(object_id_t game);
  uint32_t CreateGame(object_id_t admin_user_id, const std::string &game_name,
                      Game &game);
  void SaveGame(object_id_t game_id, Game game);
  std::optional<Game> GetGame(object_id_t game_id);
  void InviteUserToGame(object_id_t game_id, object_id_t userid);
  std::optional<uint32_t> GetAdminOfGame(object_id_t game_id);

  void RunSQL(const std::string &query);

  void HandleSQLErr(int error_code);
};

#endif // QUORIDOR_SRC_SERVER_SIDE_MODELS_DATABASE_H_
