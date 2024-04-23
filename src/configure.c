#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//#include <options.h>
#include <string.h>
#include <configure.h>


int is_num(const char *string){
	for (int i = 0; i < strlen(string); i++){
		//WON'T ACCEPT NEGATIVE NUMS
		if (isdigit(string[i])){
			return 1;
		}
	}
	return 0;
}

void save_player_data(struct Player_unlocks *data){
	//rewrite all the data (i.e. overwrite the .settings file)
	const char *filename = ".config/.settings";
	FILE *file = fopen(filename, "w");
    High_Scores scores = data->high_scores;
	if (!fprintf(file, "high-scores:\n\t-easy: %d\n\t-medium: %d\n\t-hard: %d\n", 
		scores.easy, scores.medium, scores.hard
	)){
		puts("An error occured trying to write high scores in the file.");
	}
    if (!fprintf(file, "\ncolors:\n\t-blue: %d\n\t-green: %d\n\t-yellow: %d\n\t-red: %d\n",
        data->colors[0], data->colors[1], data->colors[2], data->colors[3]
    )){
		puts("An error occured trying to write colors in the file.");
	}

}

int get_score_from_file(const char *score, int start){
	//check if score is actually defined, i.e. is it 
	if (start >= strlen(score)){
		puts("There is no score written!");
		return 2;
	}
	#define size_foo 30
	char foo[size_foo];
	if (strlen(score) >= size_foo){
		puts("Your file is corrupted!");
		return 3;
	}
	//do we need to skip the newline ('\n') in score?
	//printf("Sizeof score for dbg: %ld", strlen(score));
	strncpy(foo, &(score[start]), strlen(score) - 1);
	return (!is_num(foo)) ? -1 : atoi(foo);
}

void read_file(FILE *file, High_Scores *scores, enum state colors[], int size){
	char line[40];
	enum setting i = none;
    int j = 1;
	while (fgets(line, sizeof(line), file) != NULL) {
        //add comments using #?
		if (!strcmp(line, "high-scores:\n") && i == none){
			//puts("Good syntax");
			i = high_scores_settings;
		}
        else if (!strcmp(line, "\n")){
            i = none;
        }
        else if (!strcmp(line, "colors:\n") && i == none){
            i = colors_settings;
        }
        else if (i == colors_settings){
            read_colors(line, j, colors);
        }
		else if (i == high_scores_settings){
			read_scores(line, j, scores);
		}
        else{
            puts("Syntax error!");
            exit(1);
        }
        j++;
	}
}

void read_scores(const char *line, int line_num, High_Scores *score){
    if (!strncmp(line, "\t-easy: ", 8)){
        int score_from_file = get_score_from_file(line, 8);
        file_error(score_from_file, line_num);
        score->easy = score_from_file;
    }
    else if (!strncmp(line, "\t-medium: ", 9)){
        int score_from_file = get_score_from_file(line, 9);
        file_error(score_from_file, line_num);
        score->medium = score_from_file;
    }
    else if (!strncmp(line, "\t-hard: ", 8)){
        int score_from_file = get_score_from_file(line, 8);
        file_error(score_from_file, line_num);
        score->hard = score_from_file;
    }
    else{
        perror("Bad score syntax!\n");
        exit(1);
    }
}

enum state get_color_from_file(const char *color, int start){
	//check if score is actually defined, i.e. is it 
	if (start >= strlen(color)){
		puts("There is no color written!");
		return 2;
	}
    //eventually could be a hexa color...
	#define SIZE_COLOR 30
	char foo[size_foo];
	if (strlen(color) >= size_foo){
		puts("Your file is corrupted!");
		return error;
	}
	//do we need to skip the newline ('\n') in score?
	//printf("Sizeof score for dbg: %ld", strlen(score));
	strncpy(foo, &(color[start]), strlen(color) - 1);
	int num = (!is_num(foo)) ? -1 : atoi(foo);
    switch (num){
        case 0:
            return locked;
        case 1:
            return unlocked;
        case 2:
            return equipped;
        default:
            return error;
    }
}

//TODO must verify if available colors, and only 1 equipped
void read_colors(const char *line, int line_num, enum state colors[]){
    if (!strncmp(line, "\t-blue: ", 8)){
        colors[0] = get_color_from_file(line, 8);
    }
    else if (!strncmp(line, "\t-green: ", 9)){
        colors[1] = get_color_from_file(line, 9);
    }
    else if (!strncmp(line, "\t-yellow: ", 10)){
        colors[2] = get_color_from_file(line, 10);
    }
    else if (!strncmp(line, "\t-red: ", 7)){
        colors[3] = get_color_from_file(line, 7);
    }
    else{
        perror("Bad color syntax!\n");
        exit(1);
    }
}

void file_error(int score, int line_num){
    if(score < 0){
        printf("Syntax error on line %d!\n", line_num);
        exit(3);
    }
}

void print_scores(High_Scores *scores){
    printf("Easy: %d, Medium: %d, Hard: %d\n", scores->easy, scores->medium, scores->hard);
}

void print_colors(enum state colors[], int size){
    printf("Blue: %d, Red: %d, Yellow: %d, Green: %d\n",
        colors[0], colors[1], colors[2], colors[3]
    );
}

#ifdef _WIN32
struct Player_unlocks check_config_file(){
    const char *path = ".\\.config";
    const char *filename = ".\\.config\\.settings";
    if (GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES) {
        if (CreateDirectory(path, NULL)){
            puts("Created .config folder");
            if (!SetFileAttributesA(path, FILE_ATTRIBUTE_HIDDEN)) {
                puts("Could not make folder hidden...");
                exit(2);
            }
            else{
                FILE *file = fopen(filename, "w");
                fclose(file);
                High_Scores tmp = {0};
                enum state colors[COLOR_SIZE] = {locked};
                colors[1] = equipped;
                struct Player_unlocks unlocks;
                unlocks.high_scores = tmp;
                for (int i = 0; i < COLOR_SIZE; i++){
                    unlocks.colors[i] = colors[i];
                }
                return unlocks;
            }
        }
        else{
            puts("Error trying to make new dir");
            exit(1);
        }
    }
    else{
        FILE *file = fopen(filename, "r");
        //need to read the data
        if (file == NULL) {
            perror("The .settings file does not exist inside!");
            exit(1);
        }
        High_Scores scores = {0};
        enum state colors[COLOR_SIZE] = {locked};
        read_file(file, &scores, colors, COLOR_SIZE);
        fclose(file);
        struct Player_unlocks data;
        data.high_scores = scores;
        for (int i = 0; i < COLOR_SIZE; i++){
            data.colors[i] = colors[i];
        }
        return data;
    }
}


#else
#ifdef __linux__
High_Scores make_config_folder(){
	struct stat st;
	if (stat(pathname, &st) == 0 && S_ISDIR(st.st_mode)){
		puts(".config path exists!");
		//check if file .settings exists
		char filename[1000];
		snprintf(filename, 500, "%s/.settings", pathname);
		FILE *file = fopen(filename, "r");
		//need to read the data
		if (file == NULL) {
			perror("The .settings file does not exist inside!");
			exit(1);
		}
		High_Scores data = {0};
		read_scores(file, &data);
		fclose(file);
		return data;
	}
	else{
		if (mkdir(pathname, 0777) == 0) {
			puts("Created .config");
			char filename[1000];
			snprintf(filename, 500, "%s/.settings", pathname);
			FILE *file = fopen(filename, "w");
			if (file == NULL) exit(1);
			fclose(file);
			High_Scores data = {0};
			return data;
		}
		else{
			puts("Couldnt create a folder...");	
			exit(1);
		}
	}
}


//int print_high_scores(High_Scores *data){
//	return printf("\t\t\tHIGHSCORES\nEasy: %d\nMedium: %d\nHard: %d\n\n", data->easy, data->medium, data->hard);
//}


/** For testing */
//int main(int argc, char *argv[]){
//	High_Scores data = make_config_folder();
//	print_high_scores(&data);
//	data.easy = 5;
//	data.medium = 32;
//	data.hard = 78;
//	save_high_scores(&data);
//	//testing if we can change score
//	//change_scores(&data);
//	return 0;
//}
#else
#if defined(__APPLE__) && defined(__MACH__)
#else
	#error "Unavailable operating system"
#endif
#endif
#endif