//Name:Chhiring Sherpa
//group Project 
//group members: Mustafa, Gukdeniz, Mohommmad
//hangMan Game
#include <LiquidCrystal.h>

LiquidCrystal lcdAttempts(12, 11, 5, 4, 3, 2);// this part is for lcd screen 1
LiquidCrystal lcdHangman(10, 13, 9, 8, 7, 6);// this part is for lcd screen 2

String targetWord;
String guessedWord;
char guessedLetters[26];
int guessedPositions[26];
int numCorrectGuesses;
int attemptsLeft;
int hangmanState;

// Define your database of words
String wordDatabase[30] = {
  "apple", "banana", "orange", "pear", "grape", 
  "strawberry", "blueberry", "kiwi", "pineapple", "mango",
  "peach", "plum", "cherry", "watermelon", "lemon", 
  "lime", "coconut", "pomegranate", "apricot", "fig",
  "raspberry", "blackberry", "cranberry", "melon", "nectarine",
  "persimmon", "quince", "guava", "lychee", "kiwifruit"
};

void setup() {
  lcdAttempts.begin(16, 2);
  lcdHangman.begin(16, 2);

  // Generate a random index to select a word
  int randomIndex = random(0, 30);

  // Select a word using the random index
  targetWord = wordDatabase[randomIndex];
  Serial.begin(9600);
  lcdHangman.print("Welcome to Game!");//starting screen
  lcdHangman.setCursor(0, 1);
  lcdAttempts.print("1 To Random Word");//for choosing game mode
  lcdAttempts.setCursor(0, 2);
  lcdAttempts.print("2 For Enter Word");
  numCorrectGuesses = 0;
  attemptsLeft = 6; //its initialized the attemts left
                          
  hangmanState = 0; //it initialized the ahnmg man stat to 0 
}

void loop() {
  if (Serial.available() > 0) {  //checks which option is selected
    char option = Serial.read();
    Serial.println(option); 
    if (option == '1') { //if 1 is pressed
      lcdAttempts.clear();
      lcdAttempts.print("Starting with random word..."); //waiting screen
      startGameRandom(); 
    } else if (option == '2') { //if 2 is pressed 
      lcdAttempts.clear(); 
      lcdAttempts.print("Enter your word:"); //user input
      while (!Serial.available()); 
      targetWord = Serial.readStringUntil('\n');
      targetWord.trim(); 
      Serial.println(targetWord);
      lcdAttempts.clear();
      lcdAttempts.print("Starting with your word...");//waiting screen
      startGameCustom();
    } else if (option == '3' || option == '4') {
      restartGame(); // Restart the game on '3' or '4' input
    } else {
      handleGuess(option); // Pass the guessed character to handleGuess()
    }
  }
}

void startGameRandom() { //if we select random the code will find random words from database
  lcdAttempts.clear();
  lcdAttempts.print("Guess the word:");
  guessedWord = String(targetWord.length(), '_');//this is for words adding "_"  for every letter
  updateDisplay();
  
}


void startGameCustom() {
  guessedWord = String(targetWord.length(), '_');//if we select guessing words manually
  updateDisplay();
}

void handleGuess(char guess) {
  bool alreadyGuessed = false;

  // Check if the guessed letter has already been guessed before
  for (int i = 0; i < numCorrectGuesses; i++) {
    if (guessedLetters[i] == guess) {
      alreadyGuessed = true;
      break;
    }
  }

  // If the letter has not been guessed before, proceed to handle the guess
  if (!alreadyGuessed) {
    bool correctGuess = false;

    // Check if the guessed letter is present in the target word
    for (int i = 0; i < targetWord.length(); i++) {
      if (targetWord[i] == guess) {
        // Update the guessed word with the correctly guessed letter
        guessedWord[i] = guess;
        guessedLetters[numCorrectGuesses] = guess;
        guessedPositions[numCorrectGuesses] = i;
        numCorrectGuesses++;
        correctGuess = true;
      }
    }

    // If the guess is incorrect, decrement attempts left and update hangman state
    if (!correctGuess) {
      attemptsLeft--;
      hangmanState++;
    }

    // Update the displayed guessed word and hangman state on the LCD
    updateGuessedWord();
    updateDisplay();
  }
}

void updateGuessedWord() {
  guessedWord = "";
  // Construct the guessed word based on the correct guesses
  for (int i = 0; i < targetWord.length(); i++) {
    bool found = false;
    for (int j = 0; j < numCorrectGuesses; j++) {
      if (i == guessedPositions[j]) {
        guessedWord += guessedLetters[j];
        found = true;
        break;
      }
    }
    if (!found) {
      guessedWord += "_";  // Placeholder for letters not yet guessed
    }
  }
}

void updateDisplay() {
  lcdAttempts.clear();
  lcdHangman.clear();

  // Display the number of attempts left
  lcdAttempts.setCursor(0, 0);
  lcdAttempts.print("Attempts: ");
  lcdAttempts.print(attemptsLeft);

  // Display the hangman figure
  lcdHangman.setCursor(0, 0);
  lcdHangman.print("Hangman");

  // Display the guessed word with correct guesses filled in
  lcdAttempts.setCursor(0, 1);
  lcdAttempts.print("");
  for (int i = 0; i < targetWord.length(); i++) {
    bool found = false;
    for (int j = 0; j < numCorrectGuesses; j++) {
      if (i == guessedPositions[j]) {
        lcdAttempts.print(guessedLetters[j]);
        found = true;
        break;
      }
    }
    if (!found) {
      lcdAttempts.print("_");  // Display underscores for unguessed letters
    }
   
  }

  // Display the hint for the word
  lcdHangman.setCursor(0, 1);
  lcdHangman.print("Hint: ");

  // Display clue based on word length
  lcdHangman.print(targetWord[0]);  // Always display the first letter

  if (targetWord.length() > 4) {
    for (int i = 1; i < 4; i++) {
      lcdHangman.print("_"); // Display underscores between the first and fifth letters
    }
    lcdHangman.print(targetWord[4]);  // Display the fifth letter if word length is more than 4
  }

  // Display underscores for the rest of the word
  for (int i = 5; i < targetWord.length(); i++) {
    lcdHangman.print("_");  // Display underscores for the rest of the letters
  }

  // Display different parts of the hangman based on the hangman state
  switch (hangmanState) {
    case 0:
    case 1:
      lcdHangman.setCursor(13, 0);
      lcdHangman.print("O");
      break;
    case 2:
      lcdHangman.setCursor(13, 0);
      lcdHangman.print("O");
      lcdHangman.setCursor(13, 1);
      lcdHangman.print("|");
      break;
    case 3:
      lcdHangman.setCursor(13, 0);
      lcdHangman.print("O");
      lcdHangman.setCursor(12, 1);
      lcdHangman.print("(|");
      break;
    case 4:
      lcdHangman.setCursor(13, 0);
      lcdHangman.print("O");
      lcdHangman.setCursor(12, 1);
      lcdHangman.print("(|)");
      break;
    case 5:
      lcdHangman.setCursor(13, 0);
      lcdHangman.print("O");
      lcdHangman.setCursor(12, 1);
      lcdHangman.print("(|)");
      lcdAttempts.setCursor(13, 0);
      lcdAttempts.print("|");
      lcdAttempts.setCursor(12, 1);
      lcdAttempts.print("(");
      break;
    case 6:
      lcdHangman.setCursor(13, 0);
      lcdHangman.print("O");
      lcdHangman.setCursor(12, 1);
      lcdHangman.print("(|)");
      lcdAttempts.setCursor(13, 0);
      lcdAttempts.print("|");
      lcdAttempts.setCursor(12, 1);
      lcdAttempts.print("( )");
      break;
   	
  }

  // Display win message if the guessed word matches the target word
if (guessedWord == targetWord) {
    lcdHangman.clear(); // Assuming lcdHangman is properly initialized
    lcdHangman.setCursor(0, 0);
    lcdHangman.print("YOU WIN!");

    lcdAttempts.clear(); // Assuming lcdAttempts is properly initialized
    lcdAttempts.setCursor(0, 0);
    lcdAttempts.print("Correct Answer:");
    lcdAttempts.setCursor(0, 1);
    lcdAttempts.print(targetWord);
    delay(2000);
    lcdAttempts.clear();
    lcdAttempts.setCursor(0, 0);
    lcdAttempts.print("Press '3' to");
    lcdAttempts.setCursor(0,1);
    lcdAttempts.print("restart");
    delay(2000);
}


  // Display lose message if no attempts left
  if (attemptsLeft == 0) {
    delay(2000);
    lcdHangman.clear();
    lcdAttempts.clear();
    lcdHangman.setCursor(0, 0);
    lcdHangman.print("YOU LOST!");
    lcdAttempts.setCursor(0, 0);
    lcdAttempts.print("correct anwer:");
    lcdAttempts.setCursor(0, 1);
    lcdAttempts.print(targetWord);
    delay(2000);
    lcdAttempts.clear();
    lcdAttempts.setCursor(0, 0);
    lcdAttempts.print("Press '3' to");
    lcdAttempts.setCursor(0,1);
    lcdAttempts.print("restart");
    delay(2000);
  }
}

void restartGame() {
  // Clear LCD displays
  lcdAttempts.clear();
  lcdHangman.clear();

  // Reset game variables
  numCorrectGuesses = 0;
  attemptsLeft = 6;
  hangmanState = 0;
  memset(guessedLetters, 0, sizeof(guessedLetters));
  memset(guessedPositions, 0, sizeof(guessedPositions));

  // Generate a new random index to select a new word
  int randomIndex = random(0, 30);

  // Select a new word using the random index
  targetWord = wordDatabase[randomIndex];

  // Display game starting message
  lcdAttempts.print("Starting game...");
  delay(2000); // Delay for 2 seconds

  // Clear LCD displays again
  lcdAttempts.clear();
  lcdHangman.clear();

  // Start the new game with the selected word
  startGameRandom();
}