/* 
 * File:   main.cpp
 * Author: root
 *
 * Created on December 4, 2014, 10:06 AM
 */

#include <vector>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <string.h>
#include <string>
#include <list>

using namespace std;

int numberOfLines = 0;
int numberOfLinesCabecalho = 0;
int numberOfAttacks = 0;
int numberOfNormals = 0;
vector<std::string> *vec_normal = new vector<std::string> ();
vector<std::string> *vec_attack = new vector<std::string> ();

vector<std::string> *cabecalho = new vector<std::string> ();

unsigned int countNumberOfLine(char *file) {


    std::string line;
    std::ifstream myfile(file);

    while (std::getline(myfile, line)) {
        numberOfLinesCabecalho++;
        cabecalho->push_back(line);
        
        if (line.find("@data") != string::npos) {
            break;
        }
    }


    while (std::getline(myfile, line)) {
        numberOfLines++;
        if (numberOfLines % 1000000 == 0) {
            fprintf(stdout, "%d\n", numberOfLines);
        }

        if (line.find("attack") == std::string::npos) {
            numberOfNormals++;
            vec_normal->push_back(line);
        } else {
            numberOfAttacks++;
            vec_attack->push_back(line);
        }
    }

    return numberOfLines - 1;
}

/*
 *      "/home/viegas/Desktop/saida/syn/cenario1_rem.arff_train.arff" arquivo arff
 *      
 * 
 *  
 */
int main(int argc, char** argv) {

    int qtLinhasArff = 0;
    
    //strcpy("/home/viegas/Desktop/saida/syn/cenario1_rem.arff_train.arff", "/home/viegas/Desktop/saida/syn/cenario1_rem.arff_train.arff");

    countNumberOfLine("/home/viegas/Desktop/saida/syn/cenario1_rem.arff_train.arff");

    fprintf(stdout, "Number of header Lines in ARFF(%s): %d\n", "/home/viegas/Desktop/saida/syn/cenario1_rem.arff_train.arff", numberOfLinesCabecalho);
    fprintf(stdout, "Number of lines in ARFF(%s): %d\n", "/home/viegas/Desktop/saida/syn/cenario1_rem.arff_train.arff", numberOfLines);
    fprintf(stdout, "Number of attacks: %d\n", numberOfAttacks);
    fprintf(stdout, "Number of normal: %d\n", numberOfNormals);

    fprintf(stdout, "Randomizing attack list...\n");
    std::random_shuffle(vec_attack->begin(), vec_attack->end());


    fprintf(stdout, "Randomizing normal list...\n");
    std::random_shuffle(vec_normal->begin(), vec_normal->end());


    int numberOfAttackTrain, numberOfAttackTest, numberOfAttackValidacao;
    int numberOfNormalTrain, numberOfNormalTest, numberOfNormalValidacao;


    if (numberOfAttacks < numberOfNormals) {
        fprintf(stdout, "Quantidade de normal é menor que a quantidade de ataques, calculando quantidade para treinamento/teste/validacao\n");
        numberOfAttackTrain = numberOfAttackTest = (numberOfAttacks * 0.25f);
        numberOfNormalTrain = numberOfAttackTrain;
        numberOfNormalTest = numberOfAttackTest;

    } else {
        fprintf(stdout, "Quantidade de ataque é menor que a quantidade de normal, calculando quantidade para treinamento/teste/validacao\n");
        numberOfNormalTrain = numberOfNormalTest = (numberOfNormals * 0.25f);
        numberOfAttackTrain = numberOfNormalTrain;
        numberOfAttackTest = numberOfNormalTest;
    }

    numberOfAttackValidacao = numberOfAttacks - numberOfAttackTest - numberOfAttackTrain;
    numberOfNormalValidacao = numberOfNormals - numberOfNormalTest - numberOfNormalTrain;

    fprintf(stdout, "Quantidade teste (cada classe): %d (%d total)\n", numberOfAttackTest, numberOfAttackTest * 2);
    fprintf(stdout, "Quantidade treinamento (cada classe): %d (%d total)\n", numberOfNormalTest, numberOfNormalTest * 2);

    fprintf(stdout, "Quantidade validacao ATAQUE: %d\nQuantidade validacao NORMAL: %d\n", numberOfAttackValidacao, numberOfNormalValidacao);

    fprintf(stdout, "Gerando arquivos de saida...\n");
    fprintf(stdout, "Gerando arff de treinamento...\n");



    ofstream arquivo_saida;

    char buffer[1000];

    strcpy(buffer, "/home/viegas/Desktop/saida/syn/cenario1_rem.arff_train.arff");

    arquivo_saida.open(strcat(buffer, "_train.arff"));

    for (std::vector<std::string>::iterator it = cabecalho->begin(); it != cabecalho->end(); ++it) {
        arquivo_saida << *it;
        arquivo_saida << "\n";
    }

    std::vector<std::string>::iterator it_ataque = vec_attack->begin();
    std::vector<std::string>::iterator it_normal = vec_normal->begin();
    int i = 0;

    for (;;) {
        if (numberOfAttacks > 0) {
            arquivo_saida << *it_ataque;
            arquivo_saida << "\n";
            ++it_ataque;
        }
        if(numberOfNormals > 0){
            arquivo_saida << *it_normal;
            arquivo_saida << "\n";
            ++it_normal;
        }
        i++;
        if (i == numberOfAttackTrain) {
            break;
        }
    }

    arquivo_saida.close();

    fprintf(stdout, "Arff de treinamento gerado!\n");

    fprintf(stdout, "Gerando arff de teste...\n");

    strcpy(buffer, "/home/viegas/Desktop/saida/syn/cenario1_rem.arff_train.arff");

    arquivo_saida.open(strcat(buffer, "_test.arff"));

    for (std::vector<std::string>::iterator it = cabecalho->begin(); it != cabecalho->end(); ++it) {
        arquivo_saida << *it;
        arquivo_saida << "\n";
    }

    for (;;) {
        if (numberOfAttacks > 0) {
            arquivo_saida << *it_ataque;
            arquivo_saida << "\n";
            ++it_ataque;
        }
        if (numberOfNormals > 0) {
            arquivo_saida << *it_normal;
            arquivo_saida << "\n";
            ++it_normal;
        }
        i++;
        if (i == (numberOfAttackTrain + numberOfAttackTest)) {
            break;
        }
    }

    arquivo_saida.close();

    fprintf(stdout, "Arff de teste gerado!\n");

    fprintf(stdout, "Gerando arff de validacao...\n");

    strcpy(buffer, "/home/viegas/Desktop/saida/syn/cenario1_rem.arff_train.arff");

    arquivo_saida.open(strcat(buffer, "_validacao.arff"));

    for (std::vector<std::string>::iterator it = cabecalho->begin(); it != cabecalho->end(); ++it) {
        arquivo_saida << *it;
        arquivo_saida << "\n";
    }

    if (numberOfAttacks > 0) {
        for (; it_ataque != vec_attack->end(); ++it_ataque) {
            arquivo_saida << *it_ataque;
            arquivo_saida << "\n";
        }
    }
    if (numberOfNormals > 0) {
        for (; it_normal != vec_normal->end(); ++it_normal) {
            arquivo_saida << *it_normal;
            arquivo_saida << "\n";
        }
    }

    fprintf(stdout, "Arff de validacao gerado!\n");

    fprintf(stdout, "FIM!\n");

    return 0;
}

