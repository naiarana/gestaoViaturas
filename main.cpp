#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <optional>
#include <stdexcept>
#include <map>
#include <regex>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/core.h>
#include <cstdlib>

#include "Utils.hpp"
#include "viatura.hpp"
#include "vehicle_collection.hpp"
 
using namespace std;
using namespace fmt;
using namespace vehicle_collection;
 
/**
 *  Variáveis globais
 */ 
VehicleCollection viaturas; 
const int DEFAULT_INDENTATION = 3;
 
/**
 * Função para exibir mesagens na consola com identação padrão ou customizada.
 */
void show_msg(const string& msg, int indent = DEFAULT_INDENTATION) {
    println("{}{}", string(indent, ' '), msg);
}
 
/**
 *  Função que interage com utilizador. Output com identação parametrizada,
 *  e retorno do input com conversão para maiúsculas.
 */
string ask(const string& question, int indent = DEFAULT_INDENTATION) {
    print("{}{}", string(indent, ' '), question);
    string answer;
   
    getline(cin, answer);
    string answerToUpper =utils::to_upper_copy(answer);
    return answerToUpper;
}
 
/**
 *  Função limpa tela multiplataforma
 */
void clear_screen() {
#if defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
    system("clear");
#elif _WIN32
    system("cls");
#endif
}
 
/**
 *  Função pausar  multiplataforma
 */
void pause_(int indent = 3) {
#if defined(__linux__) || defined(__FreeBDS__)  || defined (__APPLE__)
    show_msg("Pressione qualquer tecla para continuar...");
    system("read -s -n 1");
#elif _WIN32
    show_msg("Pressione qualquer tecla para continuar...");
    system("pause>nul");     // NOTA: confirmar se comando está correcto em Windows
#else
    println("   Pressione ENTER para continuar...");
    string nao_interessa;
    getline(cin, nao_interessa);
#endif
}
 

/**
 *  Função que define e exibe em formato de tabela
 *  todas as viaturas da coleção.
 */
void show_table_with_viats(VehicleCollection& viaturas) {
    auto header = format(
        "{:^16} | {:^20} | {:^20} | {:^16}",
        "MATRICULA", "MARCA", "MODELO", "DATA"
    );
    auto sep = format(
        "{}-+-{}-+-{}-+-{}",
        string(16, '-'), string(20, '-'), string(20, '-'), string(16, '-')
    );
   
    show_msg(header);
    show_msg(sep);
 
    for (const auto& viat : viaturas) {
        auto data_line = format(
            "{:<16} | {:<20} | {:<20} | {:>16}",
            viat.get_matricula(),
            viat.get_marca(),
            viat.get_modelo(),
            viat.get_data()
           
        );
        show_msg(data_line);
        
    }
 
    pause_();
}

/**
 *  Função que define e exibe em formato de tabela
 *  uma única  viatura da coleção.
 */
void show_table_for_viat(const VehicleCollection& viatura) {
    viaturas.get_collection();
    //viaturas.add(viatura);
    show_table_with_viats(viaturas);
}
 
/**
 *  Função que lista todas as viaturas.
 */
void exec_list_viats() {
    clear_screen();
    println("");
    show_table_with_viats(viaturas);
    //pause_();
}

/**
 *  Função para pesquisa na coleção de uma viatura pela matricula
 */
void exec_search_by_matricula() {
    clear_screen();
    println("");
 
    show_msg("PESQUISA POR MATRICULA\n");
    auto matricula = ask("Indique a matricula das viaturas a pesquisar: ");
    println("");
 
    auto encontrados = viaturas.search([matricula](const Viatura& viat) {
        return viat.get_matricula() == matricula;
        }
    );
    if (encontrados.empty()) {
        show_msg(format("Não foram encontrados viaturas deste modelo {}", matricula));
    }
    else {
        show_table_with_viats(encontrados);
    }
 
    println("");
    
}

/**
 *  Função para pesquisa na coleção de uma viatura pela marca
 */
void exec_search_by_marca(){
    clear_screen();
    println("");
 
    show_msg("PESQUISA POR MARCA\n");
    auto marca = ask("Indique a marca das viaturas a pesquisar: ");
    println("");
   
    auto por_marca = [marca](const Viatura& viat) {
        return viat.get_marca() == marca;
    };
 
    auto encontrados = viaturas.search(por_marca);
    if (encontrados.empty()) {
        show_msg(format("Não foram encontrados viaturas com essa marca {}", marca));
        pause_();
    }
    else {
        show_table_with_viats(encontrados);
    }

}

/**
 *  Função para pesquisa na coleção de uma viatura pelo modelo
 */
void exec_search_by_modelo() {
    clear_screen();
    println("");
 
    show_msg("PESQUISA POR MODELO\n");
    auto modelo = ask("Indique o modelo das viaturas a pesquisar: ");
    println("");
 
    auto encontrados = viaturas.search([modelo](const Viatura& viat) {
        return viat.get_modelo() == modelo;
        }
    );
    if (encontrados.empty()) {
        show_msg(format("Não foram encontrados viaturas deste modelo {}", modelo));
    }
    else {
        show_table_with_viats(encontrados);
    }
 
    println("");
  
}
  
/**
 *  Função para inserir uma nova viatura a coleção.
 *  Verifica se matrícula já existe na VehicleCollection,
 *  valida  as informações do utilizador, e então cria-se
 *  uma nova viatura.
 */
void acresc_viatura(){
    clear_screen();
    println("");
    string matricula;
    string marca;
    string modelo;
    string data;
 
    show_msg("**Novo veiculo**");
   
    while (true) {
        println("");
        matricula = ask("Insira Matricula: ");
        if (!(viaturas.search_by_mat(matricula)) && Viatura::valida_matricula(matricula)) {
            break;
        }
        println("   Matricula {} já existe ou formato incorreto!", matricula);
    }
 
    while (true) {
        println("");
        marca = ask("Insira Marca: ");
        if (Viatura::valida_marca(marca)) {
            break;
        }
        println("   Marca {} inválida", marca);
    }
 
    while (true) {
        println("");
        modelo = ask("Insira Modelo: ");
        if(Viatura::valida_modelo(modelo)){
            break;
        }
        println("   Modelo {} inválida", modelo);
    }
 
    while (true) {
        println("");
        data = ask("Insira data formato aaaa-MM-dd: ");
        if(Viatura::valida_data(data)){
            break;
        }
        println("   Data {} inválida", data);
    }
   
    Viatura novaViatura(matricula,marca,modelo,data);
    viaturas.add(novaViatura);
    println("");
    show_msg("Novo veiculo acrescentado com sucesso!\n");
    pause_();
    //cin.ignore(1000,'\n');
}
 
/**
 *  Função para deletar uma viatura da coleção.
 *  Verifica se matrícula existe na VehicleCollection,
 *  se encontrada é deletada.
 */
void delete_viat(){
    string matricula;
    clear_screen();
    println("");
    exec_list_viats();
    println("");
    show_msg("**DELETAR**");
    while (true) {
   
        matricula = ask("Insira Matricula que deseja deletar: ");
        if (Viatura::valida_matricula(matricula)) {
            break;
        }
        println("   Matricula {} formato incorreto!", matricula);
    }
 
    if(viaturas.delete_(matricula)){
            show_msg("Viatura excluido com sucesso!");
        }
    else{
        show_msg("Viatura não encontrado!");
    }
    
}

/**
 *  Função que finaliza o programa e atualiza o ficheiro com todas as
 *  atualizações feitas a coleção pelo utilizador.
 */
void exec_end() {
    println("");
    show_msg("[+] A actualizar catálogo ...");
    viaturas.to_csv("viaturas.csv");// gravar catálogo em disco
    show_msg("[+] ... catálogo actualizado");
    show_msg("[+] Programa vai terminar.");
    exit(0);
}
 
/**
 *  Funções do Main, com interações com utilizador.
 */
void exec_menu() {
    while (true) {
        clear_screen();
        println("");
        show_msg("#################################################");
        show_msg("#                                               #");
        show_msg("#  L  - Listar catálogo                         #");
        show_msg("#  P  - Pesquisar por matricula                 #");
        show_msg("#  PM - Pesquisar por marca                     #");
        show_msg("#  PN - Pesquisar por modelo                    #");
        show_msg("#  A  - Acrescentar viatura                     #");
        show_msg("#  E  - Eliminar viatura                        #");
        show_msg("#  G  - Guardar catálogo em ficheiro            #");
        show_msg("#                                               #");
        show_msg("#  T  - Terminar o programa                     #");
        show_msg("#                                               #");
        show_msg("#################################################");
        println("");
 
        auto opcao = ask("OPÇÃO> ");
        auto OPCAO = utils::to_upper_copy(opcao);
 
        if (OPCAO == "L" || OPCAO == "LISTAR") {
            exec_list_viats();
            
        }
        else if(OPCAO == "P" || OPCAO == "PESQUISAR POR MATRICULA"){
           exec_search_by_matricula();  
        }
        else if(OPCAO == "PM" || OPCAO == "PESQUISAR POR MARCA"){
           exec_search_by_marca();  
        }
        else if(OPCAO == "PN" || OPCAO == "PESQUISAR POR MODELO"){
           exec_search_by_modelo();  
        }
        else if(OPCAO == "A" || OPCAO == "ACRESCENTAR"){
            acresc_viatura();
        }
        else if(OPCAO == "G" || OPCAO == "GUARDAR"){
            viaturas.to_csv("viaturas.csv");
            show_msg("Guardado com sucesso!");
            pause_();  
        }
        else if(OPCAO == "E" || OPCAO == "ELIMINAR"){
            delete_viat();
            pause_();
        }
        else if (OPCAO == "T" || OPCAO == "TERMINAR") {
            exec_end();
        }
        else {
            println("");
            println("ATENÇÃO: Opção {} inválida", opcao);
            
        }
    }
}
  
int main() {
    viaturas = VehicleCollection::from_csv("viaturas.csv");
    exec_menu();
}