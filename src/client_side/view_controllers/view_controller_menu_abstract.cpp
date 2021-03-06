//
// Created by Anton Romanova on 21/02/2022.
//

#include "view_controller_menu_abstract.h"

AbstractMenuViewController::AbstractMenuViewController(
    const std::shared_ptr<AbstractMenuView> &view)
    : AbstractViewController(), MenuViewDelegate(), menu_view_(view) {
  menu_view_->SetDelegate(this);
}
std::optional<std::shared_ptr<AbstractViewController>>
AbstractMenuViewController::Tick() {
  WINDOW *window;
  initscr();
  attron(A_STANDOUT);
  clear();
  noecho();

  MenuViewWillAppear();
  window = newwin(150, 150, 0, 0);
  curs_set(0);

  while (!next_view_controller_.has_value()) {
    werase(window);
    Draw(window);
    keypad(window, true);
    wtimeout(window, 1000);
    RespondToKeyboardEvent(wgetch(window));
  }
  endwin();
  auto next_vc = std::move(next_view_controller_.value());

  next_view_controller_ = {};

#undef GET_TIME_IN_MILLIS

  MenuViewWillDisappear();
  return next_vc;
}

void AbstractMenuViewController::PresentViewController(
    std::optional<std::shared_ptr<AbstractViewController>>
        next_view_controller) {
  next_view_controller_ = next_view_controller;
}

const std::shared_ptr<AbstractMenuView> &
AbstractMenuViewController::GetMenuView() const {
  return menu_view_;
}

void AbstractMenuViewController::RespondToKeyboardEvent(const int &character) {
  menu_view_->PropagateEvent(character);
}

void AbstractMenuViewController::Draw(WINDOW *window) {
  GetMenuView()->Draw(window);
}
