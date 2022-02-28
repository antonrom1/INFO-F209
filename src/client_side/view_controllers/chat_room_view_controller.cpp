//
// Created by guest on 27/02/2022.
//

#include "chat_room_view_controller.h"
#include "../views/menu_views/chat_room_menu_view.h"
#include "../views/menu_views/views/label.h"

ChatRoomViewController :: ChatRoomViewController() : MenuViewController(std::make_shared<ChatRoomMenuView>()) {
  text_field_ = std::make_shared<TextField>(GetMenuView().get(), "", this);

  messages_ = {
      std::make_shared<Label>(GetMenuView().get(), "msg1"),
      std::make_shared<Label>(GetMenuView().get(), "msg2"),
      std::make_shared<Label>(GetMenuView().get(), "msg3")
  };

  text_field_->SetPlaceholder("Text:");

  UpdateSubviews();

}

void ChatRoomViewController::TextChanged(TextField &sender,
                                         const std::string &old_text) {

}
void ChatRoomViewController::TextEditingFinished(TextField &sender) {
  messages_[2]->SetInnerText(messages_[1]->GetInnerText());
  messages_[1]->SetInnerText(messages_[0]->GetInnerText());
  messages_[0]->SetInnerText(sender.GetUserEnteredText());
  text_field_->SetInnerText("");
  UpdateSubviews();
}
