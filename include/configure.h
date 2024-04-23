#ifndef __CONFIGURE_H__
#define __CONFIGURE_H__

//fxns linked to the options menu
//soundfx/volume
//config files for size of grid
//change ctrls?
//high scores?
//eventually, perhaps read char by char instead of line by line
//and regroup the main configs with {}, and dont care about whitespace?

typedef struct high_scores{
    int easy;
    int medium;
    int hard;
} High_Scores;

enum setting{
    high_scores_settings, colors_settings, none
};

enum state{
    locked, unlocked, equipped, error
};

struct player_colors{
    enum state blue;
    enum state green;
    enum state red;
    enum state yellow;
};

struct Player_unlocks{
    High_Scores high_scores;
    struct player_colors colors;
};

int is_num(const char *string);
void save_player_data(struct Player_unlocks *data);
int get_score_from_file(const char *score, int start);
void read_file(FILE *file, High_Scores *score, struct player_colors *colors);
void read_scores(const char *line, int line_num, High_Scores *score);
enum state get_color_from_file(const char *color, int start);
void read_colors(const char *line, int line_num, struct player_colors *colors);
void file_error(int score, int line_num);
void print_scores(High_Scores *scores);
void print_colors(struct player_colors *colors);

#ifdef _WIN32
#include <fileapi.h>
struct Player_unlocks check_config_file();
#endif

#ifdef __linux__
#include <sys/stat.h>
#include <fcntl.h>
High_Scores make_config_folder();
#endif

#endif