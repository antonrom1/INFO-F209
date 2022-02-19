#include "main_menu.hpp"



std::vector <bool> MainMenu::getButtonState() const{
    return buttons;
}

void MainMenu::ReceiveMessage(std::string message){
    if (message == "259"){
        current_index-=1;
    }else if (message == "258"){
        current_index+=1;
    }
    buttons = {false, false, false, false, false};
    if(current_index<0)current_index=0;
    if(current_index>buttons.size()-1)current_index=buttons.size()-1;
    buttons[current_index]=true;
}
EnumFactory::SelectionableMenu MainMenu::ReceiveEnterMessage(){
if (current_index==4){
  return EnumFactory::Login;
}
  if (current_index==3){
    return EnumFactory::Help;
  }
  if (current_index==0){
    return EnumFactory::TypeOfGameSelect;
  }
return EnumFactory::null;
}

std::vector <bool> Login::getButtonState() const{
  return buttons;
}
void Login::SetUsername(std::string username){
    data[0] = username ;
}

void Login::setPassword(std::string password){
    //vérification
    data[1] = password ;
}
EnumFactory::SelectionableMenu Login::ReceiveEnterMessage(){
  if(current_index==0)
  return EnumFactory::Pseudo;
  if(current_index==1)
    return EnumFactory::MDP;
  if(current_index==2){
    if(data[0] != "" &&  data[1] != "")
    return EnumFactory::Main;

    return EnumFactory::null;

  }
}


std::vector<std::string> Login::getData() const {
    if(data[0] != "" &&  data[1] != ""){
        return data;
    }
}
void Login::ReceiveMessage(std::string message){
  if (message == "259"){
    current_index-=1;
  }else if (message == "258"){
    current_index+=1;
  }
  buttons = {false, false, false, false, false};
  if(current_index<0)current_index=0;
  if(current_index>buttons.size()-1)current_index=buttons.size()-1;
  buttons[current_index]=true;
}
EnumFactory::SelectionableMenu Help::ReceiveEnterMessage(){
    return EnumFactory::Main;
}

std::vector <bool> StartScreen::getButtonState() const{
  return buttons;
}
void StartScreen::ReceiveMessage(std::string message){
  if (message == "259"){
    current_index-=1;
  }else if (message == "258"){
    current_index+=1;
  }
  buttons = {false, false, false};
  if(current_index<0)current_index=0;
  if(current_index>buttons.size()-1)current_index=buttons.size()-1;
  buttons[current_index]=true;
}
EnumFactory::SelectionableMenu StartScreen::ReceiveEnterMessage(){
  if (current_index==0){
    return EnumFactory::Login;
  }
  if (current_index==2){
    return EnumFactory::Exit;
  }
  return EnumFactory::null;
}




std::vector <bool> TypeOfGameSelect::getButtonState() const{
  return buttons;
}
void TypeOfGameSelect::ReceiveMessage(std::string message){
  if (message == "259"){
    current_index-=1;
  }else if (message == "258"){
    current_index+=1;
  }
  buttons = {false, false, false};
  if(current_index<0)current_index=0;
  if(current_index>buttons.size()-1)current_index=buttons.size()-1;
  buttons[current_index]=true;
}
EnumFactory::SelectionableMenu TypeOfGameSelect::ReceiveEnterMessage(){
  if (current_index==2){
    return EnumFactory::Main;
  }
  return EnumFactory::null;
}