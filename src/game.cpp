#include"game.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);



game_t *create_default_game(int rows, int columns) {
    game_t *newGame = new game_t(rows, columns);

    for(int i=0; i<rows; i++){
        newGame->board[i] = new char[columns+2];
        if(i == 0 || i == rows-1){
            for(int j=0; j<columns; j++){
                newGame->board[i][j] = '#';
            }
        } else {
            newGame->board[i][0] = '#';
            newGame->board[i][columns-1] = '#';
            for(int j=1; j<columns-1; j++){
                newGame->board[i][j] = ' ';
            }
        }
        newGame->board[i][columns] = '\n';
        newGame->board[i][columns+1] = '\0';
    }
    newGame->board[2][2]='d';
    newGame->board[2][3]='>';
    newGame->board[2][4]='D';
    newGame->board[2][9]='*';
    newGame->num_rows = rows;
    snake_t newSnake;
    newSnake.head_col = 4;
    newSnake.head_row = 2;
    newSnake.tail_col = 2;
    newSnake.tail_row = 2;
    newSnake.live = true;
    newGame->num_snakes = 1;
    newGame->snakes = new snake_t[newGame->num_snakes];
    newGame->snakes[0] = newSnake;
    return newGame;
} //测试基础类







game_t::game_t(int rows,int columns){
    this->board = new char*[rows];
    this->num_rows=rows;
    this->num_columns=columns;
    this->num_snakes=0;
}
game_t::game_t(){
    this->board = nullptr;
    this->num_rows=0;
    this->num_columns=0;
    this->num_snakes=0;
}
char game_t::get_board_at(unsigned int row, unsigned int col){
    return this->board[row][col];
}

game_t::~game_t() {
    if (this->snakes != nullptr) {
        delete[] this->snakes;
        this->snakes = nullptr;
    }
    if (this->board != nullptr) {
        for (unsigned int i = 0; i < this->num_rows; ++i) {
            if (this->board[i] != nullptr) {
                delete[] this->board[i];
                this->board[i] = nullptr;
            }
        }
        delete[] this->board;
        this->board = nullptr;
    }
}

void game_t::print_board(FILE *fp) {
for (unsigned int i = 0; i < (this->num_rows); i++) {
        fprintf(fp, "%s", this->board[i]);
    }
}

void game_t::save_board( char *filename) {
    FILE *f = fopen(filename, "w");
    print_board(f);
    fclose(f);
}

void game_t::set_board_at(unsigned int row, unsigned int col, char ch){
    this->board[row][col]=ch;
}

char game_t::next_square(unsigned int snum) {
    int col=(this->snakes+snum)->head_col;
    int row=(this->snakes+snum)->head_row;
    char c=this->board[row][col];
    switch (c) {
        case('A'):return this->board[row][col-1];
        case('S'):return this->board[row+1][col];
        case('W'):return this->board[row-1][col];
        case('D'):return this->board[row][col+1];
        default: return '?';
    }
    return '?';
}

void game_t::update_head(unsigned int snum) {
    int headRow=(this->snakes[snum]).head_row;
    int headCol=(this->snakes[snum]).head_col;
    switch (this->get_board_at(headRow,headCol))
    {
    case 'A':
        this->set_board_at(headRow,headCol-1,this->get_board_at(headRow,headCol));
        this->set_board_at(headRow,headCol,'<');
        (this->snakes+snum)->head_col-=1;
        break;
    case 'S':
        this->set_board_at(headRow+1,headCol,this->get_board_at(headRow,headCol));
        this->set_board_at(headRow,headCol,'v');
        (this->snakes+snum)->head_row+=1;
        break;
    case 'D':
        this->set_board_at(headRow,headCol+1,this->get_board_at(headRow,headCol));
        this->set_board_at(headRow,headCol,'>');
        (this->snakes+snum)->head_col+=1;
        break;
    case 'W':
        this->set_board_at(headRow-1,headCol,this->get_board_at(headRow,headCol));
        this->set_board_at(headRow,headCol,'^');
        (this->snakes+snum)->head_row-=1;
        break;
    default:
        break;
    }
    return;
}
void game_t::update_tail(unsigned int snum) {
    int tailRow=(this->snakes+snum)->tail_row;
    int tailCol=(this->snakes+snum)->tail_col;
    int nextRow=0;
    int nextCol=0;
    switch (this->get_board_at(tailRow,tailCol))
    {
    case 's':
        nextRow=tailRow+1;
        nextCol=tailCol;
        switch (this->get_board_at(nextRow,nextCol))
        {
        case 'v':
        this->set_board_at(nextRow,nextCol,'s');
        this->set_board_at(tailRow,tailCol,' ');
        break;
        case '>':
        this->set_board_at(nextRow,nextCol,'d');
        this->set_board_at(tailRow,tailCol,' ');
        break;
        case '<':
        this->set_board_at(nextRow,nextCol,'a');
        this->set_board_at(tailRow,tailCol,' ');
        break;
        
        default:
        break;
        }
        break;
    case 'a':
        nextRow=tailRow;
        nextCol=tailCol-1;
        switch (this->get_board_at(nextRow,nextCol))
        {
        case 'v':
        this->set_board_at(nextRow,nextCol,'s');
        this->set_board_at(tailRow,tailCol,' ');
        break;
        case '^':
        this->set_board_at(nextRow,nextCol,'w');
        this->set_board_at(tailRow,tailCol,' ');
        break;
        case '<':
        this->set_board_at(nextRow,nextCol,'a');
        this->set_board_at(tailRow,tailCol,' ');
        break;
        
        default:
        break;
        }
        break;
    case 'd':
        nextRow=tailRow;
        nextCol=tailCol+1;
        switch (this->get_board_at(nextRow,nextCol))
        {
        case 'v':
        this->set_board_at(nextRow,nextCol,'s');
        this->set_board_at(tailRow,tailCol,' ');
        break;
        case '>':
        this->set_board_at(nextRow,nextCol,'d');
        this->set_board_at(tailRow,tailCol,' ');
        break;
        case '^':
        this->set_board_at(nextRow,nextCol,'w');
        this->set_board_at(tailRow,tailCol,' ');
        break;
        
        default:
        break;
        }
        break;
    case 'w':
        nextRow=tailRow-1;
        nextCol=tailCol;
        switch (this->get_board_at(nextRow,nextCol))
        {
        case '^':
        this->set_board_at(nextRow,nextCol,'w');
        this->set_board_at(tailRow,tailCol,' ');
        break;
        case '>':
        this->set_board_at(nextRow,nextCol,'d');
        this->set_board_at(tailRow,tailCol,' ');
        break;
        case '<':
        this->set_board_at(nextRow,nextCol,'a');
        this->set_board_at(tailRow,tailCol,' ');
        break;
        
        default:
        break;
        }
        break;
    default:
        break;
    }
    (this->snakes+snum)->tail_col=nextCol;
    (this->snakes+snum)->tail_row=nextRow;
    
    return;
}

void game_t::update_game(int (*add_food)(game_t *game)) {
    for(unsigned int i=0;i<this->num_snakes;i++){
        int headRow=(this->snakes+i)->head_row;
        int headCol=(this->snakes+i)->head_col;
        int nextRow=get_next_row(headRow,this->get_board_at(headRow,headCol));
        int nextCol=get_next_col(headCol,this->get_board_at(headRow,headCol));
        if(this->get_board_at(nextRow,nextCol)=='#'||is_snake(this->get_board_at(nextRow,nextCol))){
            set_board_at(headRow,headCol,'x');
            (this->snakes+i)->live=false;
            continue;
        }else if(this->get_board_at(nextRow,nextCol)=='*'){
            update_head(i);
            add_food(this);

        }else{
            update_head(i);
            update_tail(i);
        }
    }
    return;
}

void game_t::find_head(unsigned int snum) {
  int nowRow=(this->snakes+snum)->tail_row;
  int nowCol=(this->snakes+snum)->tail_col;
  while(true){
    nowRow=get_next_row(nowRow,get_board_at(nowRow,nowCol));
    nowCol=get_next_col(nowCol,get_board_at(nowRow,nowCol));
    if(is_head(get_board_at(nowRow,nowCol))){
        (this->snakes+snum)->head_row=nowRow;
        (this->snakes+snum)->head_col=nowCol;
        (this->snakes+snum)->live=true;
        if(get_board_at(nowRow,nowCol)=='x'){
          (this->snakes+snum)->live=false;
        }
        break;
    }
  }

  return;
}
game_t *initialize_snakes(game_t *game) {
  int snum=0;
  snake_t *snakes=new snake_t[game->num_snakes];
  for(unsigned int i=0;i<game->num_rows;i++){
    for(unsigned int j=0;j<(game->num_columns);j++){
      if(is_tail(game->board[i][j])){
        snake_t newSnake={i,j,0,0,true};
        snakes[snum]=newSnake;
        game->snakes=snakes;
        game->num_snakes=snum+1;
        game->find_head(snum);
        
        snum++;
      }
    }
  }
  return game;
}


static bool is_tail(char c) {

  return (c=='s'||c=='a'||c=='d'||c=='w');
}


static bool is_head(char c) {
  return (c=='S'||c=='A'||c=='D'||c=='W'||c=='x');
}


static bool is_snake(char c) {
  const char* snk="wasd^<v>WASDx";
  for (int i=0;i<12;i++) {
    if (*(snk+i)==c) {
      return true;
    }
  }
  return false;
}


static char body_to_tail(char c) {
  if (is_snake(c)) {
    switch (c) {
      case('>'):return 'd';
      case('v'):return 's';
      case('<'):return 'a';
      case('^'):return 'w';
      default: return ' ';
    }
  }
  return ' ';
}


static char head_to_body(char c) {
  if (is_head(c)) {
    switch (c) {
      case('A'):return '<';
      case('S'):return 'v';
      case('D'):return '>';
      case('W'):return '^';
      default: return ' ';
    }
  }
  return ' ';

}

static unsigned int get_next_row(unsigned int cur_row, char c) {

  if (c=='v'||c=='s'||c=='S') {
    return cur_row+1;
  }
  if (c=='^'||c=='w'||c=='W') {
    return cur_row-1;
  }
  return cur_row;
}


static unsigned int get_next_col(unsigned int cur_col, char c) {
  if (c=='>'||c=='D'||c=='d') {
    return cur_col+1;
  }
  if (c=='<'||c=='a'||c=='A') {
    return cur_col-1;
  }
  return cur_col;
}

static char* read_line(FILE *fp) {
    const size_t BUF_INIT = 64;
    size_t buf_size = BUF_INIT;
    char* buf = new char[buf_size];
    char* pos = buf;
    int c;

    if (fp == nullptr) {
        delete[] buf;
        return nullptr;
    }

  
    while ((c = fgetc(fp)) != EOF && c != '\n') {
        if (pos - buf >= (int)buf_size - 1) { 
            buf_size *= 2;
            char* new_buf = new char[buf_size];
            memcpy(new_buf, buf, pos - buf);
            delete[] buf;
            buf = new_buf;
            pos = buf + (pos - buf);
        }
        *pos++ = (char)c;
    }

    if (pos == buf && c == EOF) {
        delete[] buf;
        return nullptr;
    }

    *pos++ = '\n';
    *pos = '\0';
    return buf;
}

game_t *load_board(FILE *fp) {
    if (fp == nullptr) {
        fprintf(stderr, "Error: Null file pointer in load_board\n");
        return nullptr;
    }

    std::vector<char*> lines; 
    unsigned int num_rows = 0;
    unsigned int max_cols = 0;
    char* line = nullptr;

    while ((line = read_line(fp)) != nullptr) {
        lines.push_back(line);
        num_rows++;

        size_t line_len = strlen(line);
        if (line_len > 0 && line[line_len - 1] == '\n') {
            line_len--;
        }
        if (line_len > max_cols) {
            max_cols = line_len;
        }
    }


    if (num_rows == 0) {
        fprintf(stderr, "Error: Empty file in load_board\n");
        for (char* l : lines) delete[] l;
        return nullptr;
    }


    game_t *newGame = new game_t(num_rows, max_cols);
    newGame->num_rows = num_rows;
    newGame->num_columns = max_cols;
    for (unsigned int i = 0; i < num_rows; i++) {
        char* src_line = lines[i];

        newGame->board[i] = new char[max_cols + 2];
        memset(newGame->board[i], ' ', max_cols + 2); 


        size_t src_len = strlen(src_line);
        if (src_len > 0 && src_line[src_len - 1] == '\n') {
            src_len--; 
        }
        for (unsigned int j = 0; j < max_cols; j++) {
            if (j < src_len) {
                newGame->board[i][j] = src_line[j];
            } else {
                newGame->board[i][j] = ' '; 
            }
        }
        newGame->board[i][max_cols] = '\n';    
        newGame->board[i][max_cols + 1] = '\0';
    }

    for (char* l : lines) {
        delete[] l;
    }

    return newGame;
}
