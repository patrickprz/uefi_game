#!/bin/bash

# 1. Caminho absoluto do seu projeto atual
MEU_PROJETO_DIR="$(pwd)/Demo"

# 2. Caminho absoluto para onde você clonou o edk2
EDK2_DIR="$(pwd)/edk2" # Altere se necessário

# 3. Define o WORKSPACE no SEU projeto (Impede que a pasta Build vá para o edk2)
export WORKSPACE="$MEU_PROJETO_DIR"

# 4. Define o PACKAGES_PATH unindo seu projeto com o EDK2
export PACKAGES_PATH="$MEU_PROJETO_DIR:$EDK2_DIR"

# 5. Entra na pasta do EDK2 apenas para carregar as ferramentas de ambiente
pushd "$EDK2_DIR" > /dev/null
  source edksetup.sh
popd > /dev/null

# 6. Executa o build 
build -a X64 -t GCC -p DemoPkg/DemoPkg.dsc -m DemoPkg/MyApps/HelloWorld/HelloWorld.inf
