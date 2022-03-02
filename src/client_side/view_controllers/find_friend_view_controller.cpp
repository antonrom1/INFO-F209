//
// Created by Anton Romanova on 01/03/2022.
//

#include "find_friend_view_controller.h"
#include "../models/api_wrapper.h"
#include "../views/menu_views/find_new_friend_menu_view.h"
#include "../views/menu_views/views/label.h"

FindFriendViewController::FindFriendViewController()
    : AbstractSearchableListMenuViewController(
          std::make_shared<FindNewFriendMenuView>()) {}

void FindFriendViewController::FetchUsers() {
  auto user_fetch_result = ApiWrapper::GetAllUsers();

  if (std::holds_alternative<ApiError>(user_fetch_result)) {
    error_message_ = std::get<ApiError>(user_fetch_result).error_message;
    users_ = {};
  } else {
    error_message_ = {};
    users_ = std::move(std::get<std::vector<UserClient>>(user_fetch_result));
  }
}



void FindFriendViewController::AddFriend(const UserClient &user) {
  auto this_user_fetch_result =
      ApiWrapper::GetCurrentUserFromSharedApiWrapperInstance();
  auto error = ApiWrapper::GetShared()->AddFriend(user);
  if (error.has_value()) {
    username_of_last_added_friend_ = {};
    error_message_ = error->error_message;
    users_ = {};
  } else {
    username_of_last_added_friend_ = user.GetUsername().GetValue();
    error_message_ = {};
  }
  ReloadSubviews();
}

unsigned int FindFriendViewController::GetNumViewsToSearch() const {
  return users_.size();
}

std::shared_ptr<AbstractView>
FindFriendViewController::GetViewAtIndex(unsigned int i) const {
  return add_user_as_friend_buttons.at(i);
}

void FindFriendViewController::CreateViewsForUsers() {
  add_user_as_friend_buttons.clear();
  add_user_as_friend_buttons.reserve(users_.size());
  for (auto &user : users_) {
    add_user_as_friend_buttons.push_back(std::make_shared<Button>(
        GetMenuView().get(),
        "Add " + user.GetUsername().GetValue(),
        [&user, this]() {
          AddFriend(user);
        }
      )
    );
  }
}

void FindFriendViewController::MenuViewWillAppear() {
  FetchUsers();
  CreateViewsForUsers();
  AbstractSearchableListMenuViewController::MenuViewWillAppear();
}

std::optional<std::shared_ptr<AbstractView>>
FindFriendViewController::GetHeaderView() const {
  std::optional<std::string> header_label_message;

  if (error_message_.has_value())
    header_label_message = error_message_;
  else if (username_of_last_added_friend_.has_value())
    header_label_message = "New friend: " + *username_of_last_added_friend_;

  if (header_label_message.has_value())
    return std::make_shared<Label>(GetMenuView().get(), *header_label_message);
  else
    return {};
}
bool FindFriendViewController::DisplayList() const {
  return !error_message_.has_value();
}
