#define WORD_SIZE 20

void 	show_opening(void);
void 	try_to_guess(char *guess_letter);
int 	did_you_guessed(char guessed_letter);
void 	draw_gallows(void);
int 	got_hanged(void);
int 	hit_all_letters(void);
int 	count_words_in_buffer();
void 	choose_secret_word();
void 	add_new_world();
void 	end_game();
void 	show_winner_cup();
void	show_looser_cup();
int		wrong_guesses();