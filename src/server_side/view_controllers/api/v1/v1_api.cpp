//
// Created by Anton Romanova on 15/02/2022.
//

#include "v1_api.h"
//void V1Api::SetupRoutes() {
//  BaseQuoridorApi::SetupRoutes();
//  CROW_ROUTE(GetApp(), "/api/")( [] () {
//    // TODO: Add a guide here
//    return "TODO: Add a guide";
//  });
//
//
//
//}
//void V1Api::SetupRoutes() {
//  BaseQuoridorApi::SetupRoutes();
//
//}


template<class... Middlewares>
unsigned int V1Api<Middlewares...>::GetVersion() const {
  return version_;
}