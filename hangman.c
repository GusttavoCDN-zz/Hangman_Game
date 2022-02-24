#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h> // To use the SRAND function
#include "hangman.h"

char g_secret_word[WORD_SIZE];
char g_store_attempts_letters[26];
int g_n_guesses = 0; // Stores de user tries;

int main()
{
  char guess_letter;

  choose_secret_word();
  show_opening();

  while (!hit_all_letters() && !got_hanged())
  {
    draw_gallows();

    printf("\n");

    try_to_guess(&guess_letter);
  }

  if (hit_all_letters())
    show_winner_cup();
  else
    show_looser_cup();
  
  end_game();
  
  return (0);
}

void show_opening(void)
{
  printf("*********************\n");
  printf("*   HANGMAN GAME    *\n");
  printf("*********************\n");
}

int count_words_in_buffer()
{
  FILE *words_buffer = fopen("words.txt", "r");
  char store_word[WORD_SIZE];
  int words_counter;

  words_counter = 0;
  while (fgets(store_word, WORD_SIZE, words_buffer) != NULL)
    words_counter++;

  fclose(words_buffer);

  return (words_counter);
}

void choose_secret_word()
{
  FILE *words_buffer;
  int buffer_length;
  int random;
  int i;

  i = -1;
  buffer_length = count_words_in_buffer();

  srand(time(0));
  random = rand() % buffer_length;

  words_buffer = fopen("words.txt", "r");

  while (++i <= random)
    fscanf(words_buffer, "%s", g_secret_word);

  fclose(words_buffer);
}

void try_to_guess(char *guess_letter)
{
  printf("Chute uma letra: ");
  scanf(" %c", guess_letter);

  g_store_attempts_letters[g_n_guesses] = *guess_letter; // This stores de letter the user typed in the keyboard. SO this letter can't be guessed anymore.
  g_n_guesses++;
}

int did_you_guessed(char guessed_letter)
{
  int j = -1;
  int founded = 0;

  while (++j < g_n_guesses)
  {
    if (guessed_letter == g_store_attempts_letters[j])
    {
      founded = 1;
      return (founded);
    }
  }

  return (founded);
}

void draw_gallows()
{
  int i;

  int errors = wrong_guesses();

  printf("  _______       \n");
  printf(" |/      |      \n");
  printf(" |      %c%c%c  \n", (errors >= 1 ? '(' : ' '), (errors >= 1 ? '_' : ' '), (errors >= 1 ? ')' : ' '));
  printf(" |      %c%c%c  \n", (errors >= 3 ? '\\' : ' '), (errors >= 2 ? '|' : ' '), (errors >= 3 ? '/' : ' '));
  printf(" |       %c     \n", (errors >= 2 ? '|' : ' '));
  printf(" |      %c %c   \n", (errors >= 4 ? '/' : ' '), (errors >= 4 ? '\\' : ' '));
  printf(" |              \n");
  printf("_|___           \n");
  printf("\n\n");

  i = -1;
  printf("Esse é seu %d° chute\n", g_n_guesses + 1);
  while ((long unsigned int)++i < strlen(g_secret_word))
  {
    int founded = did_you_guessed(g_secret_word[i]);
    if (founded)
      printf("%c ", g_secret_word[i]);
    else if (g_secret_word[i] == ' ')
      printf(" ");
    else
      printf("_ ");
  }
}

int wrong_guesses()
{
  int i;
  int j;
  int errors;
  int founded;
  int word_size;

  i = -1;
  errors = 0;
  word_size = strlen(g_secret_word);
  while (++i < g_n_guesses)
  {
    j = -1;
    founded = 0;
    while (++j < word_size)
    {
      if (g_store_attempts_letters[i] == g_secret_word[j])
      {
        founded = 1;
        break;
      }
    }

    if (!founded)
      errors++;
  }

  return (errors);
}

int got_hanged()
{
  return wrong_guesses() >= 5; // The player loses if he missed more letters than te word size;
}

int hit_all_letters()
{
  int hit;
  int i;
  int j;
  int word_size;
  int attempts_size;

  word_size = strlen(g_secret_word);
  attempts_size = strlen(g_store_attempts_letters);
  hit = 0;
  i = -1;
  while (++i < word_size)
  {
    j = -1;
    while (++j < attempts_size)
    {
      if (g_store_attempts_letters[j] == g_secret_word[i])
        hit++;
    }
  }

  return (hit == word_size);
}

void add_new_world()
{
  FILE *words_buffer;
  char word_to_add[WORD_SIZE];

  printf("Digite a palavra que deseja adicionar: ");
  scanf("\n%s", word_to_add);

  words_buffer = fopen("words.txt", "a");

  fprintf(words_buffer, "\n%s", word_to_add);

  fclose(words_buffer);
}

void end_game()
{
  char add_word;
  int should_ask_again;

  should_ask_again = 0;
  while (should_ask_again == 0)
  {
    printf("Quer adicionar uma palavra ao nosso banco de dados?");
    printf("\nDigite [S] SIM || [N] NÃO\n");
    printf("Escolha uma opção: ");
    scanf(" %c", &add_word);

    switch (add_word)
    {
    case 'S':
      add_new_world();
      should_ask_again = 1;
      break;
    case 'N':
      should_ask_again = 1;
      break;
    default:
      printf("Opção Invalida, por favor, escolha uma opção válida!");
      break;
    }
  }

  printf("\nObrigado por jogar!\n");
}

void show_winner_cup()
{
  printf("\nParabéns, você ganhou!\n\n");
  printf("A palavra era **%s**\n\n", g_secret_word);

  printf("       ___________      \n");
  printf("      '._==_==_=_.'     \n");
  printf("      .-\\:      /-.    \n");
  printf("     | (|:.     |) |    \n");
  printf("      '-|:.     |-'     \n");
  printf("        \\::.    /      \n");
  printf("         '::. .'        \n");
  printf("           ) (          \n");
  printf("         _.' '._        \n");
  printf("        '-------'       \n\n");
  printf("\n");
}

void show_looser_cup()
{
  printf("\nPuxa, você foi enforcado!\n");
  printf("A palavra era **%s**\n\n", g_secret_word);

  printf("    _______________         \n");
  printf("   /               \\       \n");
  printf("  /                 \\      \n");
  printf("//                   \\/\\  \n");
  printf("\\|   XXXX     XXXX   | /   \n");
  printf(" |   XXXX     XXXX   |/     \n");
  printf(" |   XXX       XXX   |      \n");
  printf(" |                   |      \n");
  printf(" \\__      XXX      __/     \n");
  printf("   |\\     XXX     /|       \n");
  printf("   | |           | |        \n");
  printf("   | I I I I I I I |        \n");
  printf("   |  I I I I I I  |        \n");
  printf("   \\_             _/       \n");
  printf("     \\_         _/         \n");
  printf("       \\_______/           \n");
  printf("\n");
}