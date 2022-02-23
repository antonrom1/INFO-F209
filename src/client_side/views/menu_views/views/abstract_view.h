//
// Created by Anton Romanova on 22/02/2022.
//

#ifndef QUORIDOR_SRC_CLIENT_SIDE_VIEWS_MENU_VIEWS_VIEWS_ABSTRACT_VIEW_H_
#define QUORIDOR_SRC_CLIENT_SIDE_VIEWS_MENU_VIEWS_VIEWS_ABSTRACT_VIEW_H_

#include <string>
#include <ncurses.h>
#include "first_responder.h"
#include "../../../../common/constants.h"

class AbstractView : public EventResponder {
 public:
  AbstractView(const std::optional<EventResponder *> &parent, const std::string &inner_text);
  enum State { NORMAL, HOVER, SELECTED };

  [[nodiscard]] const std::string &GetInnerText() const;
  [[nodiscard]] State GetState() const;
  void SetState(State new_state);
  void UnselectIfPossible();
  [[nodiscard]] unsigned int GetNcursesEffectForCurrentState() const;

 protected:
  std::unique_ptr<std::vector<std::shared_ptr<EventResponder>>> GetChildren() override;
  virtual void SetInnerText(const std::string &inner_text);
  virtual void HasBeenSelected();
  virtual void HasBeenUnselected();

 private:
  std::string inner_text_;
  State state_ = NORMAL;
};

#endif //QUORIDOR_SRC_CLIENT_SIDE_VIEWS_MENU_VIEWS_VIEWS_ABSTRACT_VIEW_H_
