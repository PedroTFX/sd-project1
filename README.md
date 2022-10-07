# sd-project1

## Tests

Todos os testes passam: `make test_all`

## Memory Leaks

Só sobra um memory leak no tree_del(). Suspeita-se que falte libertar o nó que substitui o apagado (depois de copiado para o nó que é suposto apagar).

## Notas

Não houve alterações na serialização.
