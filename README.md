# PSE em Ação

**Sistema de Planejamento e Acompanhamento de Ações de Saúde na Escola**

Aplicação em linguagem C, executada em terminal, que apoia equipes escolares e de saúde no planejamento, no registro e no acompanhamento de **ações coletivas** do Programa Saúde na Escola (PSE).

> Atividade de Estudo Prático (AEP) — Engenharia de Software / Análise e Desenvolvimento de Sistemas — UniCesumar — 2º semestre de 2026.

## Integrantes

| Nome | RA |
|---|---|
| Guilherme Bispo Garcia de Oliveira | 26005126-2 |
| Henry Hashimoto Tanaka | 26002577-2 |

## Funcionalidades

- Cadastro de ações (código, escola, tema, público-alvo, responsável, data, participantes previstos e situação inicial)
- Listagem completa das ações
- Pesquisa por código, escola ou tema (busca parcial, sem diferenciar maiúsculas e minúsculas)
- Atualização de situação (planejada, realizada ou cancelada) e registro de participantes efetivos
- Resumo geral: ações por situação, participação em relação ao previsto e consolidado por escola
- Carga de 8 ações fictícias para demonstração
- Validação de todas as entradas: código repetido, campos vazios, quantidades negativas, datas inválidas, opções inexistentes e limite de armazenamento

## Como compilar e executar

Pré-requisito: compilador C (GCC/MinGW, Code::Blocks, Dev-C++ ou um compilador online).

**Linux / macOS**
```bash
gcc src/pse_em_acao.c -o pse_em_acao -Wall -Wextra
./pse_em_acao
```

**Windows (MinGW)**
```bat
gcc src\pse_em_acao.c -o pse_em_acao.exe -Wall -Wextra
pse_em_acao.exe
```

Para conhecer o sistema rapidamente, escolha a opção **6** para carregar dados fictícios e depois a opção **5** para ver o resumo.

## Estrutura do repositório

```
pse-em-acao/
├── README.md
├── src/
│   └── pse_em_acao.c
└── docs/
    ├── 1a_entrega/   documento da 1ª entrega
    ├── 2a_entrega/   documento final
    ├── diagramas/    fluxogramas e casos de uso (PNG + fontes)
    └── capturas/     capturas de tela do sistema
```

## Privacidade e limites éticos

O sistema trabalha **somente com dados fictícios e coletivos**. Não há campos para nome, diagnóstico, prontuário ou qualquer dado individual de estudantes, e o programa não realiza diagnóstico, triagem, prescrição ou recomendação de tratamento.

## Documentação e apresentação

- [Documento da 1ª entrega](docs/1a_entrega/)
- [Documento final](docs/2a_entrega/)
- [Vídeo de apresentação](https://youtu.be/SEU-LINK)

## Limitações conhecidas

Os dados ficam apenas em memória e são perdidos ao sair. O limite é de 50 ações, e as mensagens são exibidas sem acentuação, para compatibilidade com o terminal do Windows.
