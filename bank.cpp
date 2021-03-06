// program do zarządzania bankiem
//

#include "stdafx.h"
#include "bank.h"

//funkcja umożliwiająca wyświetlanie polskich znaków
char *tr(char *str)
{
	static char buff[256];
	char cp[] = "\245\206\251\210\344\242\230\276\253\244\217\250\235\343\340\227\275\215ąćęłńóśżźĄĆĘŁŃÓŚŻŹ";
	if (strlen(str) >= sizeof(buff)) return str;
	char *bf = buff;
	while (*str)
	{
		char *pos = strchr(cp + 18, *str);
		*(bf++) = pos ? *(pos - 18) : *str;
		++str;
	}
	*bf = 0;
	return buff;
}



//sprawdzanie czy istnieje konto o podanym loginie
bool istnieje_konto_login(string login, opis_listy konta)
{
	bool znaleziono = false;
	wsk_konto pom;
	pom = konta.pierwsze;
	while (znaleziono == false && pom != nullptr)
	{
		if (pom->login == login && pom->czy_konto_usunięte == false)
			znaleziono = true;
		pom = pom->następny;
	}
	return znaleziono;
}

//sprawdzanie czy istnieje konto o podanym loginie i haśle
bool istnieje_konto_login_hasło(string login, string hasło, opis_listy konta)
{
	bool znaleziono = false;
	wsk_konto pom;
	pom = konta.pierwsze;
	while (znaleziono == false && pom != nullptr)
	{
		if (pom->login == login && pom->hasło == hasło && pom->czy_konto_usunięte == false)
			znaleziono = true;
		pom = pom->następny;
	}
	return znaleziono;
}

//funkcja ustawiająca wskaźnik na konto o podanym loginie
void ustaw_wskaźnik_na_konto(wsk_konto & pom, opis_listy konta, string login)
{
	pom = konta.pierwsze;
	while (pom->login != login && pom != nullptr)
	{
		pom = pom->następny;

	}
}

//wpłata dowolnej kwoty
void wpłata_dowolnej_kwoty(string login, opis_listy konta, double kwota)
{
	wsk_konto pom;

	ustaw_wskaźnik_na_konto(pom, konta, login);

	if (pom == nullptr)
	{
		cout << tr("Błąd! Takie konto nie istnieje.") << endl;
	}
	else
	{
		pom->stan_konta += kwota;
		pom->ile_transakcji++;

		wsk_transakcja nowa = new transakcja;
		nowa->kwota = kwota;
		nowa->data.dzień = obecny_dzień();
		nowa->data.miesiąc = obecny_miesiąc();
		nowa->data.rok = obecny_rok();
		nowa->następna = nullptr;

		if (pom->transakcje == nullptr)
		{
			pom->transakcje = nowa;
		}
		else
		{
			wsk_transakcja pom1;
			pom1 = pom->transakcje;
			while (pom1->następna != nullptr)
			{
				pom1 = pom1->następna;
			}
			pom1->następna = nowa;
		}
	}
}

//funkcja przesuwająca element listy o 1 miejsce w kierunku początku
void odwołanie(string login, opis_listy & konta)
{
	wsk_konto pom;
	ustaw_wskaźnik_na_konto(pom, konta, login);
	if (pom != nullptr && pom->poprzedni != nullptr)
	{
		wsk_konto pom_poprzedni = pom->poprzedni;
		wsk_konto pom_poprzedni_poprzedni = pom_poprzedni->poprzedni;
		wsk_konto pom_następny = pom->następny;

		pom->poprzedni = pom_poprzedni_poprzedni;
		pom->następny = pom_poprzedni;

		pom_poprzedni->poprzedni = pom;
		pom_poprzedni->następny = pom_następny;

		if (pom_poprzedni_poprzedni != nullptr)
		{
			pom_poprzedni_poprzedni->następny = pom;
		}
		else
		{
			konta.pierwsze = pom;
		}

		if (pom_następny != nullptr)
		{
			pom_następny->poprzedni = pom_poprzedni;
		}
		else
		{
			konta.ostatnie = pom_poprzedni;
		}
	}
}

//funkcja zwracająca login konta po podaniu nr tego konta, "0" jeżeli nie ma takiego konta
string jaki_login(string nr_konta, opis_listy konta)
{
	//ustawiam wskaźnik na odpowiednie konto
	wsk_konto pom;
	pom = konta.pierwsze;
	if (nr_konta.length() == 26 && pom != nullptr)
	{
		while (pom->nr_konta != nr_konta)
		{
			pom = pom->następny;
			if (pom == nullptr)
				break;
		}
	}
	else
		return "0";

	if (pom != nullptr)
		return pom->login;
	else
		return "0";
}

//funkcja sprawdzająca, czy limit debetu będzie przekroczony, true jeżeli przekroczony
bool czy_debet_przekroczony(string login, opis_listy konta, double kwota)
{
	wsk_konto pom;
	ustaw_wskaźnik_na_konto(pom, konta, login);
	if (pom->stan_konta - kwota < -pom->limit_debetu)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//sprawdzanie czy data jest większa od zadanej, true - jeżeli tak
bool data_jest_większa(datum ta_która_ma_być_większa, datum ta_od_której_ma_być_większa)
{
	if (ta_która_ma_być_większa.rok > ta_od_której_ma_być_większa.rok)
		return true;
	else if (ta_która_ma_być_większa.rok == ta_od_której_ma_być_większa.rok && ta_która_ma_być_większa.miesiąc > ta_od_której_ma_być_większa.miesiąc - 1)
		return true;
	else if (ta_która_ma_być_większa.rok == ta_od_której_ma_być_większa.rok && ta_która_ma_być_większa.miesiąc == ta_od_której_ma_być_większa.miesiąc - 1 && ta_która_ma_być_większa.dzień >= ta_od_której_ma_być_większa.dzień)
		return true;
	else
		return false;
}

//sprawdzanie czy data jest mniejsza od zadanej, true - jeżeli tak
bool data_jest_mniejsza(datum ta_która_ma_być_mniejsza, datum ta_od_której_ma_być_mniejsza)
{
	if (ta_która_ma_być_mniejsza.rok < ta_od_której_ma_być_mniejsza.rok)
		return true;
	else if (ta_która_ma_być_mniejsza.rok == ta_od_której_ma_być_mniejsza.rok && ta_która_ma_być_mniejsza.miesiąc < ta_od_której_ma_być_mniejsza.miesiąc - 1)
		return true;
	else if (ta_która_ma_być_mniejsza.rok == ta_od_której_ma_być_mniejsza.rok && ta_która_ma_być_mniejsza.miesiąc == ta_od_której_ma_być_mniejsza.miesiąc - 1 && ta_która_ma_być_mniejsza.dzień <= ta_od_której_ma_być_mniejsza.dzień)
		return true;
	else
		return false;
}

//wyświetlenie wyciągu jednego klienta za dany okres
void wyciąg_jeden_klient(string login, opis_listy konta, datum początek, datum koniec)
{
	wsk_konto pom;
	ustaw_wskaźnik_na_konto(pom, konta, login);
	cout << tr("Wyciąg z konta klienta ") << pom->imię << " ";
	if (pom->drugie_imię != "0")
		cout << pom->drugie_imię << " ";
	if (pom->trzecie_imię != "0")
		cout << pom->trzecie_imię << " ";
	cout << pom->nazwisko << endl;
	cout << "za okres od " << początek.dzień << " " << nazwa_miesiąca[początek.miesiąc - 1] << " " << początek.rok;
	cout << " do " << koniec.dzień << " " << nazwa_miesiąca[koniec.miesiąc - 1] << " " << koniec.rok << endl;
	if (pom->transakcje == nullptr)
	{
		cout << tr("Brak jakichkolwiek transakcji!") << endl;
	}
	else
	{
		bool czy_była_transakcja = false;
		wsk_transakcja pomocnicza = pom->transakcje;
		while (pomocnicza != nullptr)
		{
			if (data_jest_większa(pomocnicza->data, początek) && data_jest_mniejsza(pomocnicza->data, koniec))
			{
				czy_była_transakcja = true;
			}

			pomocnicza = pomocnicza->następna;
		}

		if (czy_była_transakcja == false)
		{
			cout << "Brak transakcji w podanym okresie!" << endl;
		}
		else
		{
			pomocnicza = pom->transakcje;
			cout << tr("Data \t\t\t Kwota") << endl;
			while (pomocnicza != nullptr)
			{
				if (data_jest_większa(pomocnicza->data, początek) && data_jest_mniejsza(pomocnicza->data, koniec))
				{
					cout << pomocnicza->data.dzień << " " << nazwa_miesiąca[pomocnicza->data.miesiąc] << " " << pomocnicza->data.rok << " \t " << pomocnicza->kwota << endl;
				}

				pomocnicza = pomocnicza->następna;
			}
		}
	}
}

//funkcja wyświetlająca dane klienta
void dane_klienta(string login, opis_listy konta)
{
	wsk_konto pom;
	ustaw_wskaźnik_na_konto(pom, konta, login);
	cout << tr("Imię: \t\t\t") << pom->imię << endl;
	if (pom->drugie_imię != "0")
		cout << tr("Drugie imię: \t\t") << pom->drugie_imię << endl;
	if (pom->trzecie_imię != "0")
		cout << tr("Trzecie imię: \t\t") << pom->trzecie_imię << endl;
	cout << "Nazwisko: \t\t" << pom->nazwisko << endl;
	cout << "Data urodzenia: \t" << pom->data_urodzenia.dzień << " " << nazwa_miesiąca[pom->data_urodzenia.miesiąc - 1] << " " << pom->data_urodzenia.rok << endl;
	cout << tr("Data założenia konta: \t") << pom->data_założenia_konta.dzień << " " << nazwa_miesiąca[pom->data_założenia_konta.miesiąc] << " " << pom->data_założenia_konta.rok << endl;
	cout << "Pesel: \t\t\t" << pom->pesel << endl;
	cout << "Limit debetu: \t\t" << pom->limit_debetu << endl;
	cout << "Numer konta: \t\t" << pom->nr_konta << endl;
	cout << "Stan konta: \t\t" << pom->stan_konta << endl;
	cout << "Adres zamieszkania: \tul. " << pom->adres_zamieszkania.ulica << " " << pom->adres_zamieszkania.nr_domu << ", " << pom->adres_zamieszkania.miasto << ", " << pom->adres_zamieszkania.kraj << endl;
	cout << "Adres korespondencji: \t";
	if (pom->adres_korespondencji.ulica == "0")
		cout << "Taki sam jak adres zamieszkania." << endl;
	else
		cout << " ul. " << pom->adres_korespondencji.ulica << " " << pom->adres_korespondencji.nr_domu << ", " << pom->adres_korespondencji.miasto << ", " << pom->adres_korespondencji.kraj << endl;
}

//zmiana danych klienta; 0-login,1-hasło,2-imię,3-drugie imię,4-trzecie imię,5-nazwisko,6-data urodzenia,7-pesel,8-limit debetu,9-adres zamimeszkania,10-adres korespondencji
void zmiana_wybranej_danej(string &login, opis_listy konta, int która_dana)
{
	wsk_konto pom;
	ustaw_wskaźnik_na_konto(pom, konta, login);
	if (która_dana == 0)
	{
		bool jeszcze_raz;
		do {
			cout << "Podaj nowy login: ";
			string nowy_login;
			cin >> nowy_login;
			if (istnieje_konto_login(nowy_login, konta))
			{
				cout << tr("Konto o podanym loginie już istnieje!") << endl;
				cout << tr("Wpisz inny login!");
				Sleep(2500);
				system("cls");
				jeszcze_raz = true;
			}
			else
			{
				jeszcze_raz = false;
				pom->login = nowy_login;
				login = nowy_login;
			}
		} while (jeszcze_raz);
	}
	if (która_dana == 1)
	{
		bool jeszcze_raz;
		do {
			cout << tr("Podaj nowe hasło: ");
			string nowe_hasło;
			cin >> nowe_hasło;
			system("cls");
			cout << tr("Podaj hasło jeszcze raz: ");
			string hasło_jeszcze_raz;
			cin >> hasło_jeszcze_raz;
			if (nowe_hasło == hasło_jeszcze_raz)
			{
				pom->hasło = nowe_hasło;
				jeszcze_raz = false;
			}
			else
			{
				cout << tr("Coś poszło nie tak!");
				Sleep(2000);
				system("cls");
				jeszcze_raz = true;
			}
		} while (jeszcze_raz);
	}
	if (która_dana == 2)
	{
		cout << tr("Podaj swoje imię: ");
		string imię;
		cin >> imię;
		pom->imię = imię;
	}
	if (która_dana == 3)
	{
		cout << tr("Podaj swoje drugie imię [jeżeli nie posiadasz takowego, wpisz 0]: ");
		string drugie_imię;
		cin >> drugie_imię;
		pom->drugie_imię = drugie_imię;
	}
	if (która_dana == 4)
	{
		cout << tr("Podaj swoje trzecie imię [jeżeli nie posiadasz takowego, wpisz 0]: ");
		string trzecie_imię;
		cin >> trzecie_imię;
		pom->trzecie_imię = trzecie_imię;
	}
	if (która_dana == 5)
	{
		cout << "Podaj swoje nazwisko: ";
		string nazwisko;
		cin >> nazwisko;
		pom->nazwisko = nazwisko;
	}
	if (która_dana == 6)
	{
		cout << tr("Podaj swoją datę urodzenia [w liczbach]: ") << endl;
		datum data;
		cout << tr("Dzień: ");
		cin >> data.dzień;
		pom->data_urodzenia.dzień = data.dzień;
		cout << tr("Miesiąc: ");
		cin >> data.miesiąc;
		pom->data_urodzenia.miesiąc = data.miesiąc;
		cout << "Rok: ";
		cin >> data.rok;
		pom->data_urodzenia.rok = data.rok;
	}
	if (która_dana == 7)
	{
		cout << tr("Podaj swój pesel: ");
		string pesel;
		cin >> pesel;
		pom->pesel = pesel;
	}
	if (która_dana == 8)
	{
		bool jeszcze_raz;
		do {
			cout << tr("Podaj swój limit debetu [liczba dodatnia w zł][maksymalnie 50000]: ");
			double limit;
			cin >> limit;
			if (limit < 0)
			{
				cout << tr("Podaj liczbę dodatnią!");
				system("cls");
				jeszcze_raz = true;
			}
			else if (limit > 50000)
			{
				cout << tr("Nie możesz mieć takiego dużego debetu!");
				system("cls");
				jeszcze_raz = true;
			}
			else
			{
				pom->limit_debetu = limit;
				jeszcze_raz = false;
			}
		} while (jeszcze_raz);
	}
	if (która_dana == 9)
	{
		cout << tr("Podaj swój adres zamieszkania: ") << endl;
		adres adres;
		cout << tr("Ulica: ");
		cin >> adres.ulica;
		pom->adres_zamieszkania.ulica = adres.ulica;
		cout << tr("Numer domu/numer mieszkania [jeżeli nie masz numeru mieszkania, wpisz tylko numer domu]: ");
		cin >> adres.nr_domu;
		pom->adres_zamieszkania.nr_domu = adres.nr_domu;
		cout << "Miasto: ";
		cin >> adres.miasto;
		pom->adres_zamieszkania.miasto = adres.miasto;
		cout << "Kraj: ";
		cin >> adres.kraj;
		pom->adres_zamieszkania.kraj = adres.kraj;
	}
	if (która_dana == 10)
	{
		cout << "Czy adres korespondencji jest taki sam jak adres zamieszkania? [1 - TAK/0 - NIE]: ";
		bool tak;
		cin >> tak;
		if (tak)
		{
			pom->adres_korespondencji.ulica = "0";
		}
		else
		{
			cout << tr("Podaj swój adres korespondencji: ") << endl;
			adres adres;
			cout << tr("Ulica: ");
			cin >> adres.ulica;
			pom->adres_korespondencji.ulica = adres.ulica;
			cout << tr("Numer domu/numer mieszkania [jeżeli nie masz numeru mieszkania, wpisz tylko numer domu]: ");
			cin >> adres.nr_domu;
			pom->adres_korespondencji.nr_domu = adres.nr_domu;
			cout << "Miasto: ";
			cin >> adres.miasto;
			pom->adres_korespondencji.miasto = adres.miasto;
			cout << "Kraj: ";
			cin >> adres.kraj;
			pom->adres_korespondencji.kraj = adres.kraj;
		}
	}
}

//zmiana wybranej danej
void zmiana_danych(string &login, opis_listy konta)
{
	bool jeszcze_raz;
	do {
		cout << "Wybierz: " << endl;
		cout << tr("0  - aby zmienić login;") << endl;
		cout << tr("1  - aby zmienić hasło;") << endl;
		cout << tr("2  - aby zmienić imię;") << endl;
		cout << tr("3  - aby zmienić drugie imię;") << endl;
		cout << tr("4  - aby zmienić trzecie imię;") << endl;
		cout << tr("5  - aby zmienić nazwisko;") << endl;
		cout << tr("6  - aby zmienić datę urodzenia;") << endl;
		cout << tr("7  - aby zmienić pesel;") << endl;
		cout << tr("8  - aby zmienić limit debetu;") << endl;
		cout << tr("9  - aby zmienić adres zamieszkania;") << endl;
		cout << tr("10 - aby zmienić adres korespondencji.") << endl;
		int co;
		cin >> co;
		system("cls");
		switch (co)
		{
		case 0:
			zmiana_wybranej_danej(login, konta, 0);
			break;
		case 1:
			zmiana_wybranej_danej(login, konta, 1);
			break;
		case 2:
			zmiana_wybranej_danej(login, konta, 2);
			break;
		case 3:
			zmiana_wybranej_danej(login, konta, 3);
			break;
		case 4:
			zmiana_wybranej_danej(login, konta, 4);
			break;
		case 5:
			zmiana_wybranej_danej(login, konta, 5);
			break;
		case 6:
			zmiana_wybranej_danej(login, konta, 6);
			break;
		case 7:
			zmiana_wybranej_danej(login, konta, 7);
			break;
		case 8:
			zmiana_wybranej_danej(login, konta, 8);
			break;
		case 9:
			zmiana_wybranej_danej(login, konta, 9);
			break;
		case 10:
			zmiana_wybranej_danej(login, konta, 10);
			break;
		}
		system("cls");
		cout << tr("Chcesz jeszcze coś zmienić w swoim koncie? [1 - TAK/0 - NIE]: ");
		cin >> jeszcze_raz;
	} while (jeszcze_raz);
}

//tworzenie nowego konta
void utwórz_konto(string login, string hasło, opis_listy &konta)
{
	wsk_konto nowy = new konto;
	nowy->login = login;
	nowy->hasło = hasło;
	nowy->czy_konto_usunięte = false;
	nowy->data_założenia_konta.dzień = obecny_dzień();
	nowy->data_założenia_konta.miesiąc = obecny_miesiąc();
	nowy->data_założenia_konta.rok = obecny_rok();
	nowy->nr_konta = numer_konta();
	nowy->stan_konta = 0;
	nowy->transakcje = nullptr;
	nowy->ile_transakcji = 0;
	nowy->następny = nullptr;
	if (konta.pierwsze == nullptr)
	{
		konta.pierwsze = nowy;
		nowy->poprzedni = nullptr;
	}
	else
	{
		konta.ostatnie->następny = nowy;
		nowy->poprzedni = konta.ostatnie;
	}
	konta.ostatnie = nowy;
	konta.liczba_kont++;
}

void zwolnij_pamięć(opis_listy &konta)
{
	wsk_konto pom = konta.pierwsze;
	while (pom != nullptr)
	{
		if (pom->transakcje != nullptr)
		{
			wsk_transakcja poprzednia = pom->transakcje;
			wsk_transakcja nast = poprzednia->następna;
			delete poprzednia;
			while (nast != nullptr)
			{
				poprzednia = nast;
				nast = nast->następna;
				delete poprzednia;
			}
		}
		if (pom->następny == nullptr)
		{
			delete pom;
			pom = nullptr;
		}
		else
		{
			pom = pom->następny;
			delete pom->poprzedni;
		}
	}
}

//funkcja generująca nr konta
string numer_konta()
{
	srand(time(0));
	string nr_konta_koniec;
	for (int i = 0; i < 16; i++)
	{
		char znak = rand() % 10;
		znak += 48;
		nr_konta_koniec += znak;
	}
	string nr_konta = "6510100002" + nr_konta_koniec;
	return nr_konta;
}

//funkcja zwracająca obecny dzień
int obecny_dzień()
{
	time_t czas = time(0);
	tm t;
	gmtime_s(&t, &czas);
	return t.tm_mday;
}

//funkcja zwracająca obecny miesiąc
int obecny_miesiąc()
{
	time_t czas = time(0);
	tm t;
	gmtime_s(&t, &czas);
	return t.tm_mon;
}

//funkcja zwracająca obecny rok
int obecny_rok()
{
	time_t czas = time(0);
	tm t;
	gmtime_s(&t, &czas);
	return t.tm_year + 1900;
}

void odczytaj_dane_z_pliku(opis_listy &konta)
{
	ifstream otwórz_konto, otwórz_transakcje;
	otwórz_konto.open("dane_kont.kurczak", ios_base::in | ios_base::binary);
	if (otwórz_konto.is_open() == false)
	{
		cout << tr("Nie udało się otworzyć pliku z zapisanymi danymi odnośnie kont użytkowników!") << endl;
		cout << tr("Jeżeli pierwszy raz używasz ten program - nie przejmuj się, wszystko jest w porządku.") << endl << endl;
		cout << tr("Jeżeli jednak używałeś go już wcześniej, sprawdź czy aby na pewno") << endl;
		cout << tr("w tym samym folderze, co ten program, znajduje się plik o nazwie \"dane_kont.kurczak\" \noraz/lub \"transakcje_login.kurczak\"") << endl;
		cout << tr("Następnie otwórz program jeszcze raz! Życzę powodzenia!") << endl;
		system("pause");
		konta.pierwsze = nullptr;
		konta.ostatnie = nullptr;
		konta.liczba_kont = 0;
		system("cls");
	}
	else
	{
		wsk_konto pom = new konto;

		wsk_konto poprzednie = nullptr;
		otwórz_konto.read((char*)&konta.liczba_kont, sizeof konta.liczba_kont);
		for (int ile = 0; ile < konta.liczba_kont; ile++)
		{
			otwórz_konto.read((char*)&pom->czy_konto_usunięte, sizeof pom->czy_konto_usunięte);
			getline(otwórz_konto, pom->login, '\0');
			getline(otwórz_konto, pom->hasło, '\0');
			getline(otwórz_konto, pom->imię, '\0');
			getline(otwórz_konto, pom->drugie_imię, '\0');
			getline(otwórz_konto, pom->trzecie_imię, '\0');
			getline(otwórz_konto, pom->nazwisko, '\0');
			otwórz_konto.read((char*)&pom->data_urodzenia.dzień, sizeof pom->data_urodzenia.dzień);
			otwórz_konto.read((char*)&pom->data_urodzenia.miesiąc, sizeof pom->data_urodzenia.miesiąc);
			otwórz_konto.read((char*)&pom->data_urodzenia.rok, sizeof pom->data_urodzenia.rok);
			otwórz_konto.read((char*)&pom->data_założenia_konta.dzień, sizeof pom->data_założenia_konta.dzień);
			otwórz_konto.read((char*)&pom->data_założenia_konta.miesiąc, sizeof pom->data_założenia_konta.miesiąc);
			otwórz_konto.read((char*)&pom->data_założenia_konta.rok, sizeof pom->data_założenia_konta.rok);
			getline(otwórz_konto, pom->pesel, '\0');
			otwórz_konto.read((char*)&pom->limit_debetu, sizeof pom->limit_debetu);
			getline(otwórz_konto, pom->nr_konta, '\0');
			otwórz_konto.read((char*)&pom->stan_konta, sizeof pom->stan_konta);
			getline(otwórz_konto, pom->adres_zamieszkania.ulica, '\0');
			getline(otwórz_konto, pom->adres_zamieszkania.nr_domu, '\0');
			getline(otwórz_konto, pom->adres_zamieszkania.miasto, '\0');
			getline(otwórz_konto, pom->adres_zamieszkania.kraj, '\0');
			getline(otwórz_konto, pom->adres_korespondencji.ulica, '\0');
			getline(otwórz_konto, pom->adres_korespondencji.nr_domu, '\0');
			getline(otwórz_konto, pom->adres_korespondencji.miasto, '\0');
			getline(otwórz_konto, pom->adres_korespondencji.kraj, '\0');
			if (ile == 0)
			{
				konta.pierwsze = pom;
				pom->poprzedni = nullptr;
			}
			else
			{
				pom->poprzedni = poprzednie;
				poprzednie->następny = pom;
			}
			if (ile == konta.liczba_kont - 1)
			{
				konta.ostatnie = pom;
				pom->następny = nullptr;
			}

			string transakcje = "transakcje_" + pom->login + ".kurczak";
			otwórz_transakcje.open(transakcje, ios_base::in | ios_base::binary);
			if (otwórz_transakcje.is_open())
			{
				wsk_transakcja pomocnicza = new transakcja;
				wsk_transakcja poprzednia = nullptr;
				otwórz_transakcje.read((char*)&pom->ile_transakcji, sizeof pom->ile_transakcji);
				for (int ile_transakcji = 0; ile_transakcji < pom->ile_transakcji; ile_transakcji++)
				{
					otwórz_transakcje.read((char*)&pomocnicza->kwota, sizeof pomocnicza->kwota);
					otwórz_transakcje.read((char*)&pomocnicza->data.dzień, sizeof pomocnicza->data.dzień);
					otwórz_transakcje.read((char*)&pomocnicza->data.miesiąc, sizeof pomocnicza->data.miesiąc);
					otwórz_transakcje.read((char*)&pomocnicza->data.rok, sizeof pomocnicza->data.rok);
					if (ile_transakcji == 0)
						pom->transakcje = pomocnicza;
					else
						poprzednia->następna = pomocnicza;

					if (ile_transakcji == pom->ile_transakcji - 1)
						pomocnicza->następna = nullptr;

					poprzednia = pomocnicza;
					pomocnicza = new transakcja;
				}
				otwórz_transakcje.close();
			}
			else
				pom->transakcje = nullptr;

			poprzednie = pom;
			pom = new konto;
		}
		otwórz_konto.close();
	}
}

void zapisz_dane_do_pliku(opis_listy konta)
{
	ofstream zapisz_konta, zapisz_transakcje;
	zapisz_konta.open("dane_kont.kurczak", ios_base::out | ios_base::binary | ios_base::trunc);
	wsk_konto pom = konta.pierwsze;
	zapisz_konta.write((char*)&konta.liczba_kont, sizeof konta.liczba_kont);
	while (pom != nullptr)
	{
		zapisz_konta.write((char*)&pom->czy_konto_usunięte, sizeof pom->czy_konto_usunięte);
		zapisz_konta.write(pom->login.c_str(), pom->login.size() + 1);
		zapisz_konta.write(pom->hasło.c_str(), pom->hasło.size() + 1);
		zapisz_konta.write(pom->imię.c_str(), pom->imię.size() + 1);
		zapisz_konta.write(pom->drugie_imię.c_str(), pom->drugie_imię.size() + 1);
		zapisz_konta.write(pom->trzecie_imię.c_str(), pom->trzecie_imię.size() + 1);
		zapisz_konta.write(pom->nazwisko.c_str(), pom->nazwisko.size() + 1);
		zapisz_konta.write((char*)&pom->data_urodzenia.dzień, sizeof pom->data_urodzenia.dzień);
		zapisz_konta.write((char*)&pom->data_urodzenia.miesiąc, sizeof pom->data_urodzenia.miesiąc);
		zapisz_konta.write((char*)&pom->data_urodzenia.rok, sizeof pom->data_urodzenia.rok);
		zapisz_konta.write((char*)&pom->data_założenia_konta.dzień, sizeof pom->data_założenia_konta.dzień);
		zapisz_konta.write((char*)&pom->data_założenia_konta.miesiąc, sizeof pom->data_założenia_konta.miesiąc);
		zapisz_konta.write((char*)&pom->data_założenia_konta.rok, sizeof pom->data_założenia_konta.rok);
		zapisz_konta.write(pom->pesel.c_str(), pom->pesel.size() + 1);
		zapisz_konta.write((char*)&pom->limit_debetu, sizeof pom->limit_debetu);
		zapisz_konta.write(pom->nr_konta.c_str(), pom->nr_konta.size() + 1);
		zapisz_konta.write((char*)&pom->stan_konta, sizeof pom->stan_konta);
		zapisz_konta.write(pom->adres_zamieszkania.ulica.c_str(), pom->adres_zamieszkania.ulica.size() + 1);
		zapisz_konta.write(pom->adres_zamieszkania.nr_domu.c_str(), pom->adres_zamieszkania.nr_domu.size() + 1);
		zapisz_konta.write(pom->adres_zamieszkania.miasto.c_str(), pom->adres_zamieszkania.miasto.size() + 1);
		zapisz_konta.write(pom->adres_zamieszkania.kraj.c_str(), pom->adres_zamieszkania.kraj.size() + 1);
		zapisz_konta.write(pom->adres_korespondencji.ulica.c_str(), pom->adres_korespondencji.ulica.size() + 1);
		zapisz_konta.write(pom->adres_korespondencji.nr_domu.c_str(), pom->adres_korespondencji.nr_domu.size() + 1);
		zapisz_konta.write(pom->adres_korespondencji.miasto.c_str(), pom->adres_korespondencji.miasto.size() + 1);
		zapisz_konta.write(pom->adres_korespondencji.kraj.c_str(), pom->adres_korespondencji.kraj.size() + 1);

		if (pom->transakcje != nullptr)
		{
			string plik_z_transakcjami = "transakcje_" + pom->login + ".kurczak";
			zapisz_transakcje.open(plik_z_transakcjami, ios_base::out | ios_base::binary | ios_base::trunc);
			wsk_transakcja pomocnicza = pom->transakcje;
			zapisz_transakcje.write((char*)&pom->ile_transakcji, sizeof pom->ile_transakcji);
			while (pomocnicza != nullptr)
			{
				zapisz_transakcje.write((char*)&pomocnicza->kwota, sizeof pomocnicza->kwota);
				zapisz_transakcje.write((char*)&pomocnicza->data.dzień, sizeof pomocnicza->data.dzień);
				zapisz_transakcje.write((char*)&pomocnicza->data.miesiąc, sizeof pomocnicza->data.miesiąc);
				zapisz_transakcje.write((char*)&pomocnicza->data.rok, sizeof pomocnicza->data.rok);
				pomocnicza = pomocnicza->następna;
			}
			zapisz_transakcje.close();
		}
		pom = pom->następny;
	}
	zapisz_konta.close();
}

int main()
{
	opis_listy konta;
	
	odczytaj_dane_z_pliku(konta);

	bool czy_była_zmiana;
	int wybor;
	do
	{
		cout << "BANK MANAGER 1.0" << endl << endl;
		cout << tr("MENU GŁÓWNE") << endl;
		cout << "Wybierz: " << endl;
		cout << tr("1 - aby się zalogować;") << endl;
		cout << tr("2 - aby założyć konto;") << endl;
		cout << tr("3 - aby wpłacić pieniądze na dowolne konto;") << endl;
		cout << tr("0 - aby wyjść z programu!") << endl;

		//menu
		cin >> wybor;
		system("cls");
		string login;
		string hasło;
		string nr_konta;
		switch (wybor)
		{
		case 1:
			int jeszcze_raz;
			do {
				cout << "Podaj login: ";
				cin >> login;
				cout << tr("Podaj hasło: ");
				cin >> hasło;
				if (istnieje_konto_login_hasło(login, hasło, konta) == false)
				{
					system("cls");
					cout << tr("Błędny login lub hasło!") << endl << endl;
					cout << tr("Wybierz: ") << endl;
					cout << tr("1 - aby spróbować jeszcze raz;") << endl;
					cout << tr("0 - aby wrócić do menu głownego.") << endl;
					cin >> jeszcze_raz;
					system("cls");
				}
				else
				{
					jeszcze_raz = false;
					czy_była_zmiana = true;
					odwołanie(login, konta);
					//menu konta
					do {
						system("cls");
						cout << "MENU KONTA" << endl;
						cout << "Wybierz: " << endl;
						cout << tr("1 - aby wpłacić dowolną kwotę na swoje konto;") << endl;
						cout << tr("2 - aby wpłacić 5 złotych na swoje konto;") << endl;
						cout << tr("3 - aby wypłacić dowolną kwotę ze swojego konta;") << endl;
						cout << tr("4 - aby wypłacić 5 złotych ze swojego konta;") << endl;
						cout << tr("5 - aby wykonać przelew;") << endl;
						cout << tr("6 - aby uzyskać wyciąg ze swojego konta za dany okres;") << endl;
						cout << tr("7 - aby sprawdzić aktualny stan konta;") << endl;
						cout << tr("8 - aby uzyskać informację o wszystkich danych swojego konta;") << endl;
						cout << tr("9 - aby zmienić dane swojego konta;") << endl;
						cout << tr("0 - aby usunąć swoje konto.") << endl;
						cout << tr("10 - aby wyjść do menu głównego") << endl;

						int co_robić;
						cin >> co_robić;
						system("cls");
						switch (co_robić)
						{
						case 1:
							odwołanie(login, konta);
							//wpłata dowolnej kwoty
							cout << tr("Podaj kwotę [w zł]: ");
							double kwota;
							cin >> kwota;
							if (kwota > 0)
							{
								wpłata_dowolnej_kwoty(login, konta, kwota);
								cout << tr("Kwota w wysokości ") << kwota;
								cout << tr(" zł została pomyślnie wpłacona na twe konto!") << endl;
							}
							else if (kwota == 0)
							{
								cout << tr("Wpłacanie 0 zł nie ma sensu!") << endl;
							}
							else
							{
								cout << tr("Nie możesz wpłacić ujemnej kwoty!") << endl;
							}
							break;
						case 2:
							odwołanie(login, konta);
							//wpłata 5 zł
							wpłata_dowolnej_kwoty(login, konta, 5);
							cout << tr("Kwota w wysokości 5 zł została pomyślnie wpłacona na twe konto!") << endl;
							break;
						case 3:
							odwołanie(login, konta);
							//wypłata dowolnej kwoty
							cout << tr("Podaj kwotę [w zł]: ");
							cin >> kwota;
							if (kwota > 0)
							{
								if (czy_debet_przekroczony(login, konta, kwota) == false)
								{
									wpłata_dowolnej_kwoty(login, konta, -kwota);
									cout << tr("Kwota w wysokości ") << kwota;
									cout << tr(" zł została pomyślnie wypłacona z twojego konta!") << endl;
								}
								else
								{
									cout << tr("Nie można wypłacić ") << kwota << tr(" zł.") << endl;
									cout << tr("Limit debetu zostałby przekroczony!") << endl;
								}
							}
							else if (kwota == 0)
							{
								cout << tr("Wypłacanie 0 zł nie ma sensu!") << endl;
							}
							else
							{
								if (czy_debet_przekroczony(login, konta, -kwota) == false)
								{
									wpłata_dowolnej_kwoty(login, konta, kwota);
									cout << tr("Kwota w wysokości ") << -kwota << tr(" zł została pomyślnie wypłacona z twojego konta!") << endl;
								}
								else
								{
									cout << tr("Nie można wypłacić ") << -kwota << tr(" zł.") << endl;
									cout << tr("Limit debetu zostałby przekroczony!") << endl;
								}
							}
							break;
						case 4:
							odwołanie(login, konta);
							// wypłata 5 zł
							if (czy_debet_przekroczony(login, konta, 5) == false)
							{
								wpłata_dowolnej_kwoty(login, konta, -5);
								cout << tr("Kwota w wysokości 5 zł została pomyślnie wypłacona z twojego konta!") << endl;
							}
							else
							{
								cout << tr("Nie można wypłacić 5 zł.") << endl;
								cout << tr("Limit debetu zostałby przekroczony!") << endl;
							}
							break;
						case 5:
							odwołanie(login, konta);
							//przelew
							cout << tr("Podaj ostatnie 16 cyfr nr konta, na które chcesz wykonać przelew: ");
							cin >> nr_konta;
							if (jaki_login("6510100002" + nr_konta, konta) == login)
							{
								cout << "Puszczanie przelewu ze swojego konta na swoje konto jest bez sensu!" << endl;
							}
							else if (jaki_login("6510100002" + nr_konta, konta) == "0" || istnieje_konto_login(jaki_login("6510100002" + nr_konta, konta), konta) == false)
							{
								cout << "Nie istnieje konto o podanym numerze!" << endl;
							}
							else
							{
								cout << tr("Podaj kwotę przelewu [w zł]: ");
								cin >> kwota;
								if (kwota < 0)
								{
									cout << tr("Nie możesz puścić przelewu z ujemną kwotą!") << endl;
								}
								else if (kwota == 0)
								{
									cout << tr("Puszczanie przelewu o wartości 0 zł jest bez sensu!") << endl;
								}
								else
								{
									if (czy_debet_przekroczony(login, konta, kwota) == false)
									{
										odwołanie(jaki_login("6510100002" + nr_konta, konta), konta);
										wpłata_dowolnej_kwoty(jaki_login("6510100002" + nr_konta, konta), konta, kwota);
										wpłata_dowolnej_kwoty(login, konta, -kwota);
										cout << tr("Kwota w wysokości ") << kwota;
										cout << tr(" zł została pomyślnie przelana na konto o numerze 6510100002") << nr_konta << "." << endl;
									}
									else
									{
										cout << tr("Nie można puścić przelewu na kwotę ") << kwota << tr(" zł!") << endl;
										cout << tr("Limit debetu zostałby przekroczony!") << endl;
									}
								}
							}
							break;
						case 6:
							odwołanie(login, konta);
							//wyciąg
							datum początek;
							datum koniec;
							cout << tr("Podaj datę, od kiedy chcesz mieć wyciąg [w liczbach]: ") << endl;
							cout << tr("Dzień: ");
							cin >> początek.dzień;
							cout << tr("Miesiąc: ");
							cin >> początek.miesiąc;
							cout << tr("Rok: ");
							cin >> początek.rok;
							system("cls");
							cout << tr("Podaj datę, do kiedy chcesz mieć wyciąg [w liczbach]: ") << endl;
							cout << tr("Dzień: ");
							cin >> koniec.dzień;
							cout << tr("Miesiąc: ");
							cin >> koniec.miesiąc;
							cout << tr("Rok: ");
							cin >> koniec.rok;
							system("cls");
							wyciąg_jeden_klient(login, konta, początek, koniec);
							break;
						case 7:
							odwołanie(login, konta);
							//aktualny stan konta
							wsk_konto pom;
							ustaw_wskaźnik_na_konto(pom, konta, login);
							cout << "Aktualny stan twojego konta to: " << endl;
							cout << pom->stan_konta << endl;
							break;
						case 8:
							odwołanie(login, konta);
							//wszystkie dane
							dane_klienta(login, konta);
							break;
						case 9:
							odwołanie(login, konta);
							//zmiana danych
							zmiana_danych(login, konta);
							break;
						case 0:
							//usunięcie konta
							ustaw_wskaźnik_na_konto(pom, konta, login);
							if (pom->stan_konta < 0)
							{
								cout << tr("Niestety, nie możesz usunąć swojego konta, gdy masz debet!");
								Sleep(2500);
								jeszcze_raz = 3;
							}
							else
							{
								pom->czy_konto_usunięte = true;
								jeszcze_raz = 2;
							}
							break;
						}
						if (co_robić != 0 && co_robić != 10)
						{
							cout << endl << "Wybierz: " << endl;
							cout << tr("1 - aby zalogować się na inne konto;") << endl;
							cout << tr("0 - aby wrócić do menu głównego;") << endl;
							cout << tr("3 - aby wrócić do menu konta i wykonać inną operację na swoim koncie.") << endl;
							cin >> jeszcze_raz;
						}
					} while (jeszcze_raz == 3);
				}
			} while (jeszcze_raz == 1);
			break;
		case 2:
			//nowe konto
			czy_była_zmiana = true;
			do {
				cout << "Podaj login: ";
				cin >> login;
				if (istnieje_konto_login(login, konta))
				{
					cout << tr("Konto o podanym loginie już istnieje!") << endl;
					cout << tr("Wpisz inny login!");
					Sleep(2500);
					system("cls");
					jeszcze_raz = 1;
				}
				else
				{
					jeszcze_raz = 0;
					do {
						cout << tr("Podaj hasło: ");
						cin >> hasło;
						system("cls");
						cout << tr("Podaj hasło jeszcze raz: ");
						string hasło_jeszcze_raz;
						cin >> hasło_jeszcze_raz;
						if (hasło == hasło_jeszcze_raz)
						{
							system("cls");
							utwórz_konto(login, hasło, konta);
							zmiana_wybranej_danej(login, konta, 2);
							system("cls");
							zmiana_wybranej_danej(login, konta, 3);
							system("cls");
							zmiana_wybranej_danej(login, konta, 4);
							system("cls");
							zmiana_wybranej_danej(login, konta, 5);
							system("cls");
							zmiana_wybranej_danej(login, konta, 6);
							system("cls");
							zmiana_wybranej_danej(login, konta, 7);
							system("cls");
							zmiana_wybranej_danej(login, konta, 8);
							system("cls");
							zmiana_wybranej_danej(login, konta, 9);
							system("cls");
							zmiana_wybranej_danej(login, konta, 10);
							system("cls");
							cout << tr("Sprawdź czy twoje dane są na pewno poprawne!") << endl;
							dane_klienta(login, konta);
							cout << endl << tr("Jeżeli tak - wybierz 1, nie - wybierz 0: ");
							cin >> jeszcze_raz;
							if (jeszcze_raz)
							{
								cout << tr("Dziękujemy! Teraz możesz zalogować się na swoje konto!");
								Sleep(2500);
								jeszcze_raz = 0;
							}
							else
							{
								zmiana_danych(login, konta);
							}
						}
						else
						{
							cout << tr("Coś poszło nie tak!");
							Sleep(2000);
							system("cls");
							jeszcze_raz = 1;
						}
					} while (jeszcze_raz);
				}
			} while (jeszcze_raz);
			break;
		case 3:
			//wpłata na konto
			cout << tr("Podaj ostatnie 16 cyfr nr konta, na które chcesz wpłacić pieniądze: ") << endl;
			cin >> nr_konta;
			cout << tr("Podaj kwotę, którą chcesz wpłacić [w zł]: ") << endl;
			double kwota;
			cin >> kwota;
			if (istnieje_konto_login(jaki_login("6510100002" + nr_konta, konta), konta) && jaki_login("6510100002" + nr_konta, konta) != "0")
			{
				if (kwota > 0)
				{
					czy_była_zmiana = true;
					odwołanie(jaki_login("6510100002" + nr_konta, konta), konta);
					wpłata_dowolnej_kwoty(jaki_login("6510100002" + nr_konta, konta), konta, kwota);
					cout << tr("Kwota w wysokości ") << kwota;
					cout << tr(" zł została pomyślnie wpłacona na konto o numerze ") << nr_konta << "!";
				}
				else if (kwota == 0)
				{
					cout << tr("Wpłacanie 0 zł jest bez sensu!");
				}
				else
				{
					cout << tr("Nie możesz wpłacić ujemnej kwoty!");
				}
			}
			else
			{
				cout << "Konto o podanym numerze nie istnieje!";
			}
			Sleep(3000);
			break;
		case 4:
			//panel pracownika
			do {
				cout << "Podaj login: ";
				cin >> login;
				cout << tr("Podaj hasło: ");
				cin >> hasło;
				if (login == tr("wiewiórka") && hasło == "czas")
				{
					//menu panelu pacownika
					do
					{
						jeszcze_raz = false;
						cout << "MENU" << endl;
						cout << "Wybierz: " << endl;
						cout << tr("0 - aby wyświetlić dane o wszystkich kontach;") << endl;
						cout << tr("1 - aby wyświetlić operacje wykonane w danym okresie;") << endl;
						cout << tr("2 - aby wyświetlić użytkowników, którzy mają debet na koncie;") << endl;
						cout << tr("9 - aby wyjść z MENU!") << endl;
						int wybor;
						cin >> wybor;
						system("cls");
						switch (wybor)
						{
						case 0:
							//dane wszystkich kont
							if (konta.liczba_kont != 0)
							{
								cout << "Liczba kont: " << konta.liczba_kont << endl << endl;
								wsk_konto pom = konta.pierwsze;
								for (int i = 1; i <= konta.liczba_kont; i++)
								{
									cout << "--------------------" << endl;
									cout << "Konto nr " << i << ". " << endl << endl;
									cout << tr("Czy konto zostało usunięte przez użytkownika?") << endl;
									if (pom->czy_konto_usunięte == true)
										cout << "TAK";
									else
										cout << "NIE";
									cout << endl << endl;
									dane_klienta(pom->login, konta);
									cout << "--------------------" << endl;
									pom = pom->następny;
								}
							}
							else
							{
								cout << "Brak kont w bazie danych!!" << endl;
							}
							break;
						case 1:
							//operacje w danym okresie
							datum początek;
							datum koniec;
							cout << tr("Podaj datę, od kiedy chcesz mieć wyciąg [w liczbach]: ") << endl;
							cout << tr("Dzień: ");
							cin >> początek.dzień;
							cout << tr("Miesiąc: ");
							cin >> początek.miesiąc;
							cout << tr("Rok: ");
							cin >> początek.rok;
							system("cls");
							cout << tr("Podaj datę, do kiedy chcesz mieć wyciąg [w liczbach]: ") << endl;
							cout << tr("Dzień: ");
							cin >> koniec.dzień;
							cout << tr("Miesiąc: ");
							cin >> koniec.miesiąc;
							cout << tr("Rok: ");
							cin >> koniec.rok;
							system("cls");
							if (konta.liczba_kont != 0)
							{
								wsk_konto pom = konta.pierwsze;
								for (int i = 1; i <= konta.liczba_kont; i++)
								{
									cout << "--------------------" << endl;
									wyciąg_jeden_klient(pom->login, konta, początek, koniec);
									cout << "--------------------" << endl;
									pom = pom->następny;
								}
							}
							else
							{
								cout << "Brak jakichkolwiek transakcji w bazie danych!!" << endl;
							}
							break;
						case 2:
							//użytkownicy, którzy mają debet
							if (konta.liczba_kont != 0)
							{
								wsk_konto pom = konta.pierwsze;
								int ile = 0;
								for (int i = 1; i <= konta.liczba_kont; i++)
								{
									if (pom->stan_konta < 0)
									{
										cout << "--------------------" << endl;
										dane_klienta(pom->login, konta);
										cout << "--------------------" << endl;
										ile++;
									}
									pom = pom->następny;
								}
								if (ile > 0)
								{
									cout << endl << tr("Powyżej znajdują się dane użytkowników, którzy mają debet na koncie!");
									cout << endl << tr("Ilość użytkowników z debetem: ") << ile << endl;
								}
								else
								{
									cout << tr("Brak użytkowników z debetem na koncie :)");
								}
							}
							else
							{
								cout << "Brak jakichkolwiek kont w bazie danych!!" << endl;
							}
							break;
						}
						if (wybor != 9)
						{
							cout << endl << "Wybierz: " << endl;
							cout << tr("0 - aby wrócić do menu głównego;") << endl;
							cout << tr("1 - aby wrócić do panelu pracownika i wykonać inną operację.") << endl;
							cin >> jeszcze_raz;
						}
						system("cls");
					} while (jeszcze_raz);
				}
				else
				{
					cout << tr("Błędny login lub hasło!") << endl;
					cout << "Wybierz: " << endl;
					cout << tr("1 - aby spróbować jeszcze raz;") << endl;
					cout << tr("0 - aby wrócić do głównego menu.") << endl;
					cin >> jeszcze_raz;
					system("cls");
				}
			} while (jeszcze_raz);
			break;
		}
		system("cls");
	}while (wybor);

	

	if (czy_była_zmiana == true)
	{
		zapisz_dane_do_pliku(konta);
	}

	zwolnij_pamięć(konta);
	return 0;
}

