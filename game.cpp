#include<game.hpp>


game_t *create_default_game(int rows,int columns) {

    game_t *newGame=new game_t(rows,columns);

    for(int i=0;i<rows;i++){
        newGame->board[i]=new char[columns+2];
        if(i==0||i==rows-1){
        for(int j=0;j<columns;j++){
            newGame->board[i][j]='#';
        }
        }else{
        newGame->board[i][0]='#';
        newGame->board[i][columns-1]='#';
        for(int j=1;j<columns-1;j++){
            newGame->board[i][j]=' ';
        }
        }
        newGame->board[i][columns]='\n';
        newGame->board[i][columns+1]='\0';
    }
    newGame->num_rows=rows;
    snake_t newSnake;
    newSnake.head_col=4;
    newSnake.head_row=2;
    newSnake.tail_col=2;
    newSnake.tail_row=2;
    newSnake.live=true;
    newGame->num_snakes=1;
    newGame->snakes=new snake_t[newGame->num_snakes];
    newGame->snakes[0]=newSnake;
    return newGame;
} //测试基础类


class game_t{
public:
  unsigned int num_rows;
  unsigned int num_columns;
  char **board;

  unsigned int num_snakes;
  snake_t *snakes;
  game_t(int rows,int columns){
    char **board=new char*[rows];
    this->num_rows=rows;
    this->num_columns=columns;
    this->num_snakes=0;
    
  }
  char get_board_at(game_t *game, unsigned int row, unsigned int col){

  }

  ~game_t() {
  free(this->snakes);
  free(this->board);
  free(this);
  return;
    }
    void print_board( FILE *fp) {
    for (unsigned int i = 0; i < this->num_rows; i++) {
        fprintf(fp, "%s", this->board[i]);
    }
    }
    void save_board( char *filename) {
    FILE *f = fopen(filename, "w");
    print_board(f);
    fclose(f);
    }
};