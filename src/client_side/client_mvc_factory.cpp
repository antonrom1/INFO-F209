#include <string>
#include "client_mvc_factory.h"
#include "client_main_controller.h"
#include "client_login_controller.h"
#include "abstract_controller.h"


AbstractController* ClientMVCFactory::getController(SelectionableMenu mvcname){
  switch(mvcname){
    case Main:
      return new ControllerMainMenu;
      break;
    case Login:
      return new ControllerLoginClient;
      break;
  }
}