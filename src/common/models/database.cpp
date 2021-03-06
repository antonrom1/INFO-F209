#include "database.h"
#include "../../common/models/Game.h"
#include "board.h"

int DataBase::friendsId = 0;
int DataBase::ranking_id_ = 0;

#define LOCK_DB std::lock_guard<std::mutex> __lock(accessing_db_)

static int callback(void *data, int argc, char **argv, char **azColName) {
  fprintf(stderr, "%s: ", (const char *)data);

  for (int i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }

  printf("\n");
  return 0;
}

static int select_callback(void *data, int argc, char **argv, char **colName) {

  auto *res = static_cast<std::vector<std::vector<string>> *>(data);

  res->emplace_back(argv, argv + argc);

  return 0;
}

std::vector<std::vector<string>>
DataBase::GetSelect(const std::string &statement) {
  LOCK_DB;
  std::vector<std::vector<string>> res;

  last_sqlite3_exit_code_ =
      sqlite3_exec(db_, statement.c_str(), select_callback, &res, nullptr);

  HandleSQLErr(last_sqlite3_exit_code_);

  return res;
}

void DataBase::CreateTables() {
  // Create first table USER
  LOCK_DB;
  sql_ = "CREATE TABLE IF NOT EXISTS USER(    "
         "ID             INTEGER PRIMARY KEY, "
         "PSEUDO         TEXT    NOT NULL,    "
         "PASSWORD       TEXT    NOT NULL,    "
         "TIMESTAMP      TEXT    NOT NULL,    "
         "SCORE          INT     NOT NULL)    ";

  //  ReloadFile("example.db");

  last_sqlite3_exit_code_ = sqlite3_open(DATABASE_FILE_NAME, &db_);
  last_sqlite3_exit_code_ =
      sqlite3_exec(db_, sql_.c_str(), nullptr, nullptr, &messageError);
  // Create second table BOARD
  //  sql_ = "CREATE TABLE IF NOT EXISTS BOARD("
  //         "ID                      INTEGER   PRIMARY KEY , "
  //         "PAWNS                   INT       NOT NULL    , "
  //         "WALLS                   TEXT      NOT NULL    , "
  //         "PLAYER1_PAWN            TEXT      NOT NULL    , "
  //         "PLAYER1_WALLS_LEFT      INT       NOT NULL    , "
  //         "PLAYER2_PAWN            TEXT      NOT NULL    , "
  //         "PLAYER2_WALLS_LEFT      INT       NOT NULL    , "
  //         "PLAYER3_PAWN            TEXT      NOT NULL    , "
  //         "PLAYER3_WALLS_LEFT      INT       NOT NULL    , "
  //         "PLAYER4_PAWN            TEXT      NOT NULL    , "
  //         "PLAYER4_WALLS_LEFT      INT       NOT NULL    , "
  //         "LAST_TO_MOVE            INT       NOT NULL    ) ";

  last_sqlite3_exit_code_ =
      sqlite3_exec(db_, sql_.c_str(), nullptr, nullptr, &messageError);

  // Create second table GAMES
  sql_ = "create table GAMES"
         "(ID        INTEGER primary key,"
         "ROOM_NAME TEXT not null,"
         "ADMIN_ID  INT  not null constraint GAMES_USER__fk references USER "
         "(\"\") on delete cascade,"
         "GAME_JSON TEXT not null);";

  last_sqlite3_exit_code_ =
      sqlite3_exec(db_, sql_.c_str(), nullptr, nullptr, &messageError);

  // Create fourth table FRIENDS
  sql_ = "create table FRIENDS"
         "("
         " ID               INTEGER primary key,"
         " MY_USER_ID       INT not null"
         "          constraint FRIENDS_USER__fk references USER (\"\")"
         "          on delete cascade,"
         " MY_FRIEND_ID     INT not null"
         "          constraint FRIENDS_USER__fk_2 references USER (\"\")"
         "          on delete cascade,"
         "PORT              INT not null"
         ");";
  last_sqlite3_exit_code_ =
      sqlite3_exec(db_, sql_.c_str(), nullptr, nullptr, &messageError);

  // Create fifth table CONVERSATIONS
  sql_ = "create table CONVERSATIONS"
         "(ID          INTEGER primary key,"
         "SENDER_ID   INT  not null constraint CONVERSATIONS_USER__fk "
         "references USER (\"\") on delete cascade,"
         "RECEIVER_ID INT  not null constraint CONVERSATIONS_USER__fk_2 "
         "references USER (\"\") on delete cascade,"
         "TIMESTAMP   INT  not null,"
         "CONTENT     TEXT not null);";

  last_sqlite3_exit_code_ =
      sqlite3_exec(db_, sql_.c_str(), nullptr, nullptr, &messageError);

  // Create fifth table GAME_PARTICIPANTS
  sql_ = "create table GAME_PARTICIPANTS"
         "(ID      INTEGER primary key,"
         "GAME_ID INT not null constraint GAME_PARTICIPANTS_GAMES__fk"
         "    references GAMES (\"\") on delete cascade,"
         "USER_ID INT not null constraint GAME_PARTICIPANTS_USER__fk"
         "    references USER (\"\") on delete cascade"
         ");";

  last_sqlite3_exit_code_ =
      sqlite3_exec(db_, sql_.c_str(), nullptr, nullptr, &messageError);

  // Verify table
  VerifyTable("Creating tables");

  HandleSQLErr(last_sqlite3_exit_code_);
}

void DataBase::InsertMessage(const int sender_id, const int receiver_id,
                             int64_t timestamp, const std::string &content) {
  LOCK_DB;
  sql_ = "INSERT INTO CONVERSATIONS (SENDER_ID, RECEIVER_ID, TIMESTAMP, "
         "CONTENT) VALUES(" +
         to_string(sender_id) + "," + to_string(receiver_id) + "," +
         to_string(timestamp) + ",\"" + content + "\");";

  last_sqlite3_exit_code_ =
      sqlite3_exec(db_, sql_.c_str(), callback, nullptr, &messageError);

  // Verify creation of table
  VerifyTable("Insert message into conversation");

  HandleSQLErr(last_sqlite3_exit_code_);
}

void DataBase::InsertPlayer(const string &username, const string &password,
                            int64_t timestamp, uint32_t score) {
  LOCK_DB;
  string pseudo; // TO be completed later by the server.

  // Insert in the table
  sql_ = "INSERT INTO USER (PSEUDO, PASSWORD, TIMESTAMP, SCORE) VALUES(\"" +
         username + "\",\"" + password + "\"," + to_string(timestamp) + "," +
         to_string(score) + ");";
  last_sqlite3_exit_code_ =
      sqlite3_exec(db_, sql_.c_str(), callback, nullptr, &messageError);

  // Verify creation of table
  VerifyTable("Insert values into player");

  HandleSQLErr(last_sqlite3_exit_code_);
}

void DataBase::InsertFriend(int user1_id, int user2_id) {
  LOCK_DB;

  string query = "SELECT * FROM FRIENDS;";
  sqlite3_exec(db_, query.c_str(), callback, nullptr, nullptr);
  // Insert the friend-me and me-friend relation into the database
  sql_ = ("INSERT INTO FRIENDS VALUES(" + to_string(friendsId) + "," +
          to_string(user1_id) + "," + to_string(user2_id) + ",0);");
  friendsId++;
  last_sqlite3_exit_code_ =
      sqlite3_exec(db_, sql_.c_str(), nullptr, nullptr, &messageError);

  VerifyTable("Insert values into friend");
}

std::unordered_set<uint32_t> DataBase::SearchFriends(object_id_t user_id) {

  std::string current_user_id_str = std::to_string(user_id);

  string data("CALLBACK FUNCTION");
  string query = "SELECT MY_USER_ID, MY_FRIEND_ID FROM FRIENDS WHERE "
                 "FRIENDS.MY_FRIEND_ID=" +
                 current_user_id_str +
                 " OR FRIENDS.MY_USER_ID=" + current_user_id_str + ";";

  auto query_res = GetSelect(query);
  std::unordered_set<object_id_t> friends = {};
  friends.reserve(query_res.size());
  for (const std::vector<std::string> &friend_string_repr : query_res) {
    std::string other_user_id_str =
        friend_string_repr[friend_string_repr[0] == current_user_id_str ? 1
                                                                        : 0];

    friends.insert(std::stoul(other_user_id_str));
  }

  HandleSQLErr(last_sqlite3_exit_code_);

  return friends;
}

void DataBase::UpdateUser(uint32_t score, uint32_t id) {
  LOCK_DB;

  // Update Table
  string query = "UPDATE USER SET SCORE=" + to_string(score) +
                 " WHERE ID=" + to_string(id) + ";";
  last_sqlite3_exit_code_ =
      sqlite3_exec(db_, query.c_str(), callback, nullptr, nullptr);

  HandleSQLErr(last_sqlite3_exit_code_);
}

void DataBase::VerifyTable(const string &message) {
  HandleSQLErr(last_sqlite3_exit_code_);
}

DataBase *DataBase::GetInstance() {
  static bool initialized = false;
  static DataBase db;
  if (!initialized) {

    db.CreateTables();
    sqlite3_extended_result_codes(db.db_, true);
    initialized = true;
  }

  return &db;
}

DataBase::~DataBase() { sqlite3_close(db_); }

void DataBase::HandleSQLErr(int error_code) {
  if (error_code != SQLITE_OK) {
    auto extended_error_code = sqlite3_extended_errcode(db_);
    if (extended_error_code == SQLITE_CANTOPEN) {
      sqlite3_close(db_);
      last_sqlite3_exit_code_ = sqlite3_open(DATABASE_FILE_NAME, &db_);
    }
    std::cout << "Database error: " << sqlite3_errmsg(db_) << std::endl;
  }
}

uint32_t DataBase::CreateGame(object_id_t admin_user_id,
                              const std::string &game_name, Game &game) {
  accessing_db_.lock();
  std::string sql_query =
      "INSERT INTO GAMES (ROOM_NAME, ADMIN_ID, GAME_JSON) VALUES('" +
      game_name + "' , " + std::to_string(admin_user_id) + ", '" +
      game.GetGameJson().dump() + "')";

  last_sqlite3_exit_code_ =
      sqlite3_exec(db_, sql_query.c_str(), nullptr, nullptr, &messageError);

  accessing_db_.unlock();

  auto game_id = stoul(
      GetSelect("SELECT ID FROM GAMES ORDER BY ID DESC LIMIT 1").at(0).at(0));

  for (const auto &player : game.GetPlayers()) {
    if (player->GetUserId().has_value())
      InviteUserToGame(game_id, *player->GetUserId());
  }
  return game_id;
}

void DataBase::RunSQL(const string &query) {
  HandleSQLErr(
      sqlite3_exec(db_, query.c_str(), nullptr, nullptr, &messageError));
}

std::vector<std::tuple<uint32_t, std::string>>
DataBase::GetAllGamesWhereUserIsAdmin(object_id_t user) {
  std::vector<std::tuple<object_id_t, std::string>> output;
  auto res = GetSelect("SELECT ID, ROOM_NAME FROM GAMES WHERE ADMIN_ID = " +
                       std::to_string(user));
  output.reserve(res.size());
  for (auto &row : res) {
    output.emplace_back(stoul(row.at(0)), row.at(1));
  }
  return output;
}

void DataBase::InviteUserToGame(object_id_t game_id, object_id_t userid) {

  //  if (GetAdminOfGame(game_id) == userid) {
  //    return;
  //  }

  auto existing_invitation_for_this_game = GetSelect(
      "SELECT GAME_ID FROM GAME_PARTICIPANTS WHERE GAME_ID=" +
      std::to_string(game_id) + " AND USER_ID=" + std::to_string(userid));

  if (!existing_invitation_for_this_game.empty()) {
    return;
  }

  LOCK_DB;
  RunSQL("INSERT INTO GAME_PARTICIPANTS (GAME_ID, USER_ID) VALUES ( " +
         std::to_string(game_id) + ", " + std::to_string(userid) + " )");
}

std::vector<std::tuple<uint32_t, std::string>>
DataBase::GetAllGamesForUser(object_id_t user) {
  std::vector<std::tuple<object_id_t, std::string>> output;
  auto res = GetSelect("SELECT ID, ROOM_NAME FROM GAMES WHERE ID IN (SELECT "
                       "GAME_ID FROM GAME_PARTICIPANTS WHERE USER_ID = " +
                       std::to_string(user) + " )");
  output.reserve(res.size());
  for (auto &row : res) {
    output.emplace_back(stoul(row.at(0)), row.at(1));
  }
  return output;
}

std::vector<object_id_t> DataBase::GetAllParticipantsInGame(object_id_t game) {
  auto res =
      GetSelect("SELECT USER_ID FROM GAME_PARTICIPANTS WHERE GAME_ID = " +
                std::to_string(game));

  std::vector<object_id_t> output;

  if (!output.empty()) {
    output.push_back(*GetAdminOfGame(game));
  }

  for (auto &row : res) {
    output.push_back(std::stoul(row.at(0)));
  }

  return output;
}

std::optional<object_id_t> DataBase::GetAdminOfGame(object_id_t game_id) {
  auto q_res = GetSelect("SELECT ADMIN_ID FROM GAMES WHERE ID = " +
                         std::to_string(game_id));
  if (q_res.empty()) {
    return {};
  } else {
    return std::stoul(q_res[0].at(0));
  }
}

void DataBase::SaveGame(uint32_t game_id, Game game) {
  GetSelect("UPDATE GAMES\n"
            "SET GAME_JSON = '" +
            game.GetGameJson().dump() +
            "' WHERE ID = " + std::to_string(game_id) + ";");
}
std::optional<Game> DataBase::GetGame(uint32_t game_id) {
  auto q_res = GetSelect("SELECT GAME_JSON FROM GAMES WHERE ID = " +
                         std::to_string(game_id));
  if (q_res.empty()) {
    return {};
  }
  return Game::InitGameFromJson(crow::json::load(q_res[0][0]));
}
