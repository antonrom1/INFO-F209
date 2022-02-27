#include "create_account_menu_view.h"
#include "../../view_controllers/home_menu_view_controller.h"
#include "views/label.h"
#include "views/text_field.h"
#include <memory>

CreateAccountMenuView::CreateAccountMenuView()
    : AbstractMenuView(menu_name_) {
  auto login_label = std::make_shared<Label>(this, "Insert your new pseudo:");
  auto password_label = std::make_shared<Label>(this, "Password:");
  auto confirm_password_label = std::make_shared<Label>(this, "Confirm your password:");

  auto login_field = std::make_shared<TextField>(this, "", this);
  auto password_field = std::make_shared<TextField>(this, "", this);
  auto confirm_password_field =  std::make_shared<TextField>(this, "", this);

  UpdateSubviews({
      login_label,
      login_field,
      password_label,
      password_field,
      confirm_password_label,
      confirm_password_field,

      std::make_shared<Label>(this, ""),

      std::make_shared<MenuButtonItem>(
          this, "Continue", std::make_shared<HomeMenuViewController>(), this),
      std::make_shared<MenuButtonItem>(
          this, "Quit",
          std::optional<std::shared_ptr<AbstractViewController>>{}, this),
  });

  login_field->SetPlaceholder("empty login");
  password_field->SetPlaceholder("empty password");
  confirm_password_field->SetPlaceholder("empty password");

  password_field->SetDisplayMode(TextField::PASSWORD);
  confirm_password_field->SetDisplayMode(TextField::PASSWORD);

  login_label->SetDisplayMode(Label::UNDERLINE);
  password_label->SetDisplayMode(Label::UNDERLINE);
  confirm_password_label->SetDisplayMode(Label::UNDERLINE);

}

void CreateAccountMenuView::TextChanged(TextField &sender,
                                const std::string &old_text) {}

void CreateAccountMenuView::TextEditingFinished(TextField &sender) {}