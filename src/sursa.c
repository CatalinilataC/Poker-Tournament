#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "info.h"
//BAJENARU CATALIN
//GRUPA 311CB

void eliberare_memorie(Sala S)
{
	TLista p = S->masa, d, aux;
	while(p)
	{
		d = ((Masa)p->info)->jucatori;
		while(d)
		{
			free(((Jucator)d->info)->nume);
			free(d->info);
			aux = d;
			d = d->urm;
			free(aux);
		}
		free(((Masa)p->info)->numeMasa);
		free(p->info);
		aux = p;
		p = p->urm;
		free(aux);
	}
}

void initsala(Sala S, FILE *conf)
{//aloca memorie pt cele 2 liste si pune info in ele	
int x, i, j, a, b;
char str[20];
fscanf(conf, "%d", &x);
S->nrMese = x;
S->nrLocCrt = S->nrLocMax = 0;
S->masa = (TLista)malloc(sizeof(struct celulag));
if(!S->masa)
{
	eliberare_memorie(S);
	return;
}
TLista p = S->masa, d;
for(i = 0; i < x; i++)
{//citire nume masa si nr curent si max de locuri + alocari
	fscanf(conf, "%s%d%d", str, &a, &b);
	p->info = (Masa)malloc(sizeof(struct masa));
	if(!p->info)
{
	eliberare_memorie(S);
	return;
}
	((Masa)p->info)->nrCrtJucatori = a;
	((Masa)p->info)->nrMaxJucatori = b;
	((Masa)p->info)->numeMasa = malloc(strlen(str) + 1);
	if(!((Masa)p->info)->numeMasa)
{
	eliberare_memorie(S);
	return;
}
	memcpy(((Masa)p->info)->numeMasa, str, strlen(str) + 1);
	((Masa)p->info)->jucatori = (TLista)malloc(sizeof(struct celulag));
		if(!((Masa)p->info)->jucatori)
{
	eliberare_memorie(S);
	return;
}
	S->nrLocMax += b;
	S->nrLocCrt += a;
	d = ((Masa)p->info)->jucatori;
	for(j = 0; j < ((Masa)p->info)->nrCrtJucatori; j++)
	{// citire nume jucator si nrmaini + alocari
		fscanf(conf, "%s%d", str, &a);
		d->info = (Jucator)malloc(sizeof(struct jucator));
			if(!d->info)
{
	eliberare_memorie(S);
	return;
}
		((Jucator)d->info)->nrMaini = a;
		((Jucator)d->info)->nume = (char*)malloc(strlen(str) + 1);
			if(!((Jucator)d->info)->nume)
{
	eliberare_memorie(S);
	return;
}
		memcpy(((Jucator)d->info)->nume, str, strlen(str) + 1);
		if(j == ((Masa)p->info)->nrCrtJucatori - 1)
			d->urm = NULL;
		else
		{//daca mai sunt de citit jucatori aloca o celula noua
			d->urm = (TLista)malloc(sizeof(struct celulag));
			if(!d->urm)
				{
					eliberare_memorie(S);
					return;
				}
			d = d->urm;
		}
	}
	if(i == x - 1)
		p->urm = NULL;
	else
	{//daca mai sunt de citit mese aloca o celula noua
		p->urm = (TLista)malloc(sizeof(struct celulag));
		if(!p->urm)
			{
				eliberare_memorie(S);
				return;
			}
		p = p->urm;
	}
}
}

void afisare(struct sala S, FILE *out)
{
	if(S.nrLocCrt == 0)
	{
		fprintf(out, "Sala este inchisa!");
		return;
	}
	TLista p = S.masa, d;
	while(p)
	{
		fprintf(out ,"%s:", ((Masa)p->info)->numeMasa);
		d = ((Masa)p->info)->jucatori;
		while(d && d->urm)
		{//ultima celula e afisata inafara acestui while
			fprintf(out, " %s - %d;",((Jucator)d->info)->nume, ((Jucator)d->info)->nrMaini);
			d = d->urm;
		}
		if(((Masa)p->info)->jucatori)
			fprintf(out, " %s - %d.\n", ((Jucator)d->info)->nume, ((Jucator)d->info)->nrMaini);
		p = p->urm;
	}
}

void eliminare(ALista a, TLista x)
{
	TLista aux;
	if( x->info == (*a)->info)
	{//daca elementul de eliminat e primul, reface capul listei + dealocare
		aux = (*a)->urm;
		free((*a)->info);
		free(*a);
		(*a) = aux;
		return;
	}
	TLista p = (*a)->urm, ant = *a;
	while(p)
	{//cauta in lista elementul de eliminat apoi il elimina + dealocare
		if(p->info == x->info)
		{
			ant->urm = p->urm;
			free(p->info);
			free(p);
			return;
		}
		p = p->urm;
		ant = ant->urm;
	}
}

void soarta(Sala S, int val, FILE* out, char nj[20], char nm[20])
{
TLista p = S->masa, d;
	while(p)
	{
		if(p->urm == NULL && strcmp(((Masa)p->info)->numeMasa, nm) != 0)
			{//daca s a ajuns la finalul listei si inca nu l a gasit
				fprintf(out, "Masa %s nu exista!\n", nm);
				return;
			}
		if(strcmp(((Masa)p->info)->numeMasa, nm) == 0)
		{//daca a gasit masa
			d = ((Masa)p->info)->jucatori;
			while(d)
			{
				if(d->urm == NULL && strcmp(((Jucator)d->info)->nume, nj) != 0)
					{//daca s a ajuns la finalul listei si inca nu l a gasit
						fprintf(out, "Jucatorul %s nu exista la masa %s!\n", nj, nm);
						 return;
					}
				if(strcmp(((Jucator)d->info)->nume, nj) == 0)
				{//daca a gasit jucatorul
					 ((Jucator)d->info)->nrMaini += val;
					 if(((Jucator)d->info)->nrMaini <= 0)
					 {//daca trebuie eliminat jucatorul
					 	free(((Jucator)d->info)->nume);
					 	eliminare(&(((Masa)p->info)->jucatori), d);
					 	S->nrLocCrt--;
					 	((Masa)p->info)->nrCrtJucatori--;
					 	if(!((Masa)p->info)->jucatori)//verifica daca mai sunt jucatori la masa
					 		{
					 			S->nrLocMax -= ((Masa)p->info)->nrMaxJucatori;
					 			free(((Masa)p->info)->numeMasa);
					 			eliminare(&(S->masa), p);
					 		}
					 }
					return;
				}
				d = d->urm;
			}
		}
		p = p->urm;
	}
}

void tura(Sala S, FILE * out, char nm[22])
{
TLista p = S->masa, d;
TLista aux, t;
	while(p)
	{
		if(p->urm == NULL && strcmp(((Masa)p->info)->numeMasa, nm) != 0)
			{
				fprintf(out, "Masa %s nu exista!\n", nm);
				return;
			}
		if(strcmp(((Masa)p->info)->numeMasa, nm) == 0)
		{//daca a gasit masa
			d = ((Masa)p->info)->jucatori;
			while(d->urm)
				d = d->urm;//ajunge la finalul listei
			if(((Masa)p->info)->jucatori->urm != NULL)
			{//face rotatia
				aux = ((Masa)p->info)->jucatori;
				((Masa)p->info)->jucatori = aux->urm;
				aux->urm = NULL;
				d->urm = aux;
			}
			d = ((Masa)p->info)->jucatori;
			while(d)
			{//parcurge lista, decrementeaza nrmaini, elimina daca e cazul
				 if(--(((Jucator)d->info)->nrMaini) <= 0)
					 {
					 	t = d->urm;
					 	free(((Jucator)d->info)->nume);
					 	eliminare(&(((Masa)p->info)->jucatori), d);
					 	S->nrLocCrt--;
					 	((Masa)p->info)->nrCrtJucatori--;
					 	if(!((Masa)p->info)->jucatori)//verifica daca mai sunt jucatori la masa
					 		{	//elimina masa
					 			S->nrLocMax -= ((Masa)p->info)->nrMaxJucatori;
					 			free(((Masa)p->info)->numeMasa);
					 			eliminare(&(S->masa), p);
					 		}
					 	d = t;
					 }
					 else d = d->urm;
			}
			return;
		}
		p = p->urm;
	}
}

int cmp(TLista a, TLista b)
{
	if(((Jucator)a->info)->nrMaini == ((Jucator)b->info)->nrMaini)
		return strcmp(((Jucator)a->info)->nume, ((Jucator)b->info)->nume);
	else return ((Jucator)a->info)->nrMaini - ((Jucator)b->info)->nrMaini;
}

void inserareordonata(ALista a, TLista x)
{
	if(*a == NULL)
	{//lista e vida
		*a = x;
		return;
	}
	if(cmp(*a, x) <= 0)
	{//inaintea capului de lista
		TLista aux = *a;
		*a = x;
		(*a)->urm = aux;
		return;
	}
	TLista p = *a, aux;
	while(p->urm)
	{//verifica pana la ultimul element
		if(cmp(p->urm, x) < 0)
		{
			aux =  p->urm;
			p->urm = x;
			x->urm = aux;
			return;
		}
		p = p->urm;
	}
	p->urm = x;
	x->urm = NULL;
}

void clasament(Sala S, FILE *out, char nm[22])
{
	TLista L = NULL, p = S->masa, d, t, x;
	while(p)
	{
		if(p->urm == NULL && strcmp(((Masa)p->info)->numeMasa, nm) != 0)
			{
				fprintf(out, "Masa %s nu exista!\n", nm);
				return;
			}
		if(strcmp(((Masa)p->info)->numeMasa, nm) == 0)
		{
			d = ((Masa)p->info)->jucatori;
			while(d)
			{//face o lista noua cu elementele in ordine
				x = malloc(sizeof(struct celulag));
				x->info = (Jucator)malloc(sizeof(struct jucator));
				((Jucator)x->info)->nume = malloc(1 + strlen(((Jucator)d->info)->nume)); 
				memcpy(((Jucator)x->info)->nume, ((Jucator)d->info)->nume, 1 + strlen(((Jucator)d->info)->nume));
				((Jucator)x->info)->nrMaini = ((Jucator)d->info)->nrMaini;
				x->urm = NULL;
				inserareordonata(&L, x);
				d = d->urm;
			}
			fprintf(out, "Clasament %s:\n", nm);
			t = L;
			while(t)
			{//afiseaza lista noua + dealoca ce a alocat pt ea
				fprintf(out, "%s %d\n", ((Jucator)t->info)->nume, ((Jucator)t->info)->nrMaini);
				x = t;
				t = t->urm;
				free(((Jucator)x->info)->nume);
				free(x->info);
				free(x);
			}
			return;
		}
		p = p->urm;
	}
}

void inchide(Sala S, char nm[22], FILE *out)
{
	TLista p = S->masa, d, it, it2;
	while(p)
	{
		if(p->urm == NULL && strcmp(((Masa)p->info)->numeMasa, nm) != 0)
			{
				fprintf(out, "Masa %s nu exista!\n", nm);
				return;
			}
		if(strcmp(((Masa)p->info)->numeMasa, nm) == 0)
		{
			if(S->nrLocMax - S->nrLocCrt - ((Masa)p->info)->nrMaxJucatori < 0)
			{
				fprintf(out, "Nu exista suficiente locuri in sala!\n");
				return;
			}
			it2 = ((Masa)p->info)->jucatori;
			for(d = S->masa; d; d = d->urm)
			{
				if(strcmp(((Masa)d->info)->numeMasa, ((Masa)p->info)->numeMasa) == 0)continue;
				it = ((Masa)d->info)->jucatori;
				while(it->urm)
					it = it->urm;//ajunge la finalul listei, unde va adauga
				while(((Masa)d->info)->nrMaxJucatori - ((Masa)d->info)->nrCrtJucatori
				 && ((Masa)p->info)->nrCrtJucatori)
				{//adauga jucatorii la mesele la care se poate
					((Masa)d->info)->nrCrtJucatori++;
					((Masa)p->info)->nrCrtJucatori--;
					it->urm = it2;
					it = it->urm;
					it2 = it2->urm;
				}
				it->urm = NULL;
				if(!((Masa)p->info)->nrCrtJucatori)
					break;
			}
			 S->nrLocMax -= ((Masa)p->info)->nrMaxJucatori;
			 free(((Masa)p->info)->numeMasa);
			 eliminare(&S->masa, p);
			 return;
		}
		p = p->urm;
	}
}

int main(int argc, char *argv[])
{
	if(argc != 4)
		return 0;
	struct sala S;
	TLista p, t;
	FILE *conf = fopen(argv[1], "rt");
	FILE *in = fopen(argv[2], "rt");
	FILE *out = fopen(argv[3], "wt");
	initsala(&S, conf);
	char str[22], b[22], a[22];
	int x;
	while(fscanf(in, "%s", str) > 0)
	{
		if(strcmp(str, "print") == 0)
			afisare(S, out);
		else
			if(strcmp(str, "noroc") == 0)
			{
				fscanf(in, "%s%s%d", a, b, &x);
				soarta(&S, x, out, b, a);
			}
			else
				if(strcmp(str, "ghinion") == 0)
				{//soarta apelata cu valoare negativa
					fscanf(in, "%s%s%d", a, b, &x);
					soarta(&S, x * (-1), out, b, a);
				}
				else
					if(strcmp(str, "tura") == 0)
					{
						fscanf(in, "%s", a);
						tura(&S, out, a);
					}
					else
						if(strcmp(str, "tura_completa") == 0)
						{
							p = S.masa;
							while(p)
							{
								t = p->urm;
								tura(&S, out, ((Masa)p->info)->numeMasa);
								p = t;
							}
						}
						else
							if(strcmp(str, "clasament") == 0)
							{
								fscanf(in, "%s", a);
								clasament(&S, out, a);
							}
							else 
								if(strcmp(str, "inchide") == 0)
								{
									fscanf(in, "%s", a);
									inchide(&S, a, out);
								}
	}
eliberare_memorie(&S);//elibereaza ce a alocat
	return 0;
}