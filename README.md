# Shell


## Colaboradores
 - Gustavo Mota
 - Ian maeda
 - Murilo Croce


## Pré-requisitos
1. Ter um sistema operacional Linux, seja VM, físico ou WSL

   - Link para download do Ubuntu Server -> https://ubuntu.com/download/server#manual-install
   - Link para download do Virtual Box   -> https://www.virtualbox.org/wiki/Downloads

2. Para compilação utilizar o GCC (já presente no sistema Linux)
   - Para compilação, executar `gcc -o shell Shell_SO.c`
   - Para executar o programa em si, executar `./shell`
  
## Funcionalidades básicas
- Comandos internos:
  - `exit`: Mata o shell
  - `cd <caminho>`: Muda o diretório de trabalho.
  - `path <caminho> [<caminho> <caminho> ...]`: Define um novo path para o shell*.
*O path utiliza o path local da variável de ambiente.

##Funcionalidades Extras
  ### cat
   - Compilar: `gcc -o cat cat.c`
   - Executar: `./cat <arquivo>`

  ### ls
   - Compilar: `gcc -o ls ls.c`
   - Execitar: `./ls <arg1> <arg2>`
   -args:

   -`-l` Lista características dos arquivos
   
   -`-a` Show_All, mostra todos os arquivos, incluindo os ocultos

## Funcionamento do arquivo batch
- Para executar um arquivo batch utilize `run <arquivo batch>`

