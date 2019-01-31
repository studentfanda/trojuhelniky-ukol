#include <iostream>
#include <fstream>
#include <limits> //kvuli numeric limits

using namespace std;

struct point{  //Zakladni jednotkou je bod, obsahujici dve souradnice
   double x,y; //v souboru jsou cisla s přesnosti 7 cifer, tak je treba double
   point()
   {}
   point(double a, double b) //uz pri vytvareni muzu dosadit hodnoty
   {
       x=a;
       y=b;
   }
};

//funkce, ktera porovna aktualni bod s extremy, na ktere jsem zatim narazil a pripadne extrem prepise na sebe
void porovnani_s_extremy (point& novy, point& max, point& min)
{
   if(novy.x > max.x) max.x=novy.x;
   if(novy.y > max.y) max.y=novy.y;
   if(novy.x < min.x) min.x=novy.x;
   if(novy.y < min.y) min.y=novy.y;
}

void vypis(point& a, point& b, point& c) //jen vypise ty dva trojuhelniky
{printf ("%lf %lf %lf %lf %lf %lf\n", a.x, a.y, b.x, b.y, c.x, c.y);
}

void dvasloupce (int dim2, point prvni[], point druhy[]) //ze dvou sloupcu pod sebou postupne udela trojuhelniky
{
	for (int i = 1; i < dim2; i++)
	{vypis(prvni[i-1], prvni[i], druhy[i-1]);//tady si musim dat pozor, abych je oba vypisoval ve stejnem smeru
	 vypis(druhy[i-1], prvni[i], druhy[i]);
	}
	
}

//Nacte novy sloupec ze souboru a hned kontroluje, jestli soubor predcasne neskoncil
void napln(int pocet, point pole[], ifstream& infile, point& max, point& min)
{
   for(int i=0;i<pocet;i++) {
       infile >> pole[i].x;
       if(infile.eof())     //Kontrola vstupniho souboru - mimo tuto funkci se uz nacitat nebude
           throw 1;
       infile >> pole[i].y;				
      if(infile.eof())
            throw 1;
       porovnani_s_extremy(pole[i], max, min);
   }
}

int main (int argc, char *argv[]) {
   try {
       ifstream infile;
       //pokud program nedostane spravny pocet argumentu tj. svoji binarku a jeden vstupni soubor tak hodim vyjimku
       if (argc != 2) {
           throw 4;
       }
       infile.open(argv[1]);
       //pokud soubor z argumentu z nejakeho duvodu nejde otevrit tak hodim vyjimku
       if(!infile)
           throw 5;

       //diff jsou rozdily extremu na obou osach nactene ze souboru
       point diff;
       infile >> diff.x >> diff.y;
       //vytvorim dva specialní body, jeden obsahuje nejnizsi souradnice obou os, na ktere jsem zatim narazil
       //minima nastavim na maximalni double a maxima na minimalni, aby je body v mrizce musely nutne prepsat
       point max=point(numeric_limits<double>::lowest(),numeric_limits<double>::lowest());
       point min=point(numeric_limits<double>::max(),numeric_limits<double>::max());

       //dim jsou rozmery mrizky nactene ze souboru
		double dim1d,dim2d; // tyhle doubly jsou zbytecne, ale neslo mi to cislo nacist hned do int a už takto to posilam pozde
		int dim1, dim2;
		infile >> dim1d >> dim2d;
		//pokud dim neni cele cislo, nebo je mensi nez dva tak je tabulka zdegenerovana nebo nedefinovana tak hodim vyjimku
		if((int)dim1d!=dim1d || (int)dim2d!=dim2d || dim1d<2 || dim2d<2)
           throw 2;
		dim1=(int)dim1d;
		dim2=(int)dim2d;
       
		printf("%d\n", (dim1-1)*(dim2-1)*2);
        //Pokud je radku i sloupcu, jak bylo napsano, tak to musi vyjít takto. A nemá cenu to kontrolovat
        //protoze to by nebyla chyba vstupu (na kterou bych prisel jinde), ale matematiky.
     
       //dva sloupce, ktere budu resit
       point a[dim2], b[dim2];
       point * prvni=a;
       point * druhy=b;
       napln(dim2,prvni,infile, max, min);//napred nactu samostatne prvni radek

		for (int i = 1; i < dim1; i++) //jadro programu - tady se dela vslatne vsechno. A vybral jsem tento
		//algoritmus, abych nemusel mit v pameti vic radku a zaroven prehledne nacital cele radky 
			{
			napln(dim2,druhy,infile, max, min);
			dvasloupce(dim2, prvni, druhy);
			swap(prvni,druhy); //prohodim, abych zase nacital do prvniho
			}
			
		//kontroly rozmeru - neni nutne vypisovat, ale asi lepsi, nez shazovat cely program
		if((max.x-min.x)>diff.x)	
			printf("Pozor, rozmer x se lisil o %lf místo o %lf, coz je vic \n", (max.x-min.x), diff.x);
		
		if((max.y-min.y)>diff.y)  
			printf("Pozor, rozmer y se lisil o %lf místo o %lf, coz je vic \n", (max.y-min.y), diff.y);
			
	}
       //odchytavam vyjimky a vypisu spravnou chybovou hlasku
   catch(int x)
   {
       if(x==1)
           fputs("Neocekavany konec souboru", stderr);
       if(x==2)
           fputs("Nespravne rozmery tabulky", stderr);
       if(x==3) //tohle tu je zbytecne, nakonec to neshazuje program, ale jen vypise varovani
           fputs("Prvni radek sougoru neobsahuje spravne rozpeti", stderr);
       if(x==4)
           fputs("Na vstupu zadejte jmeno spousteneho souboru a data, tedy napr. a.out data.txt", stderr);
       if(x==5)
           fputs("Vstupni soubor nejde otevrit\n",stderr);
       return 1;
   }
}
