
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


using namespace std;

//crtanje zmije

void renderPlayer(SDL_Renderer* renderer, SDL_Rect player, int x, int y, int scale, vector<int> tailX, vector<int> tailY, int tailLength) {
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	player.w = scale;
	player.h = scale;

	// x i y od repa uzme pa oboji u istu boju kao player

	for (int i = 0; i < tailLength; i++) {
		player.x = tailX[i];
		player.y = tailY[i];
		SDL_RenderFillRect(renderer, &player);
	}

	player.x = x;
	player.y = y;

	SDL_RenderFillRect(renderer, &player);
}

void renderFood(SDL_Renderer* renderer, SDL_Rect food) {
	SDL_SetRenderDrawColor(renderer, 225, 22, 22, 53);
	SDL_RenderFillRect(renderer, &food);
}

void renderBackground(SDL_Renderer* renderer){

        SDL_Surface * image = SDL_LoadBMP("grass.bmp");
        SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_DestroyTexture(texture);
}

void renderScore(SDL_Renderer* renderer, int tailLength, int scale, int wScale) {
	SDL_Color Black = { 0, 0, 0 };

	TTF_Font* font = TTF_OpenFont((char*)"Roboto-Black.ttf", 100);
	if (font == NULL) {
		cout << "Font loading error" << endl;
		return;
	}

    string Result;
    ostringstream convert;
    convert << tailLength;
    Result = convert.str();
    const char *cstr = Result.c_str();

	SDL_Surface* score = TTF_RenderText_Solid(font, cstr, Black);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Rect scoreRect;
	scoreRect.w = 100;
	scoreRect.h = 30;
	scoreRect.x = ((scale*wScale) / 2) - (scoreRect.w / 2);
	scoreRect.y = 0;
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);
	TTF_CloseFont(font);
}


bool checkCollision(int foodx, int foody, int playerx, int playery) {
	if (playerx == foodx && playery == foody){
		return true;
	}
	return false;
}

pair <int, int> getFoodSpawn(vector <int> tailX, vector <int> tailY, int playerX, int playerY, int scale, int wScale, int tailLength) {

	bool valid = false;
	int x = 0;
	int y = 0;
	srand(time(0));
	x = scale*(rand() % wScale);
	y = scale*(rand() % wScale);
	valid = true;

	//provera gde je rep i glava
	for (int i = 0; i < tailLength; i++) {
		if ((x == tailX[i] && y == tailY[i]) || (x == playerX && y == playerY)) {
			valid = false;
		}
	}

	if (!valid) {
		pair <int, int> foodLoc;
		foodLoc = make_pair(-100, -100);
		return foodLoc;
	}
	pair <int, int> foodLoc;
	foodLoc = make_pair(x, y);

	return foodLoc;
}

void gameOver(SDL_Renderer* renderer, SDL_Event event, int scale, int wScale, int tailLength) {
	SDL_Color Red = { 255, 0, 0 };
	SDL_Color White = { 255, 255, 255 };
	SDL_Color Black = { 0, 0, 0 };

	TTF_Font* font = TTF_OpenFont((char*)"Roboto-Black.ttf", 10);
	if (font == NULL) {
		cout << "Font loading error" << endl;
		return;
	}

	SDL_Surface* gameover = TTF_RenderText_Solid(font, "Game Over", Red);
	SDL_Surface* retry = TTF_RenderText_Solid(font, "Press Enter to retry", White);


    string Result;
    ostringstream convert;
    convert << tailLength;
    Result = convert.str();
    const char *cstr = Result.c_str();

	SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score : ") + cstr).c_str(), Black);
	SDL_Texture* gameoverMessage = SDL_CreateTextureFromSurface(renderer, gameover);
	SDL_Texture* retryMessage = SDL_CreateTextureFromSurface(renderer, retry);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Rect gameoverRect;
	SDL_Rect retryRect;
	SDL_Rect scoreRect;
	gameoverRect.w = 200;
	gameoverRect.h = 100;
	gameoverRect.x = ((scale*wScale) / 2)-(gameoverRect.w/2);
	gameoverRect.y = ((scale*wScale) / 2)-(gameoverRect.h/2)-50;
	retryRect.w = 300;
	retryRect.h = 50;
	retryRect.x = ((scale*wScale) / 2) - ((retryRect.w / 2));
	retryRect.y = (((scale*wScale) / 2) - ((retryRect.h / 2))+150);
	scoreRect.w = 100;
	scoreRect.h = 30;
	scoreRect.x = ((scale*wScale) / 2) - (scoreRect.w / 2);
	scoreRect.y = 0;
	SDL_RenderCopy(renderer, gameoverMessage, NULL, &gameoverRect);
	SDL_RenderCopy(renderer, retryMessage, NULL, &retryRect);
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);
	TTF_CloseFont(font);




	//game over dok se ne pritisne enter
	while (true) {
		SDL_RenderPresent(renderer);
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				exit(0);
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				return;
			}
		}
	}
}

void YouWin(SDL_Renderer* renderer, SDL_Event event, int scale, int wScale, int tailLength) {
	SDL_Color Red = { 255, 0, 0 };
	SDL_Color White = { 255, 255, 255 };
	SDL_Color Black = { 0, 0, 0 };
	SDL_Color Yellow = { 255, 255, 0 };

	TTF_Font* font = TTF_OpenFont((char*)"Roboto-Black.ttf", 10);
	if (font == NULL) {
		cout << "Font loading error" << endl;
		return;
	}

	SDL_Surface* gameover = TTF_RenderText_Solid(font, "You won!", Yellow);
	SDL_Surface* retry = TTF_RenderText_Solid(font, "Press Enter to play again", White);

    string Result;
    ostringstream convert;
    convert << tailLength;
    Result = convert.str();
    const char *cstr = Result.c_str();

	SDL_Surface* score = TTF_RenderText_Solid(font, cstr, Black);
	SDL_Texture* gameoverMessage = SDL_CreateTextureFromSurface(renderer, gameover);
	SDL_Texture* retryMessage = SDL_CreateTextureFromSurface(renderer, retry);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Rect gameoverRect;
	SDL_Rect retryRect;
	SDL_Rect scoreRect;
	gameoverRect.w = 200;
	gameoverRect.h = 100;
	gameoverRect.x = ((scale*wScale) / 2) - (gameoverRect.w / 2);
	gameoverRect.y = ((scale*wScale) / 2) - (gameoverRect.h / 2) - 50;
	retryRect.w = 300;
	retryRect.h = 50;
	retryRect.x = ((scale*wScale) / 2) - ((retryRect.w / 2));
	retryRect.y = (((scale*wScale) / 2) - ((retryRect.h / 2)) + 150);
	scoreRect.w = 100;
	scoreRect.h = 30;
	scoreRect.x = ((scale*wScale) / 2) - (scoreRect.w / 2);
	scoreRect.y = 0;
	SDL_RenderCopy(renderer, gameoverMessage, NULL, &gameoverRect);
	SDL_RenderCopy(renderer, retryMessage, NULL, &retryRect);
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);
	TTF_CloseFont(font);

	//you win dok se ne pritisne enter
	while (true) {
		SDL_RenderPresent(renderer);
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				exit(0);
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				return;
			}
		}
	}
}

int main(int argc, char* argv[]) {

	//init sve i ttf
	SDL_Init(SDL_INIT_EVERYTHING);
	if (TTF_Init() < 0) {
		cout << "Error: " << TTF_GetError() << endl;
	}

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	//zmija, pocetna pozicija 0,0
	SDL_Rect player;
	player.x = 0;
	player.y = 0;
	player.h = 0;
	player.w = 0;

	//rep koji se inkrementuje kad se pokupi food
	int tailLength = 0;

	//pozicije repa, vector jer std::array ima fixed size
	vector<int> tailX;
	vector<int> tailY;

	//za velicinu blocks i window
	int scale = 24;
	int wScale = 24;

	//default vrednost za zmiju
	int x = 0;
	int y = 0;
	int prevX = 0;
	int prevY = 0;

	//za kretanje
	bool up = false;
	bool down = false;
	bool right = false;
	bool left = false;
    bool inputThisFrame = false;
	bool redo = false;

	//food, x i y ce biti random
	SDL_Rect food;
	food.w = scale;
	food.h = scale;
	food.x = 0;
	food.y = 0;

    // lokacija food, provera da ne bude na mestu repa ili zmije
	pair <int, int> foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
	food.x = foodLoc.first;
	food.y = foodLoc.second;

	//pravljenje prozora i rendera
	window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, scale*wScale+1, scale*wScale+1, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//za sporije kretanje zmije
	float time = SDL_GetTicks() / 100;

	while (true) {

		float newTime = SDL_GetTicks() / 75; //75 je brzina kojom se blokovi updateuju
		float delta = newTime - time;
		time = newTime;

		inputThisFrame = false;

        // ako je rep 5 ili vise = pobeda
		if (tailLength >= 5) {
			YouWin(renderer, event, scale, wScale, tailLength);
			x = 0;
			y = 0;
			up = false;
			left = false;
			right = false;
			down = false;
			tailX.clear();
			tailY.clear();
			tailLength = 0;
			redo = false;
			foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
			if (food.x == -100 && food.y == -100) {
				redo = true;
			}
			food.x = foodLoc.first;
			food.y = foodLoc.second;
		}

		//za kretanje
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				exit(0);
			}
			if (event.type == SDL_KEYDOWN && inputThisFrame == false) {
				if (down == false && event.key.keysym.scancode == SDL_SCANCODE_UP) {
					up = true;
					left = false;
					right = false;
					down = false;
					inputThisFrame = true;
				}
				else if (right == false && event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
					up = false;
					left = true;
					right = false;
					down = false;
					inputThisFrame = true;
				}
				else if (up == false && event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
					up = false;
					left = false;
					right = false;
					down = true;
					inputThisFrame = true;
				}
				else if (left == false && event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
					up = false;
					left = false;
					right = true;
					down = false;
					inputThisFrame = true;
				}
			}
		}

		//prethodna pozicija zmije (glave) da ne bi rep bio gde je glava
		prevX = x;
		prevY = y;

		//delta*xMovementPerTick , da se zmija krece istom brzinom bez obzira na framerate
		if (up) {
			y -= delta*scale;
		}
		else if (left) {
			x -= delta*scale;
		}
		else if (right) {
			x += delta*scale;
		}
		else if (down) {
			y += delta*scale;
		}

		if (redo == true) {
			redo = false;
			foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;
			if (food.x == -100 && food.y == -100) {
				redo = true;
			}
		}

		//da li je zmija pokupila food
		if (checkCollision(food.x, food.y, x, y)) {
			//napravi novu hranu i povecaj rep
			foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;
			if (food.x == -100 && food.y == -100) {
				redo = true;
			}
			tailLength++;
		}

		//u frejmovima kad se zmija pomeri
		if (delta*scale == 24) {
			//update za tail velicinu i poziciju
			if (tailX.size() != tailLength) {
				tailX.push_back(prevX);
				tailY.push_back(prevY);
			}

			//pomeranje svakog bloka repa na mesto ispred (crashovalo je kad je pomerao u nazad)
			for (int i = 0; i < tailLength; i++) {
				if (i > 0) {
					tailX[i - 1] = tailX[i];
					tailY[i - 1] = tailY[i];
				}
			}

			if (tailLength > 0) {
				tailX[tailLength - 1] = prevX;
				tailY[tailLength - 1] = prevY;
			}
		}

		//game over ako udari u rep
		for (int i = 0; i < tailLength; i++) {
			if (x == tailX[i] && y == tailY[i]) {
				gameOver(renderer, event, scale, wScale, tailLength);
				x = 0;
				y = 0;
				up = false;
				left = false;
				right = false;
				down = false;
				tailX.clear();
				tailY.clear();
				tailLength = 0;
				redo = false;
				foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
				if (food.x == -100 && food.y == -100) {
					redo = true;
				}
				food.x = foodLoc.first;
				food.y = foodLoc.second;
			}
		}

		//game over ako udari u zid
		// stavi oba game over slucaja u isti if (mozda je bolje ovako jer je preglednije)
		if (x < 0 || y < 0 || x > scale*wScale-scale || y > scale*wScale-scale) {
			gameOver(renderer, event, scale, wScale, tailLength);
			x = 0;
			y = 0;
			up = false;
			left = false;
			right = false;
			down = false;
			tailX.clear();
			tailY.clear();
			tailLength = 0;
			redo = false;
			foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;
			if (food.x == -100 && food.y == -100) {
				redo = true;
			}
		}

		//renderovanje svega

        renderBackground(renderer);
		renderFood(renderer, food);
		renderPlayer(renderer, player, x, y, scale, tailX, tailY, tailLength);
		renderScore(renderer, tailLength, scale, wScale);

		//SDL_RenderDrawLine(renderer, 0, 0, 0, 24 * 24);
		//SDL_RenderDrawLine(renderer, 0, 24*24, 24 * 24, 24 * 24);
	//	SDL_RenderDrawLine(renderer, 24*24, 24 * 24, 24*24, 0);
//		SDL_RenderDrawLine(renderer, 24*24, 0, 0, 0);

		SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255); // pozadina za game over i you win
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);

	}


	SDL_DestroyWindow(window);

	TTF_Quit();

	SDL_Quit();

	return 0;

}
