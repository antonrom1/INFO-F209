
#include <vector>
#include "home_menu_view.h"
#include "../../view_controllers/help_view_controller.h"
#include "../../view_controllers/ranking_view_controller.h"
#include "../../view_controllers/selection_menu_view_controller.h"
#include "../../view_controllers/chat_room_view_controller.h"

HomeMenuView::HomeMenuView() : AbstractMenuView(menu_name_) {
  UpdateSubviews({
    std::make_shared<MenuButtonItem>(this, "Play", std::make_shared<SelectionMenuViewController>(), this),
    std::make_shared<MenuButtonItem>(this, "Friend List", std::optional<std::shared_ptr<AbstractViewController>>{}, this),
    std::make_shared<MenuButtonItem>(this, "Chat Room", std::make_shared<ChatRoomViewController>(), this),
    std::make_shared<MenuButtonItem>(this, "Ranking",std::make_shared<RankingMenuViewController>(), this),
    std::make_shared<MenuButtonItem>(this, "Help", std::make_shared<HelpViewController>(), this),
    std::make_shared<MenuButtonItem>(this, "Logout", std::optional<std::shared_ptr<AbstractViewController>>{}, this),
  });
}
