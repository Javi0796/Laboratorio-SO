#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int esUnico(char *palabraS, char *wordA){
	if(strcmp((palabraS),wordA)==0){
		return false;
	}
	return true;
} 

void mensaje(){
	printf("Error, datos ingresados incorrectamente");
}

int main(int argc, char*argv[]){
	char *palabra=(char*)malloc(2100*sizeof(char));
	if(argc < 2) {
		printf("You must specify a filepath\n");
		return EXIT_FAILURE;
	}

	FILE* fp = fopen(argv[1], "r");
	FILE* solucion;


	if(!fp){
		printf("Error opening the file %s\n", argv[1]);
	}

	solucion = fopen("output.txt", "w");

	if(solucion == NULL){
		printf("Error, no se ha podido crear el fichero");
		return EXIT_FAILURE;
	}

	char line[1024];
	int linecount = 0;
	int quantities[4];
	int termscount = 0;
	int numingredients = 0;
	int numIngDif = 0;
	char *word;
	char *rest = fgets(line, 1024, fp);
	//numero de pedidos y diferentes numeros de platos
	while((word = strtok_r(rest, " ", &rest))){
		quantities[termscount] = atoi(word);
		termscount++;
	}

	int d = 0;
	for(int i=0; i<4;i++){
		if(quantities[i]=='\0'){
			mensaje();
			return EXIT_FAILURE;
		}
		if(i>0){
			int auxliar = i+1;
			d= d + quantities[i]*auxliar;
		}

	}
	if(d!=quantities[0]){
		mensaje();
		return EXIT_FAILURE;
	}

	termscount = 0;
	int cantidadP = quantities[0];
	int numing[quantities[0]];
	char* plato = (char*)malloc(cantidadP*2100*sizeof(char));
	while(fgets(line, 1024, fp)){	
		rest = line;	
		word = strtok_r(rest, " ", &rest);
		numingredients = atoi(word);
		numing[termscount] = numingredients;		
		//Lectura y guardado de ingredientes
		for(int i=0; i<numingredients; i++){		
			word = strtok_r(rest, " ", &rest);
			if(i == numingredients-1){
				char *s=word;
				while(*s!='\n'){
					++s;
				}
				*s='\0';
			}
			int creator = 0;
			for(int k=0; k<numIngDif;k++){
				if(!esUnico((palabra+k*21),word)){
					creator = 1;
				}
			}
			if(creator == 0){
				strcpy((palabra + (numIngDif*21)),word);
				numIngDif++;
			}

			int sum = 0;
			for(int j =0; j<termscount; j++){
					sum = sum + numing[j];
			}

			int pos = sum*21 + i * 21;
			strcpy(plato + pos, word);
		}

		termscount++;
	}


	/*Creacion matriz
	  CantidadaP = Numero de platos
	  numIngDif = Numero de ingredientes diferentes */
	int matriz[cantidadP][numIngDif];
	for(int i=0; i<numIngDif; i++){
		for(int k=0; k<cantidadP; k++){
			int unico = 0;
			for(int j=0; j<numing[k]; j++){
				int sum =0;
				for(int aux =0; aux<k; aux++){
					sum = sum + numing[aux];
				}

				int pos = j*21 + sum*21;
				if(!esUnico((plato+pos),palabra+(i*21))){
					unico = 1;
					break;
				}
			}
			if(unico == 1){
				matriz[k][i] = 1;
			}
			else {
				matriz[k][i] = 0;
			}

		}


	} 

	//Impresion matriz plato x ingrediente unico
	for(int i=0; i<cantidadP; i++){
		for(int j=0; j<numIngDif; j++){
			char aux = matriz[i][j] + '0';
			fputc(aux,solucion);
			fputc(' ',solucion);
		}
		fputc('\n',solucion);
	} 

	char* pedido = (char*)malloc(cantidadP*2100*sizeof(char));
	fputc('\n',solucion);
	fputs("La cantidad de ingredientes diferentes es: ", solucion);
	char nIng[2];
	sprintf(nIng, "%d", numIngDif);
	fputs(nIng, solucion);
	fputc('\n', solucion);
	fputc('\n', solucion);

	//Impresion de cada pedido dependiendo del numero de platos en la orden
	int sumatoria =0;
	int final =0;
	int ingAdd = 0;
	int nP =0;
	for(int i=1; i<4; i++){
		for(int j=0; j<quantities[i]; j++){
			nP++;
		}
	}

	int ingUnicos[nP];
	int ingTotal[nP];
	for (int i = 0; i < nP; ++i)
	{
		ingUnicos[i] = 0;

	}
	int q = 0;
	for(int j = 1 ; j< 4; j++){
		for(int i = 0; i<quantities[j]; i++){
			ingTotal[q]=j+1;
			q++;
		}		
	}
	int peds = 0;
	for(int i=0; i<nP; i++){
		int operador = 0;
		sumatoria = sumatoria + ingTotal[i];
		
		while(final<sumatoria){
			for(int j = 0; j<numing[final]; j++){
				int sum = 0;
				for(int k = 0; k<final; k++){
					sum = sum + numing[k];
				}
				int pos = j*21 +sum*21;
				int comparador = 0;
				int posPed = ingAdd*21 - operador*21;
				for(int x=0; x<operador;x++){
					if(!esUnico(pedido + posPed,plato + pos)){
						comparador = 1;
					}
					posPed = posPed + 21;
				} 

				if(comparador == 0){
					strcpy(pedido+posPed, plato+pos);
					operador++;
					ingAdd++;
					ingUnicos[i]++;
				}


			}
			final++;
			
		}

	}

	final = 0;
	for (int i = 0; i < nP; ++i)
	{
		int nPed = i +'0';
		fputs("El pedido ", solucion);
		fputc(nPed, solucion);
		fputs(" contiene: ",solucion);

		for(int j=0; j < ingUnicos[i]; j++){
			int sum = 0;
			for(int k=0; k<i; k++){
				sum = sum + ingUnicos[k];
			}
			int pos = sum*21 + j*21;
			fputs(pedido + pos, solucion);
			fputs(", ", solucion);
		}
		fputc('\n', solucion);
	}

	fclose(fp);
	fclose(solucion);
}


