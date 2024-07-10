#ifndef __VEHICLE_COLLECTION_HPP__  // Verifica se o cabeçalho já foi incluído
#define __VEHICLE_COLLECTION_HPP__   // Define o cabeçalho para evitar múltiplas inclusões

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

#include "viatura.hpp"

namespace vehicle_collection {
    class DuplicateValue : public std::invalid_argument {
        using std::invalid_argument::invalid_argument;
    };

    class VehicleCollection {
    private:
        // atributo da classe
        std::vector<Viatura> viaturas;
    
    public:
        std::vector<Viatura> get_collection() {
            return this->viaturas;
        }

        /**
         *  Função que cria um objeto da Classe VehicleCollection
         *  e atribui dados(Viaturas) a partir de um ficheiro CSV.
         */
        static VehicleCollection from_csv(const std::string& path) {
            VehicleCollection viaturas;
    
            std::ifstream csv_file(path);
            std::string line;
            while (std::getline(csv_file, line)) {
                //se após remoção dos espaços a direita e esquerda
                // a string estiver vazia
                if (utils::trim(line).empty()) {
                    continue; //pula para próxima linha
                }
    
                if (line.find("##") == 0 || line.find("//") == 0) {
                    continue;
                }
                viaturas.add(Viatura::from_csv(line));
            }
            return viaturas;
        }

        /**
         * Função que subscreve linha a linha convertendo cada elem(Viatura) da
         * coleção em  formato CSV, adicionando uma quebra de linha ao fim de cada elemento
         * da coleção, enquando esse não for o último.
         */
        void to_csv(const std::string& path) {
            std::ofstream csv_file;
            csv_file.open(path);
   
            for (std::size_t i = 0; i < viaturas.size(); i += 1) {
                csv_file << viaturas[i].to_csv();
   
                if (i < viaturas.size() - 1) {
                    csv_file << '\n';
                }
            }
            fmt::println("");     
        }
     
        /**
        * Função que verfica se uma matrícula já existe na coleção
        */
        std::optional<Viatura> search_by_mat(const std::string& matricula) {
            for (const auto& mat : this->viaturas) {
                if (mat.get_matricula() == matricula) {
                    return mat;
                }
            }
            return {};
        }

        /**
         * Função que recebe uma viatura e adiciona à coleção, mas antes verifica se já existe
         * se existir, uma exceção é lançada.
         */
        void add(const Viatura& viat) {
            if (this->search_by_mat(viat.get_matricula())) {
                throw DuplicateValue(fmt::format("Matricula {} já existe", viat.get_matricula()));
            }
            this->viaturas.emplace_back(viat);
        }

        /**
         * Função para deletar uma Viatura da coleção, na posição onde
         * a matricula fornecida for encontrada.
         */
        bool delete_(const std::string& matricula) {
            for (std::size_t i = 0; i < viaturas.size(); i++) {
                if (matricula == viaturas[i].get_matricula()) {
                    //viaturas[i].mostra();
                    viaturas.erase(viaturas.begin() + i);
                    return true;
                }
            }
            return false;
        }
    
        /**
         * Template de função pesquisar que recebe um predicado(lambda) no critério da pesquisa
         */
        template<typename F>
        VehicleCollection search(F funcao_criterio) const {
            VehicleCollection found_viaturas;
            for (const auto& viat : this->viaturas) {
                if (funcao_criterio(viat)) {
                    found_viaturas.add(viat);
                }
            }
            return found_viaturas;
        }
    
        /**
         * sintaxe para transformar a colecão de objetos iterável
         */
        std::vector<Viatura>::iterator begin() {
            return this->viaturas.begin();
        }
    
        std::vector<Viatura>::iterator end() {
            return this->viaturas.end();
        }
    
        std::size_t size() const {
            return this->viaturas.size();
        }
    
        bool empty() const {
            return this->viaturas.empty();
        }
    };
}

#endif
