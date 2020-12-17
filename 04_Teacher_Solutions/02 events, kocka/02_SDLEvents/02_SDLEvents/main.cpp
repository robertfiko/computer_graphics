#include <SDL.h>
#include <iostream>
#include <time.h>

void exitProgram()
{
	std::cout << "Kil�p�shez nyomj meg egy billenty�t..." << std::endl;
	std::cin.get();
}

int main( int argc, char* args[] )
{
	atexit( exitProgram );
	//
	// 1. l�p�s: inicializ�ljuk az SDL-t
	//

	// a grafikus alrendszert kapcsoljuk csak be, ha gond van, akkor jelezz�k �s l�pj�n ki
	if ( SDL_Init( SDL_INIT_VIDEO ) == -1 )
	{
		// irjuk ki a hibat es terminaljon a program
		std::cout << "[SDL ind�t�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
		return 1;
	}
			
	//
	// 2. l�p�s: hozzuk l�tre az ablakot, amire rajzolni fogunk
	//

	SDL_Window *win = nullptr;
    win = SDL_CreateWindow( "Hello SDL!",				// az ablak fejl�ce
							100,						// az ablak bal-fels� sark�nak kezdeti X koordin�t�ja
							100,						// az ablak bal-fels� sark�nak kezdeti Y koordin�t�ja
							640,						// ablak sz�less�ge
							480,						// �s magass�ga
							SDL_WINDOW_SHOWN);			// megjelen�t�si tulajdons�gok

	// ha nem siker�lt l�trehozni az ablakot, akkor �rjuk ki a hib�t, amit kaptunk �s l�pj�nk ki
    if (win == nullptr)
	{
		std::cout << "[Ablak l�trehoz�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 3. l�p�s: hozzunk l�tre egy renderel�t, rajzol�t
	//

    SDL_Renderer *ren = nullptr;
    ren = SDL_CreateRenderer(	win, // melyik ablakhoz rendelj�k hozz� a renderert
								-1,  // melyik index� renderert inicializ�ljuka
									 // a -1 a harmadik param�terben meghat�rozott ig�nyeinknek megfelel� els� renderel�t jelenti
								SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	// az ig�nyeink, azaz
																						// hardveresen gyors�tott �s vsync-et bev�r�
    if (ren == nullptr)
	{
        std::cout << "[Renderer l�trehoz�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 4. l�p�s: ind�tsuk el a f� �zenetfeldolgoz� ciklust
	// 

	// v�get kell-e �rjen a program fut�sa?
	bool quit = false;
	// feldolgozand� �zenet ide ker�l
	SDL_Event ev;
	// eg�r X �s Y koordin�t�i
	Sint32 mouseX = 0, mouseY = 0;

	float cursorColor[] = { 0, 255, 255 };
	srand(time(nullptr));

	while (!quit)
	{
		// am�g van feldolgozand� �zenet dolgozzuk fel mindet:
		while ( SDL_PollEvent(&ev) )
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if (ev.key.keysym.sym == SDLK_ESCAPE)
				{
					quit = true;
				}
				else if (ev.key.keysym.sym == SDLK_SPACE)
				{
					cursorColor[0] = rand() % 256;
					cursorColor[1] = rand() % 256;
					cursorColor[2] = rand() % 256;
				}

				break;
			case SDL_MOUSEMOTION:
				mouseX = ev.motion.x;
				mouseY = ev.motion.y;
				break;
			case SDL_MOUSEBUTTONUP:
				if (ev.button.button == SDL_BUTTON_LEFT)
				{
					cursorColor[0] = 255;
					cursorColor[1] = cursorColor[2] = 0;
				}
				else if (ev.button.button == SDL_BUTTON_RIGHT)
				{
					cursorColor[2] = 255;
					cursorColor[0] = cursorColor[1] = 0;
				}
				// eg�rgomb felenged�s�nek esem�nye; a felengedett gomb a ev.button.button -ban tal�lhat�
				// a lehets�ges gombok a k�vetkez�ek: SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, 
				//		SDL_BUTTON_RIGHT, SDL_BUTTON_WHEELUP, SDL_BUTTON_WHEELDOWN
				break;
			}
		}

		/*
		// t�r�lj�k a h�tteret feh�rre
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		// aktu�lis rajzol�si sz�n legyen z�ld �s rajzoljunk ki egy vonalat
		SDL_SetRenderDrawColor(	ren,	// renderer c�me, aminek a rajzol�si sz�n�t be akarjuk �ll�tani
								0,		// piros
								255,	// z�ld
								0,		// k�k
								255);	// �tl�tszatlans�g

		SDL_RenderDrawLine(	ren,	// renderer c�me, ahov� vonalat akarunk rajzolni
							0, 0, // vonal kezd�pontj�nak (x,y) koordin�t�i
							mouseX, mouseY);// vonal v�gpontj�nak (x,y) koordin�t�i
		
		// defini�ljunk egy (mouseX, mouseY) k�z�ppont�, tengelyekkel p�rhuzamos oldal�
		// 20x20-as n�gyzetet:
		SDL_SetRenderDrawColor(ren,
			cursorColor[0],
			cursorColor[1],
			cursorColor[2],
			255);

		SDL_Rect cursor_rect;
		cursor_rect.x = mouseX - 10;
		cursor_rect.y = mouseY - 10;
		cursor_rect.w = 20 * abs(sin(SDL_GetTicks() / 1000.0f * M_PI));
		cursor_rect.h = 20 * abs(sin(SDL_GetTicks() / 1000.0f * M_PI));
		// legyen a kit�lt�si sz�n piros
		//SDL_SetRenderDrawColor( ren, 255, 0, 0, 255 );
		SDL_RenderFillRect( ren, &cursor_rect);

		// 1. feladat: az eltelt id� f�ggv�ny�ben periodikusan n�jj�n �s cs�kkenjen
		//    az eg�rmutat� k�z�ppontj�val kirajzolt n�gysz�g

		//std::cout << SDL_GetTicks() << std::endl;

		// 2. feladat: ha a user a bal eg�rgombot nyomja meg akkor a t�glalap sz�ne v�ltson pirosra,
		//    ha a jobb eg�rgombot, akkor k�kre



		// 3. feladat: rajzolj ki egy 50 sugar� k�rvonalat az eg�rmutat� k�r�!
		// seg�ts�g: haszn�ld a SDL_RenderDrawLines()-t

		SDL_Point points[30];
		int r = 50;
		for (int i = 0; i < 30; i++)
		{
			float a = M_PI * 2 / 29 * i;
			points[i] = { mouseX + (int)(r * cosf(a)), mouseY + (int)(r * sinf(a)) };
		}

		SDL_RenderDrawLines(ren, points, 30);
		*/


		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		int w = 640;
		int h = 480;
		int size = 150;
		int offsetAmount = 30;

		SDL_Point offset = { (mouseX / (float)w - 0.5f) * 2 * offsetAmount, (mouseY / (float)h - 0.5f) * 2 * offsetAmount };

		SDL_Point points[] = { {(w - size) / 2 + offset.x, (h - size) / 2 + offset.y},
								{(w - size) / 2 + size + offset.x, (h - size) / 2 + offset.y},
								{(w - size) / 2 + size + offset.x, (h - size) / 2 + size + offset.y},
								{(w - size) / 2 + offset.x, (h - size) / 2 + size + offset.y},

								{(w - size) / 2 - offset.x, (h - size) / 2 - offset.y},
								{(w - size) / 2 + size - offset.x, (h - size) / 2 - offset.y},
								{(w - size) / 2 + size - offset.x, (h - size) / 2 + size - offset.y},
								{(w - size) / 2 - offset.x, (h - size) / 2 + size - offset.y}
		};

		SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);

		SDL_RenderDrawLines(ren, points, 4);
		SDL_RenderDrawLine(ren, points[3].x, points[3].y, points[0].x, points[0].y);

		SDL_RenderDrawLines(ren, points + 4, 4);
		SDL_RenderDrawLine(ren, points[7].x, points[7].y, points[4].x, points[4].y);

		for (int i = 0; i < 4; i++)
		{
			SDL_RenderDrawLine(ren, points[0 + i].x, points[0 + i].y, points[4 + i].x, points[4 + i].y);
		}


		// jelen�ts�k meg a backbuffer tartalm�t
		SDL_RenderPresent(ren);
	}



	//
	// 4. l�p�s: l�pj�nk ki
	// 

	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}