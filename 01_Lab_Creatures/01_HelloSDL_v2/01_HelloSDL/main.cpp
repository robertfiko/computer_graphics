#include <SDL.h>
#include <iostream>

void exitProgram()
{
	std::cout << "Kil�p�shez nyomj meg egy billenty�t..." << std::endl;
	std::cin.get();
}

int main( int argc, char* args[] )
{
	// �ll�tsuk be, hogy kil�p�s el�tt h�vja meg a rendszer az exitProgram() f�ggv�nyt
	atexit( exitProgram );

	//
	// 1. l�p�s: inicializ�ljuk az SDL-t
	//

	// a grafikus alrendszert kapcsoljuk csak be, ha gond van, akkor jelezz�k �s l�pj�nk ki
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
	// 3. l�p�s: t�r�lj�k az ablak h�tt�rsz�n�t �s v�rjunk 2 m�sodpercet
	//

	// aktu�lis rajzol�si sz�n legyen fekete �s t�r�lj�k az aktu�lis rajzol�si sz�nnel az ablak klienster�let�t
	SDL_SetRenderDrawColor(	ren,	// melyik renderel�nek �ll�tjuk be az aktu�lis rajzol�si sz�n�t
							255,	// v�r�s intenzit�s - 8 bites el�jel n�lk�li eg�sz sz�m
							0,		// z�ld intenzit�s - 8 bites el�jel n�lk�li eg�sz sz�m
							0,		// k�k intenzit�s - 8 bites el�jel n�lk�li eg�sz sz�m
							255);	// �tl�tsz�s�g - 8 bites el�jel n�lk�li eg�sz sz�m
	SDL_RenderClear(ren);

	// aktu�lis rajzol�si sz�n legyen z�ld �s rajzoljunk ki egy vonalat
	//SDL_SetRenderDrawColor(	ren,	// renderer c�me, aminek a rajzol�si sz�n�t be akarjuk �ll�tani
	//						0,		// piros
	//						255,	// z�ld
	//						0,		// k�k
	//						255);	// �tl�tszatlans�g

	//SDL_RenderDrawLine(	ren,	// renderer c�me, ahov� vonalat akarunk rajzolni
	//					10, 10, // vonal kezd�pontj�nak (x,y) koordin�t�i
	//					10, 60);// vonal v�gpontj�nak (x,y) koordin�t�i

	// 1. feladat: eg�sz�ts�k ki a fenti vonalat egy H bet�v�!

	// ...

	// 2. feladat: �rjuk ki a "HELLO" sz�veget a k�perny�re! Ehhez haszn�lhat� a
	// SDL_RenderDrawLines( <renderer ptr>, <SDL_Point t�mb>, <pontok sz�ma>); parancs!

	// ...

	// 3. feladat: 2 m�sodpercenk�nt v�ltozzon a h�tt�r sz�ne! El�sz�r legyen piros, azt�n z�ld �s v�g�l k�k,
	// majd l�pjen ki a program!

	// jelen�ts�k meg a backbuffer tartalm�t
	SDL_RenderPresent(ren);

	// v�rjunk 2 m�sodpercet
	SDL_Delay(1500);

	// aktu�lis rajzol�si sz�n legyen fekete �s t�r�lj�k az aktu�lis rajzol�si sz�nnel az ablak klienster�let�t
	SDL_SetRenderDrawColor(ren,	// melyik renderel�nek �ll�tjuk be az aktu�lis rajzol�si sz�n�t
		0,	// v�r�s intenzit�s - 8 bites el�jel n�lk�li eg�sz sz�m
		255,		// z�ld intenzit�s - 8 bites el�jel n�lk�li eg�sz sz�m
		0,		// k�k intenzit�s - 8 bites el�jel n�lk�li eg�sz sz�m
		255);	// �tl�tsz�s�g - 8 bites el�jel n�lk�li eg�sz sz�m
	SDL_RenderClear(ren);

	SDL_RenderPresent(ren);

	SDL_Delay(1500);

	// aktu�lis rajzol�si sz�n legyen fekete �s t�r�lj�k az aktu�lis rajzol�si sz�nnel az ablak klienster�let�t
	SDL_SetRenderDrawColor(ren,	// melyik renderel�nek �ll�tjuk be az aktu�lis rajzol�si sz�n�t
		0,	// v�r�s intenzit�s - 8 bites el�jel n�lk�li eg�sz sz�m
		0,		// z�ld intenzit�s - 8 bites el�jel n�lk�li eg�sz sz�m
		255,		// k�k intenzit�s - 8 bites el�jel n�lk�li eg�sz sz�m
		255);	// �tl�tsz�s�g - 8 bites el�jel n�lk�li eg�sz sz�m
	SDL_RenderClear(ren);

	SDL_RenderPresent(ren);

	SDL_Delay(1500);




	//
	// 4. l�p�s: l�pj�nk ki
	// 

	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}