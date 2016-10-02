#include "BlackGPIO/BlackGPIO.h"
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

/*
Essa variavel "dispensaPrimeiro" é usada para descartar a primeira leitura do sensor.
Essa leitura gera um valor muito alto que resulta em um falso positivo na lógica inicial
do algoritmo
*/

#define PATH_ADC "/sys/bus/iio/devices/iio:device0/in_voltage"

int leitorAnalogico(int number){
	stringstream ss;
   	ss << PATH_ADC << number << "_raw";
	fstream fs;
	fs.open(ss.str().c_str(), fstream::in);
	fs >> number;
	fs.close();
	return number;
}

bool verificaPresenca(int vInicial, int vFinal){
	int diferenca = vFinal - vInicial;
	if (abs(diferenca) > 100){		
		//para o loop
		return true;
	}
	else{
		//continue o loop
		return false;
	}
}

int main(int argc, char *argv[]){
    //GPIO_50 está faltando no arquivo BlackGPIO.h
    BlackLib::BlackGPIO   led1(BlackLib::GPIO_50,BlackLib::output, BlackLib::SecureMode);
    led1.setValue(BlackLib::high);

    BlackLib::BlackGPIO ledR(BlackLib::GPIO_69, BlackLib::output, BlackLib::SecureMode);
    BlackLib::BlackGPIO ledG(BlackLib::GPIO_45, BlackLib::output, BlackLib::SecureMode);
    BlackLib::BlackGPIO	ledB(BlackLib::GPIO_66, BlackLib::output, BlackLib::SecureMode);

    ledG.setValue(BlackLib::high);
    bool x = true;
    int dispensaPrimeiro = 0;

    while(x){
    	int valor0 = leitorAnalogico(1);
    	sleep(1);
    	int valor1 = leitorAnalogico(1);
    	if(verificaPresenca(valor0, valor1)){

    		dispensaPrimeiro++;
    		if(dispensaPrimeiro > 1){
    			ledG.setValue(BlackLib::low);
    			ledR.setValue(BlackLib::high);
    			sleep(5);
    			break;
    		}

    	}

    } 

    led1.setValue(BlackLib::low);
    return 0;
}


