//
// Created by Anton Romanova on 11/02/2022.
//

#ifndef QUORIDOR_SRC_SERVER_SIDE_MODELS_POSITION_H_
#define QUORIDOR_SRC_SERVER_SIDE_MODELS_POSITION_H_

struct Position {
  int col, row;

  bool operator==(const Position& pos){
    if(this->row==pos.row && this->col==pos.col) {
      return true;
    }else{
      return false;
    }
  }
  
};

#endif //QUORIDOR_SRC_SERVER_SIDE_MODELS_POSITION_H_
