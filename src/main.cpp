#include <iostream>
#include "engine/core/Game.hpp"

int main() 
{
  Game game;

  if (!game.initialize()) {
    std::cerr << "Erro ao inicializar jogo" << std::endl;
    return -1;
  }

  game.run();
  
  game.shutdown();
  return 0;
}
