#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <math.h>
#include <ctime>

using namespace std;

struct dane_o_pakiecie {
	int numer_pakietu;
	int rozmiar_pakietu;
};

int main() {
	ifstream wczytywany;
	wczytywany.open("a.txt", ios::binary | ios::in);//zobacz czy nie dodac dodatkowego trybu do czytania
	char * do_znakow;

	srand(time(NULL));

	int dlugosc;
	int ktory_pakiet = 0;
	int do_dlugosci = 0;

	wczytywany.seekg(0, ios::end);
	dlugosc = (wczytywany.tellg());
	do_znakow = new char[dlugosc];//zmiennej dlugosci tablica wskaznikow charow

	wczytywany.seekg(0, ios::beg);
	wczytywany.read(do_znakow, dlugosc);//powinno dzialac jeden po drugim

	dane_o_pakiecie pakiecik;

	cout << "Jaka ma byc wielkosc pakietu?" << endl;
	cin >> pakiecik.rozmiar_pakietu;

	cout.write(do_znakow, dlugosc);//kontrolne wyswietlenie (pamietaj ze tu jest wskaznik)

	//int rozmiar= sizeof(pakiecik) + sizeof(char);
	//dane_o_pakiecie* pakiecik_wskaznik;

//	int ktory_pakiet;
	int const ile_paczek = ceil((double)dlugosc / pakiecik.rozmiar_pakietu);
	int ostatni_pakiet = dlugosc -((ile_paczek-1)*pakiecik.rozmiar_pakietu);


	char** cala_paczka = new char*[ile_paczek];
	for (int i = 0; i < ile_paczek; ++i)
	{
		cala_paczka[i] = new char[sizeof(dane_o_pakiecie) + pakiecik.rozmiar_pakietu];
	}


	cout << endl;
	int rozmiar_pakietu_niestruct = pakiecik.rozmiar_pakietu;


	//dane_o_pakiecie* paczka = (dane_o_pakiecie*)(cala_paczka + 4);//wskaznik na ile miejsca z calego_pakietu to ten struct a tu dopiero od czwatego miejsca bo domyslnie wskaznik pokazuje od pierwszego
	for (int g = 0; g < ile_paczek; g++)
	{
		if (g == ile_paczek - 1)
		{
			if (ostatni_pakiet == 0)
			{
				ostatni_pakiet = pakiecik.rozmiar_pakietu;
			}
			pakiecik.rozmiar_pakietu = ostatni_pakiet;
		}
		dane_o_pakiecie* paczka = (dane_o_pakiecie*)(cala_paczka[g]);//TU MUSI BYC CALA_PACZKA[CYFRA] BO TO JUZ NIE WSKAZNIK

		paczka->numer_pakietu = g;//wyluskanie przez wskaznik
		paczka->rozmiar_pakietu = pakiecik.rozmiar_pakietu;

		for (int i = 0; i < rozmiar_pakietu_niestruct; i++)
		{
			if (do_dlugosci < dlugosc)
			{
				*(cala_paczka[g] + sizeof(dane_o_pakiecie) + i) = do_znakow[do_dlugosci];
				do_dlugosci++;
			}
			else
			{
				*(cala_paczka[g] + sizeof(dane_o_pakiecie) + i) = NULL;
			}
		}

		for (int i = 0; i < 8 + rozmiar_pakietu_niestruct; i++)//8 bo 8 bitow ze stringa + chary
		{
			if (i == 0)
				cout << "|";
			cout << to_string(cala_paczka[g][i]) << " ";
			if (i == 3)
				cout << "|";
			if (i == 7)
				cout << "|";
		}
		cout << "|" << endl;
	}

	cout << endl << "Ilosc paczek to: " << ile_paczek << endl;

	cout << "Teraz przesylanie..." << endl;


	char** po_przeslaniu = new char*[ile_paczek];
	for (int i = 0; i < ile_paczek; ++i)
	{
		po_przeslaniu[i] = new char[sizeof(dane_o_pakiecie) + rozmiar_pakietu_niestruct];
	}

	bool *czy_zapisane = new bool[ile_paczek];

	for (int i = 0; i < ile_paczek; i++)
	{
		czy_zapisane[i] = false;
	}

	//rand()
	long int do_zaklocen;
	int licznik = 0;
	

	while (licznik != ile_paczek)
	{
		do_zaklocen = rand()%ile_paczek;

		if (czy_zapisane[do_zaklocen] == false)
		{
				for (int i = 0; i < 8 + rozmiar_pakietu_niestruct; i++)//8 bo 8 bitow ze stringa + chary
				{
					po_przeslaniu[licznik] = cala_paczka[do_zaklocen];
					//po_przeslaniu[licznik][i] = cala_paczka[do_zaklocen][i];

				}
				czy_zapisane[do_zaklocen] = true;
				licznik++;
		}

	}

	for (int g = 0; g < ile_paczek; g++)
	{
		for (int i = 0; i < 8 + rozmiar_pakietu_niestruct; i++)//8 bo 8 bitow ze stringa + chary
		{
			if (i == 0)
				cout << "|";
			cout << to_string(po_przeslaniu[g][i]) << " ";
			if (i == 3)
				cout << "|";
			if (i == 7)
				cout << "|";
		}
		cout << "|" << endl;
	}
	/*
	for (int i = 0; i < ile_paczek; ++i)
	{
		delete[]cala_paczka[i];
	}
	delete[]cala_paczka;
	*/

	char** posortowane = new char*[ile_paczek];
	for (int i = 0; i < ile_paczek; ++i)
	{
		posortowane[i] = new char[sizeof(dane_o_pakiecie) + rozmiar_pakietu_niestruct];
	}



	int *tymczasowy = new int[4];
	int s;

	for(int g=0;g<ile_paczek;g++)
	{
		for (int i = 0; i < 4; i++)
		{
			tymczasowy[i] = po_przeslaniu[g][i];
			//cout << tymczasowy[i];
		
			
		}
		//g = (*(&po_przeslaniu[g][0]));
		memcpy(&s, &po_przeslaniu[g][0], sizeof(int));//do adresu s wartosc z adresupo_przeslaniu dlugosci 4(int)
		memcpy(&posortowane[s], &po_przeslaniu[g], sizeof(sizeof(dane_o_pakiecie) + rozmiar_pakietu_niestruct));

		//cout << "s to " << s << endl;
	}

	cout << "Teraz uporzadkowane: " << endl;


	for (int g = 0; g < ile_paczek; g++)
	{
		for (int i = 0; i < 8 + rozmiar_pakietu_niestruct; i++)
		{
			if (i == 0)
				cout << "|";
			cout << to_string(posortowane[g][i]) << " ";
			if (i == 3)
				cout << "|";
			if (i == 7)
				cout << "|";
		}
		cout << "|" << endl;
	}

	cout << dlugosc << endl;
	



	/*
	int tymczasowa;
	for (int x = 0; x < ile_paczek; x++)
	{
		tymczasowa = do_zaklocen[x];
		for (j = x - 1; j >= 0; j--)
		{
			if (tymczasowa < do_zaklocen[j])
				do_zaklocen[j + 1] = do_zaklocen[j];
			else
				break;
		}
		do_zaklocen[j + 1] = tymczasowa;
	}
	*/

	

	//free(paczka);
	system("PAUSE");
	return 0;
}