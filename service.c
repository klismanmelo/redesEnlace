#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUADRO_SIZE 1024  // Tamanho máximo do quadro
#define MAX_DADOS_SIZE 512   // Tamanho máximo dos dados

// Estrutura do quadro de dados
typedef struct {
    char endereco_origem[6];  // Endereço de origem (ex: MAC Address)
    char endereco_destino[6]; // Endereço de destino (ex: MAC Address)
    unsigned short tipo_protocolo;  // Tipo de protocolo (ex: Ethernet type)
    char dados[MAX_DADOS_SIZE];  // Dados (payload)
    unsigned short crc;  // Trailer (CRC simulado)
} Quadro;

// Função para calcular um CRC simples (simulação)
unsigned short calcular_crc(const char* dados, size_t tamanho) {
    unsigned short crc = 0xFFFF;
    for (size_t i = 0; i < tamanho; i++) {
        crc ^= dados[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

// Função para ler os dados do arquivo
size_t ler_arquivo(const char* nome_arquivo, char* buffer, size_t tamanho_max) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }
    size_t bytes_lidos = fread(buffer, 1, tamanho_max, arquivo);
    fclose(arquivo);
    return bytes_lidos;
}

// Função para criar o quadro de dados
void criar_quadro(Quadro* quadro, const char* dados, size_t tamanho_dados) {
    // Endereço de origem e destino fictícios (exemplo de endereços MAC)
    memcpy(quadro->endereco_origem, "\x01\x02\x03\x04\x05\x06", 6);
    memcpy(quadro->endereco_destino, "\x06\x05\x04\x03\x02\x01", 6);
    
    // Tipo de protocolo fictício (ex: 0x0800 para IPv4 em Ethernet)
    quadro->tipo_protocolo = 0x0800;
    
    // Copiar os dados para o quadro
    memcpy(quadro->dados, dados, tamanho_dados);
    
    // Calcular o CRC para o trailer
    quadro->crc = calcular_crc(quadro->dados, tamanho_dados);
}

// Função para exibir o quadro
void exibir_quadro(const Quadro* quadro, size_t tamanho_dados) {
    printf("Quadro de Dados:\n");
    printf("Endereço de Origem: ");
    for (int i = 0; i < 6; i++) {
        printf("%02X ", (unsigned char)quadro->endereco_origem[i]);
    }
    printf("\n");

    printf("Endereço de Destino: ");
    for (int i = 0; i < 6; i++) {
        printf("%02X ", (unsigned char)quadro->endereco_destino[i]);
    }
    printf("\n");

    printf("Tipo de Protocolo: 0x%04X\n", quadro->tipo_protocolo);

    printf("Dados:\n");
    for (size_t i = 0; i < tamanho_dados; i++) {
        printf("%c", quadro->dados[i]);
    }
    printf("\n");

    printf("CRC: 0x%04X\n", quadro->crc);
}

int main() {
    const char* nome_arquivo = "dados.txt";
    char dados[MAX_DADOS_SIZE];
    size_t tamanho_dados = ler_arquivo(nome_arquivo, dados, sizeof(dados));

    if (tamanho_dados == 0) {
        return 1;  // Erro ao ler o arquivo
    }

    Quadro quadro;
    criar_quadro(&quadro, dados, tamanho_dados);

    exibir_quadro(&quadro, tamanho_dados);

    return 0;
}
