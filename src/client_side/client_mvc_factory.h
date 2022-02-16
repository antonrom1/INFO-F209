//
// Created by guest on 16/02/2022.
//
#include "abstract_controller.h"
#include <string>

#ifndef INFO_F209_SRC_CLIENT_SIDE_CLIENT_MVC_FACTORY_H_
#define INFO_F209_SRC_CLIENT_SIDE_CLIENT_MVC_FACTORY_H_

class ClientMVCFactory {
 public:
  enum SelectionableMenu{Main};
  AbstractController* getController(SelectionableMenu mvcname);
};

#endif //INFO_F209_SRC_CLIENT_SIDE_CLIENT_MVC_FACTORY_H_
