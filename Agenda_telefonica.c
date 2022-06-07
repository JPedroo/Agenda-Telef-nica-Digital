#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> //Utilizamos esta funcao para que o computador atualize e gere um numero randomico de maneira que deia problemas em outras maquinas.

#define ARQ_LOGIN                "admin.txt" //Se precisar alterar o nome do arquivo de login
#define ARQ_AGENDA_TELEFONICA    "Agenda.txt" //Se precisar alterar o nome do arquivo da agenda
#define ARQ_TEMPORARIO           "temp.txt" //Este é um arquivo auxiliar que serve para armazenar os dados da agenda afins de manter salvo os contatos para que possa ser realizado a exclusao de determinados contatos sem excluir de uma vez todos eles.

//Struct aniversario -> niver
struct aniversario {
    int dia;
    int mes;
    int ano;
};

//Struct telefone -> tel
struct telefone {
    int DDD;
    long int numero;
};

/*Struct que define toda a agenda*/
//Struct agenda -> apelido "a"
typedef struct {
    int registro;
    char nome[100];
    char sexo[8];
    int idade;
    char cpf[12];
    char endereco[50]; //Pensamos em fazer uma struct para o endereco, entretanto, ficaria muito extenso e complexo para o usuario digitar
    struct telefone tel; //struct composta por DDD + numero
    struct aniversario niver; //struct composta por DD MM AAAA
} agenda;

//PROTOTIPOS DAS FUNCOES UTILIZADAS
void menu_principal();
void inicializacao();
void valida_login();
void carregar_arquivo();
void verifica_abertura();
void add_contato();
void busca_contato();
void exibe_agenda();
void exclui_contato();
void altera_contato();
void lixera();

//MAIN
int main() {
    inicializacao();
    return 0;
}

//INICIALIZACAO
/* Proteção EXTRA para deixar mais o prototipo mais completo*/
void inicializacao() {
    valida_login();
    menu_principal();
}

//VALIDA LOGIN
/* Proteção EXTRA para deixar mais completo. Sendo que o usuario somente podera ter acesso se realizar o login com as credencias corretas. */
void valida_login(){
    system("cls");
    printf("Acesso restrito! Realize o LOGIN p/continuar.\n");
    char usuario_digitado[32],senha_digitada[32];
    char user_validar[32],password_validar[32];
    char conteudo[100];

    FILE *arquivo = fopen("login.txt","r");

    if (arquivo == NULL){
        printf("Arquivo nÃ£o pode ser aberto\n");
        system("pause");
        exit(1);
    }

    printf("\nUsuario: ");
    scanf("%s",usuario_digitado);
    printf("Senha: ");
    fflush(stdin);
    scanf("%s",senha_digitada);

    if(arquivo != NULL){
        while ((fscanf(arquivo,"usuario:%s\nsenha:%s\n",user_validar, password_validar))!=EOF){   
            if(strcmp(usuario_digitado,user_validar)==0 && strcmp(senha_digitada,password_validar)==0){
                printf("\nUsuario e senha CORRETOS :D\n");
                fclose(arquivo);
            }else{
                printf("\nUsuario e senha INCORRETOS D:\n");
                printf("\nPressione qualquer tecla para prosseguir...\n");
                getch();
                main();
            }
        }
    }
}

//MENU PRINCIPAL
/* Aqui é o inicio do sistema no qual mostra as opções e recursos que o administrador terá em mãos. */
void menu_principal() {
    FILE *f; int atrib=0,opcao=0;
    system("cls");
    printf("\t\t\tBEM VINDO AO SISTEMA DA AGENDA TELEFONICA");
    printf("\n========DIGITE O NUMERO REFERENTE A OPCAO DESEJADA PARA MANIPULACAO DO SISTEMA========");
    printf("\n\n1. Adicionar Cadastro \n2. Exibir Agenda \n3. Pesquisar Contato \n4. Modificar Contato \n5. Excluir contato \n6. Limpar a lista \n7. Sair\n\nDigite sua escolha: ");

    switch (getch()){
    case '1': //Adicionar cadastro
        system("cls");
        atrib = 1; // -> AB+ = Este valor atributo serve de auxilio no switch da funcao carregar_arquivo para saber qual modo o arquivo sera aberto.
        carregar_arquivo(f,atrib,ARQ_AGENDA_TELEFONICA,opcao); //Executa a funcao de carregar o arquivo trazendo consigo todos os valores necessarios para a execucao.
        break;
    case '2': //Exibir agenda
        system("cls");
        opcao = 2; //Os case 2,3 e 5 possuem os mesmos atributos, logo, para identificar as mensagends erro ao abrir o arquivo a variavel opcao armazena o valor de opcao que o usuario escolheu para sabermos qual funcao que deu erro.
        atrib = 2; // -> RB
        carregar_arquivo(f,atrib,ARQ_AGENDA_TELEFONICA,opcao);
        break;
    case '3': //Pesquisar contato
        system("cls");
        opcao = 3;
        atrib = 2; // -> RB
        carregar_arquivo(f,atrib,ARQ_AGENDA_TELEFONICA,opcao);
        break;
    case '4': //Alterar contato
        opcao = 4;
        atrib = 3; // -> RB+
        system("cls");
        carregar_arquivo(f,atrib,ARQ_AGENDA_TELEFONICA,opcao);
        break;
    case '5': //Excluir contato
        system("cls");
        opcao = 5;
        atrib = 2; // -> RB + WB+ (wb+ por conta do arquivo temporario necessario para servir como auxilio na hora de excluir um determinado contato da agenda como dito no inicio do codigo).
        carregar_arquivo(f,atrib,ARQ_AGENDA_TELEFONICA,opcao);
        break;
    case '6': //Limpar a lista
        opcao = 6;
        atrib = 4;
        system("cls");
        carregar_arquivo(f,atrib,ARQ_AGENDA_TELEFONICA,opcao);
        break;
    case '7': //Sair
        system("cls");
        printf("\n========OBRIGADO POR USAR O SISTEMA - VOLTE SEMPRE!========\n\n");
        exit(0);
        break;
    default:
        system("cls");
        printf("\n Por favor, insira apenas de 1 a 7.");
        printf("\n Insira qualquer tecla para prosseguir...");
        getch();
        menu_principal();
    }
    system("cls");
    menu_principal();
}

//CARREGAR_ARQUIVO
/* Carrega o arquivo tal que o modo que ele sera aberto sera definido de acordo com o valor de atributo definido no switch case do menu principal.*/
void carregar_arquivo(FILE *f,int atributo,const char *arquivo,int opcao){
    switch (atributo){
    case 1:
        f = fopen(ARQ_AGENDA_TELEFONICA,"ab+");//ARQ_AGENDA_TELEFONICA foi definido no inicio do codigo. Abre o arquivo "Agenda.txt" no modo "ab+" que possibilita anexar novos dados em um arquivo binario para leitura e gravacao.
        verifica_abertura(f,atributo); //Inicia a funcao de verificar abertura do arquivo para saber se está tudo OK.
    case 2:
        f = fopen(ARQ_AGENDA_TELEFONICA,"rb"); //Abre o arquivo "Agenda.txt" no modo "rb" que abre arquivo binário para leitura.
        verifica_abertura(f,atributo,opcao); //Executa a funcao verificar para dois propositos: verificar qual das funcoes irao ser priorizadas por conta do atributo e tambem saber se terao erro ao abrir o arquivo.
    case 3:
        f = fopen(ARQ_AGENDA_TELEFONICA,"rb+"); //Abre o arquivo "Agenda.txt" no modo "rb+" que possibilita abrir arquivo binário para leitura/gravacao.
        verifica_abertura(f,atributo); //Inicia a funcao de verificar abertura do arquivo para saber se está tudo OK.
    case 4:
        f = fopen(ARQ_AGENDA_TELEFONICA,"w"); //Abre o arquivo "Agenda.txt" no modo "rb+" que possibilita abrir o arquivo binário para escrita.
        verifica_abertura(f,atributo); //Inicia a funcao de verificar abertura do arquivo para saber se está tudo OK.
    }
}



//VERIFICA_ABERTURA
/* Verifica se o arquivo foi aberto/lido corretamente, e tambem é uma funcao responsavel por executar as escolhas que o usuario definiu no menu principal.
   O Switch nessa funcao serve para saber qual das funcoes que esta abrindo o arquivo no modo rb, pois no codigo existem 3 casos onde o arquivo vai
   ser aberto neste modo -> Exibe_agenda, Busca_contato e Exclui_contato. Com isso, nos conseguimos enviar mensagens mais especificas sobre o erro
   e ate mesmo susgestoes tais como "Possivel solucao" para que o usuario consiga resolver o problema ao executar o codigo.*/
void verifica_abertura(FILE *f, int atributo,int opcao){ // *Lembrando que no menu principal, os arquivos nos cases 2,3 e 5 são abertos usando o mesmo atributo -> RB.*/
    switch (atributo){
    case 1: //MODO AB+
        if(f == NULL){
            printf("\n Erro na criação do arquivo Agenda.txt");
            system("pause");
            exit(1);
        }
        add_contato(f); //EXECUTAR FUNÇÃO DE ADICIONAR NOVO CONTATO.
    case 2: //MODO RB
        switch (opcao){
        case 2:
            if (f == NULL){
                printf("\n Erro de abertura do arquivo Agenda.txt");
                printf("\n Nao foram encontrados nenhum usuario cadastrado para exibi-los...\n");
                printf("\n Possivel solucao -> O arquivo Agenda.txt nao existe ainda, para cria-lo\n INSIRA um NOVO contato a partir do menu inicial para gerar o arquivo.\n\n");
                system("pause");
                menu_principal();
            }
            exibe_agenda(f); //EXECUTAR FUNÇÃO DE EXIBE AGENDA.
        case 3: //MODO RB
            if (f == NULL){
                printf("\n Erro de abertura do arquivo Agenda.txt");
                printf("\n Nao foram encontrados nenhum usuario cadastrado para buscar...\n");
                printf("\n Possivel solucao -> O arquivo Agenda.txt nao existe ainda, para cria-lo\n INSIRA um NOVO contato a partir do menu inicial para gerar o arquivo.\n\n");
                system("pause");
                menu_principal();
            }
            busca_contato(f); //EXECUTAR FUNÇÃO BUSCA CONTATO.
            break;
        case 5: //MODO RB
            if (f == NULL){
                printf("\n DADOS DO CONTATO AINDA NAO FORAM ADICIONADOS NO ARQUIVO Agenda.txt");
                printf("\n Nao foram encontrados nenhum usuario cadastrado para excluir...\n");
                printf("\n Possivel solucao -> O arquivo Agenda.txt nao existe ainda, para cria-lo\n INSIRA um NOVO contato a partir do menu inicial para gerar o arquivo.\n\n");
                system("pause");
                menu_principal();
            }
            exclui_contato(f); //EXECUTAR FUNÇÃO EXCLUI CONTATO.
            break;
        }
    case 3: //MODO WB+
        if (f == NULL) {
            printf("\n DADOS DO CONTATO AINDA NAO FORAM ADICIONADOS NO ARQUIVO Agenda.txt");
            printf("\n Nao foram encontrados nenhum usuario cadastrado para modificar...\n");
            printf("\n Possivel solucao -> O arquivo Agenda.txt nao existe ainda, para cria-lo\n INSIRA um NOVO contato a partir do menu inicial para gerar o arquivo.\n\n");
            system("pause");
            menu_principal();
        }
        altera_contato(f); //EXECUTAR FUNÇÃO ALTERA CONTATO.
        break;
    case 4: //MODO W
        if (f){
            printf("\nArquivo aberto com sucesso!\n");
            fclose(f);
            // tenta remover o arquivo agenda.txt
            printf("\nRetorno: %d\n", remove(ARQ_AGENDA_TELEFONICA));
            printf("Arquivo removido com sucesso!\n\n");
            system("pause");
            menu_principal();
        }
        lixeira(f);
    default:
        break;
    }
}

//ADD_CONTATO
/* Realiza o cadastro de usuarios, sendo que, a quantidade de cadastro é definida pelo usuario e também o codigo de registro é gerado aleatoriamente.*/
void add_contato(FILE *f) {
    srand(time(NULL)); //Para evitar de repetir um numero gerado aleatoriamente, inicializar o seed aleatória em cada execução do código para obter um resultado aleatório diferente por srand. Usando o tempo (NULL) para definir uma semente diferente de aleatório através do srand.
    agenda a;

    int i,n;
    printf (" Quantos cadastros deseja-se realizar? Digite 0 para cancelar --> ");
    scanf("%d",&n);
    system ("CLS");

    printf("========AO DIGITAR NAO COLOQUE CARACTERES ESPECIAIS OU ACENTO========");

    //variavel i serve como contador para o for, esse que estabelece quantas vezes a solicitacao dos dados de cadastro ira se repetir.
    for (i=0;i<n;i++){
        printf ("\nCadastro numero ");
	    printf ("[%d] <=> Restam ainda [%d] para serem realizados.\n",i+1,n-i);

        a.registro= 0 + (rand() % 100); //Gera um codigo de restrito randomizado.
        printf("\n Numero de registro gerado automaticamente: %d",a.registro);

        printf("\n Insira o nome: ");
        fflush(stdin);
        gets(a.nome);
        printf(" Insira o sexo (M)asculino (F)eminino: ");
        fflush(stdin);
        gets(a.sexo);
        printf(" Data de nascimento DD MM AAAA: ");
        fflush(stdin);
        scanf("%d %d %d", &a.niver.dia, &a.niver.mes, &a.niver.ano);
        a.idade = 2022 - a.niver.ano;
        printf(" Sua idade foi calculada automaticamente: %d anos.", a.idade);
        printf("\n Insira o CPF: ");
        fflush(stdin);
        gets(a.cpf);
        printf(" Insira o endereco: ");
        fflush(stdin);
        gets(a.endereco);
        printf(" Insira o telefone DDD XXXXXXXXX: ");
        fflush(stdin);
        scanf("%d %ld", &a.tel.DDD, &a.tel.numero);

        fwrite(&a, sizeof(a), 1, f); //Escreve todos os dados no arquivo agenda.txt.

        fflush(stdin);
        printf("\n Contato salvo");
        printf("\n\n Pressione qualquer tecla para continuar...");
        getch();
    }

    fclose(f);
    system("cls");
    menu_principal();
}

//EXIBE_AGENDA
/* Exibe a agenda de usuario para usuario, ou seja, se houver 4 usuarios cadastrados, toda vez que o utilizador apertar uma tecla, a tela irá
   atualizar e mostrará o proximo usuario na tela até chegar no final do arquivo.*/
void exibe_agenda(FILE *f) {
    agenda a;
    int indicador=0;
    while (fread(&a, sizeof(a), 1, f) == 1) //Faz a leitura do arquivo até chegar no final.
    {
        indicador++;
        system("cls");
        printf("\n As informacoes do contato [%i] sao: \n", indicador);
        printf("\n Registro: %d\n Nome: %s\n Sexo: %s\n Data de nascimento: %d/%d/%d\n Idade: %d anos\n CPF: %s\n Endereco: %s\n Telefone: (%d) %ld\n", a.registro, a.nome, a.sexo, a.niver.dia, a.niver.mes, a.niver.ano, a.idade, a.cpf, a.endereco, a.tel.DDD, a.tel.numero);
        printf("\n Insira qualquer tecla para prosseguir para o proximo usuario da lista...");
        getch();
        system("cls");
    }
    fclose(f);
    system("cls");
    printf("\n Fim da lista! Adicione mais contatos para aparecerem aqui.");
    printf("\n Insira qualquer tecla para continuar...");
    getch();
    menu_principal();
}

//BUSCA_CONTATO
/* Realiza a busca de um determinado usuario atráves do nome ou do registro que havia sido gerado aleatoriamente quando se foi adiciona-lo na lista.*/
void busca_contato(FILE *f){ //Mesma coisa de Exibi_Contato();
    agenda a;
    char nome[100];
    int registro;

    printf("\n Voce deseja pesquisar o contato pelo (n)ome ou (r)egistro? ");
    fflush(stdin);
    switch (getch()){
        case 'N':
        case 'n':
            printf("\n Digite o nome da pessoa a ser pesquisada: ");
            fflush(stdin);
            gets(nome);
            while (fread(&a, sizeof(a), 1, f) == 1) {
                if (strcmp(a.nome, nome) == 0) { //Realiza a comparação de duas strings...
                    printf(" Informacoes detalhadas sobre %s\n", nome);
                    printf("\n Registro: %d\n Nome: %s\n Sexo: %s\n Data de nascimento: %d/%d/%d\n Idade: %d anos\n CPF: %s\n Endereco: %s\n Telefone: (%d) %ld\n", a.registro, a.nome, a.sexo, a.niver.dia, a.niver.mes, a.niver.ano, a.idade, a.cpf, a.endereco, a.tel.DDD, a.tel.numero);
                    fclose(f);
                    printf("\n Pressione qualquer tecla para voltar ao menu...");
                    getch();
                    system("cls");
                    menu_principal();
                }else{
                    printf("\n Usuario nao encontrado.");
                    fclose(f);
                }
            }
            fclose(f);
            printf("\n Pressione qualquer tecla...");
            getch();
            system("cls");
            menu_principal();
        case 'R':
        case 'r':
            printf("\n Digite o registro gerado aleatoriamente da pessoa a ser pesquisada: ");
            fflush(stdin);
            scanf("%d",&registro);
            while (fread(&a, sizeof(a), 1, f) == 1) {
                if (registro == a.registro) { //Realiza a comparação de dois valores inteiros...
                    printf("\n Informacoes detalhadas sobre o registro N°%i\n", registro);
                    printf("\n Registro: %d\n Nome: %s\n Sexo: %s\n Data de nascimento: %d/%d/%d\n Idade: %d anos\n CPF: %s\n Endereco: %s\n Telefone: (%d) %ld\n", a.registro, a.nome, a.sexo, a.niver.dia, a.niver.mes, a.niver.ano, a.idade, a.cpf, a.endereco, a.tel.DDD, a.tel.numero);
                    fclose(f);
                    printf("\n Pressione qualquer tecla para voltar ao menu...");
                    getch();
                    system("cls");
                    menu_principal();
                }else{
                    printf("\n Usuario nao encontrado.");
                    fclose(f);
                }
            }
            fclose(f);
            printf("\n Pressione qualquer tecla para voltar ao menu");
            getch();
            system("cls");
            menu_principal();
        default:
            system("cls");
            printf("\n Por favor, insira apenas 'n' ou 'r'");
            printf("\n Insira qualquer tecla para prosseguir...");
            getch();
            system("cls");
            busca_contato(f);
    }
}

//EXCLUI_CONTATO
/* São abertos dois arquivos, sendo eles a agenda principal e o arquivo temporario para armazenar o contato que será excluido de forma com que,
   não sejam excluidos todos os contatos, e sempre que o processo é finalizado, o arquivo temp será removido para não causar poluição.*/
void exclui_contato(FILE *f){
    agenda a;
    int flag;
    char nome[100];

    FILE *ft;
    //Abre o arquivo "Agenda.txt" no modo "wb+" que possibilita criar arquivo binário para leitura/gravacao.
    ft =fopen(ARQ_TEMPORARIO,"wb+"); //Executa a funcao de carregar o arquivo trazendo consigo todos os valores necessarios para a execucao.

    if (ft == NULL){
        printf("\n Erro na criacao do arquivo temporario.");
    }else{
        printf("\n Insira o NOME de contato para ser apagado: ");
        fflush(stdin);
        gets(nome);
        while (fread(&a, sizeof(a), 1, f) == 1) {
            if (strcmp(a.nome, nome) != 0)
                fwrite(&a, sizeof(a), 1, ft);
                    if (strcmp(a.nome, nome) == 0)
                        flag = 1;
        }
        fclose(f);
        fclose(ft);
        if (flag != 1) {
            printf("\n NAO FOI ENCONTRADO NENHUM REGISTRO DESTE USUARIO.");
            remove(ARQ_TEMPORARIO);
        }else{
            remove(ARQ_AGENDA_TELEFONICA);
            rename(ARQ_TEMPORARIO,ARQ_AGENDA_TELEFONICA);
            printf("\n REGISTRO APAGADO COM SUCESSO.");
        }
    }
    printf("\n Pressione qualquer tecla...");
    getch();
    system("cls");
    menu_principal();
}

//ALTERA_CONTATO
/* A agenda recebe dois novos parametros, sendo que a agenda "s" serve como auxilio para realizar a substituição dos dados na agenda principal.
   Logo, trata-se de uma transcrição de arquivos da agenda 's' para a genda principal 'a'. */
void altera_contato(FILE *f){
    agenda a, s;
    int flag = 0;
    char nome[50];

    system("cls");
    printf("\n Digite o NOME DO CONTATO PARA MODIFICAR: ");
    fflush(stdin);
    gets(nome);

    while (fread(&a, sizeof(a), 1, f) == 1){
        if (strcmp(nome, a.nome) == 0) {
            printf(" Digite as novas informacoes do contato abaixo:\n");
            s.registro = a.registro;
            printf("\n Numero de registro (IMUTAVEL): %d",s.registro);
            printf("\n Insira o NOVO nome: ");
            fflush(stdin);
            gets(s.nome);
            printf(" Insira o sexo (M)asculino (F)eminino: ");
            fflush(stdin);
            gets(s.sexo);
            printf(" Data de nascimento DD MM AAAA: ");
            fflush(stdin);
            scanf("%d %d %d", &s.niver.dia, &s.niver.mes, &s.niver.ano);
            s.idade = 2022 - s.niver.ano;
            printf(" Sua idade foi calculada automaticamente: %d anos", s.idade);
            printf("\n Insira o CPF: ");
            fflush(stdin);
            gets(s.cpf);
            printf(" Insira o endereco: ");
            fflush(stdin);
            gets(s.endereco);
            printf(" Insira o telefone DDD XXXXXXXXX: ");
            fflush(stdin);
            scanf("%d %ld", &s.tel.DDD, &s.tel.numero);

            fseek(f,-sizeof(a), SEEK_CUR); //Move a posição do ponteiro do arquivo para a posição atual daquele determinado contato que estava sendo pesquisado para ser substituido.
            fwrite(&s, sizeof(a), 1, f); //Realiza a transcrição dos dados da agenda auxiliar 's' para o arquivo agenda.txt.
            flag = 1;
            break;
            }
        fflush(stdin);
        }

    if (flag == 1)
        printf("\n Seus dados foram modificados");
    else
        printf(" \n Os dados nao foram encontrados");

    fclose(f);
    printf("\n Pressione qualquer tecla...");
    getch();
    system("cls");
    menu_principal();
}

//LIXEIRA - LIMPAR AGENDA
/* Realiza a limpeza da agenda.txt*/
void lixeira(FILE *f){
	system("cls");
    printf("\nErro ao abrir o arquivo! -> Ele ja foi removido ou ainda nao existe\n");
    // Ao tentar remover um arquivo que não existe a função retorna o valor -1
    printf("\nRetorno: %d -> Indica erro\n", remove(ARQ_AGENDA_TELEFONICA));
    exit(0);
}