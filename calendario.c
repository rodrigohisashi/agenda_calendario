#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>


void menu();
void menuMarca();
void menuProcura();
void menuMostra();
void diaDeHoje();
void existeArquivo();
bool dataValida();
bool horaValida();
int diaDaSemana();
int temCompromisso();
void imprimirCalendarioMes();

struct info{
  char nome[15];
  int dias;
};
struct info criaInfo();

int main() {
  existeArquivo();
	setlocale(LC_ALL,"Portuguese");
	menu();
	return 0;
}

void diaDeHoje(
	int *diaHj,
	int *mesHj,
	int *anoHj) // 420 adicionados *diaHj, *mesHj, *anoHj
{
	struct tm *data; // ponteiro para struct que armazena data e hora
	time_t seg;		 // armazena os segundos (variavel da biblioteca time)
	time(&seg);		 // obter o tempo em seg.
	data = localtime(&seg);
	printf(
		"Data: %d/%d/%d  Horário: %d:%d\n",
		data->tm_mday,
		data->tm_mon + 1,
		data->tm_year + 1900,
		data->tm_hour,
		data->tm_min);
	*diaHj = data->tm_mday;
	*mesHj = data->tm_mon + 1;
	*anoHj = data->tm_year + 1900;
}

void menu() {
  FILE *file;
	int n, diaHj, mesHj, anoHj;
	printf("______________________________________\n");
	printf("|          Agenda Calendário          |\n");
	printf("|_____________________________________|\n\n");
  file = fopen("compromissos.txt", "r");
  
  int contador = 0;
  fseek(file, 0, SEEK_SET);
  fscanf(file, "%d", &contador);
  fclose(file);

	diaDeHoje(&diaHj, &mesHj, &anoHj); 

	printf("Você tem %d compromisso(s) marcado(s).\n\n", contador); // Colocar os compromisso aqui
	printf("Escolha uma opção: \n\n");
	printf("1 - Marcar compromisso\n");
	printf("2 - Ver compromisso\n");
	printf("3 - Mostrar Calendario\n");
	printf("4 - Finalizar programa\n\n");

	do {
		printf("Escolha uma opção válida:\n");
		scanf("%d", &n);
	} while (n > 4 || n <= 0);

	switch (n) {
	case 1:
		menuMarca(diaHj, mesHj, anoHj); 
		break;
	case 2:
		menuProcura();
		break;
	case 3:
		menuMostra();
		break;
	case 4:
		exit(1);
	}
}

void menuProcura() {
  FILE *f;
  f= fopen("compromissos.txt", "r");
  int qt;
  fscanf(f,"%d",&qt);
  int dia, mes, ano;
  printf("Qual ano você deseja?\n");
  scanf("%d", &ano);
  printf("Qual mês você deseja?\n");
	scanf("%d", &mes);
  imprimirCalendarioMes(mes,ano);
  printf("\n");
  printf("Qual dia você deseja?\n");
  scanf("%d",&dia);
  int auxAno,auxMes,auxDia,hora,minuto, contador=0;
  char compromisso[1000];
  for(int i=0;i<qt;i++)
  {
    fscanf(f,"%d %d %d %d %d \n%[^\n]s", &auxDia,&auxMes,&auxAno,&hora,&minuto,&compromisso[0]);
    if((dia==auxDia)&&(auxMes==mes)&&(auxAno=ano))
    {
      printf("______________________________\n\n");
      printf("Hora: %d:%02d\n",hora,minuto);
      printf("Compromisso: %s\n",compromisso);
      printf("\n");
      contador+=1;
    } 
  } 
  if(contador==0)printf("Você não possui compromissos nessa data\n");
   fclose(f);
  printf("\n");
  menu();
}

void menuMostra(){
  int i,j,k,ano;
  printf("Digite o ano desejado:\n");
  scanf("%d",&ano);
  printf("Ano: %d\n", ano);
  int b = diaDaSemana(1,1,ano);
  for (i=1;i<=12;i++){
    struct info mes = criaInfo(i, ano);
    printf("%s\n", mes.nome);
    printf("Dom Seg Ter Qua Qui Sex Sab\n");
    for (k=0;k<b;k++){
      printf("    ");
    }
    for (j=1;j<=mes.dias;j++){
      if(temCompromisso(j,i,ano)==0){
        printf("%3d ",j);
      }
      else{
        printf("*");
        printf("%2d ",j);
      }
      if(++k>6){
        k=0;
        printf("\n");
      }
    }
    printf("\n\n");
    b = k;
  }
  menu();
}

bool bissexto(long int *ano) {
	if (*ano % 400 == 0) {
		return true;
	} else if (*ano % 4 == 0 && *ano % 100 != 0) {
		return true;
	} else {
		return false;
	}
}

bool horaValida(int hora, int min) {
	if ((hora >= 0 && hora <= 23) && (min >= 0 && min <= 60))
		return true;
	else
		return false;
}

int diaDaSemana(int d, int m, int a) { 
  static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 }; 
  if (m<3){
    a -= 1; 
  }
  return (a + a/4 - a/100 + a/400 + t[m - 1] + d) % 7; 
}

struct info criaInfo(int numMes, int ano){
  struct info mes;
  switch (numMes){
    case 1:
        strcpy(mes.nome, "Janeiro");
        mes.dias = 31;
        break;
    case 2:
        if (ano%400==0 || (ano%4==0 && ano%100!=0)){
            strcpy(mes.nome,"Fevereiro");
            mes.dias = 29;
        }
        else{
            strcpy(mes.nome,"Fevereiro");
            mes.dias = 28;
        }
        break;
    case 3:
        strcpy(mes.nome, "Março");
        mes.dias = 31;
        break;
    case 4:
        strcpy(mes.nome, "Abril");
        mes.dias = 30;
        break;    
    case 5:
        strcpy(mes.nome, "Maio");
        mes.dias = 31;
        break;
    case 6:
        strcpy(mes.nome, "Junho");
        mes.dias = 30;
        break;
    case 7:
        strcpy(mes.nome, "Julho");
        mes.dias = 31;
        break;
    case 8:
        strcpy(mes.nome, "Agosto");
        mes.dias = 31;
        break;
    case 9:
        strcpy(mes.nome, "Setembro");
        mes.dias = 30;
        break;
    case 10:
        strcpy(mes.nome, "Outubro");
        mes.dias = 31;
        break;
    case 11:
        strcpy(mes.nome, "Novembro");
        mes.dias = 30;
        break;
    case 12:
        strcpy(mes.nome, "Dezembro");
        mes.dias = 31;
        break;
    }
    return mes;
}


void existeArquivo(){
    FILE *file;
    if ((file = fopen("compromissos.txt", "r"))){
        //fprintf(file,"%d\n", 0);
        fclose(file);
        return;
    }
    file = fopen("compromissos.txt", "w");
    fprintf(file,"%d\n", 0);
    fclose(file);
}

void imprimirCalendarioMes(int m, int ano){
  int j,k;
  printf("Mes: %d Ano: %d\n", m, ano);
  int priDia = diaDaSemana(1,m,ano);
  struct info mes = criaInfo(m, ano);
  printf("%s\n", mes.nome);
  printf("Dom Seg Ter Qua Qui Sex Sab\n");
  for (k=0;k<priDia;k++){
    printf("    ");
  }
  for (j=1;j<=mes.dias;j++){
    if(temCompromisso(j,m,ano)==0){
      printf("%3d ",j);
    }
    else{
      printf("*");
      printf("%2d ",j);
    }
    if(++k>6){
      k=0;
      printf("\n");
    }
  }
  printf("\n\n");
}


int temCompromisso(int dia, int mes, int ano) {
  FILE *f;
  f= fopen("compromissos.txt", "r");
  int qt;
  fscanf(f,"%d",&qt);
  int auxAno,auxMes,auxDia,hora,minuto, contador=0;
  char compromisso[1000];
  for(int i=0;i<qt;i++)
  {
    fscanf(f,"%d %d %d %d %d \n%[^\n]s", &auxDia,&auxMes,&auxAno,&hora,&minuto,&compromisso[0]);
    if((dia==auxDia)&&(auxMes==mes)&&(auxAno==ano))
    {
      return 1;
    } 
  } 
  return 0;
  fclose(f);
}

void menuMarca(int diaHj, int mesHj, int anoHj) {
	FILE *file;
	file = fopen("compromissos.txt", "r+");
	printf("______________________________________\n");
	printf("|       Agendar um compromisso        |\n");
	printf("|_____________________________________|\n\n");
	int dia, mes, ano, hora, min;
	char frase[50];

	do {
		printf("\nDigite a data a ser marcada:\n");
		scanf("%d %d %d", &dia, &mes, &ano);
		if (!(dataValida(dia, mes, ano, diaHj, mesHj, anoHj))) {
			printf("\nData inválida!\nDigite novamente uma data válida\n");
			continue;
		}
		printf("\nDigite a hora a ser marcada: \n");
		scanf("%d %d", &hora, &min);
		if (!(horaValida(hora, min))) {
			printf("\nHora inválida!\nDigite novamente uma Hora válida\n");
			continue;
		}

		printf("\nDigite o compromisso a ser marcado:\n ");
		scanf("\n%[^\n]s", frase); 
	} while (!(dataValida(
		dia, mes, ano, diaHj, mesHj, anoHj)) || !(horaValida(hora, min)));
  fseek(file, 0, SEEK_END);
  fprintf(file, "%d %d %d %d %d\n", dia, mes, ano, hora, min);
  fseek(file, 0, SEEK_END);
  fprintf(file, "%s\n", frase);
	//fclose(file);
  //file = fopen ("compromissos.txt","r");
  int contador = 0;
  fseek(file, 0, SEEK_SET);
  fscanf(file, "%d", &contador);
  fseek(file, 0, SEEK_SET);
  contador++;
  fprintf(file, "%d\n", contador);
  fclose(file);
  
  printf("\nCompromisso adicionado com sucesso!\n");
  menu();

}

bool dataValida(
  int dia,
  int mes,
  long int ano,
  int diaHj,
  int mesHj,
  int anoHj) {
  if (ano > anoHj) {
    if (mes > 12 || mes < 1) {
      return false;
    } else {
      if ((dia >= 1 && dia <= 31) &&
        (mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 ||
          mes == 10 || mes == 12)) {
        return true;
      } else if (
        (dia >= 1 && dia <= 30) &&
        (mes == 4 || mes == 6 || mes == 9 || mes == 11)) {
        return true;
      } else if (
        (dia >= 1 && dia <= 28) &&
        (mes == 2 && bissexto( & ano) == false)) {
        return true;
      } else if (
        (dia >= 1 && dia <= 29) &&
        (mes == 2 && bissexto( & ano) == true)) {
        return true;
      } else {
        return false;
      }
    }
  } else if (ano == anoHj && mes > mesHj) {

    if (mes > 12 || mes < 1) {
      return false;
    } else {
      if ((dia >= 1 && dia <= 31) &&
        (mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 ||
          mes == 10 || mes == 12)) {
        return true;
      } else if (
        (dia >= 1 && dia <= 30) &&
        (mes == 4 || mes == 6 || mes == 9 || mes == 11)) {
        return true;
      } else if (
        (dia >= 1 && dia <= 28) &&
        (mes == 2 && bissexto( & ano) == false)) {
        return true;
      } else if (
        (dia >= 1 && dia <= 29) &&
        (mes == 2 && bissexto( & ano) == true)) {
        return true;
      }
    }
  }
  else {
    if (ano == anoHj && mes == mesHj && dia >= diaHj) {

      if (mes > 12 || mes < 1) {
        return false;
      } else {
        if ((dia >= 1 && dia <= 31) &&
          (mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 ||
            mes == 10 || mes == 12)) {
          return true;
        } else if (
          (dia >= 1 && dia <= 30) &&
          (mes == 4 || mes == 6 || mes == 9 || mes == 11)) {
          return true;
        } else if (
          (dia >= 1 && dia <= 28) &&
          (mes == 2 && bissexto( & ano) == false)) {
          return true;
        } else if (
          (dia >= 1 && dia <= 29) &&
          (mes == 2 && bissexto( & ano) == true)) {
          return true;
        } else {
          return false;
        }
      }
    } else
      return false;
  }
  return false;
}
